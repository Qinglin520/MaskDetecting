#include "stdafx.h"
#include "MaskDefectProcessing.h"

#define max(a, b)  (((a) > (b)) ? (a) : (b));

using namespace std;

CMaskDefectProcessing::CMaskDefectProcessing()
{
}

CMaskDefectProcessing::~CMaskDefectProcessing()
{
}

//��ԭͼ�õ�m_maskrect��ת���󣬿����ں���Ԥ����ͼ��Ľ�����
int CMaskDefectProcessing::GetMaskRect(Mat src)
{
	Mat   src_bin, src_dilate, src_erode;
	//cvThreshold(g_pGrayImage, g_pBinaryImage, 0, 255, CV_THRESH_OTSU);//��ֵ��
	threshold(src, src_bin, 0, 255, CV_THRESH_OTSU);//��ֵ��������Ӧ��ֵ�ָ
	blur(src_bin, src_bin, Size(3, 3));//ģ��
	Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));//��ʴ�������ӣ�15*15�ľ���
	dilate(src_bin, src_dilate, element);//����
	erode(src_dilate, src_erode, element);//��ʴ
	//imshow("src_erode", src_erode);
	//waitKey(0);

	vector<vector<Point> > contours;//����
	cv::findContours(src_erode,
		contours, // a vector of contours 
		CV_RETR_EXTERNAL, // retrieve the external contours
		CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours �˺���Ϊ�������� minAreaRect() ʹ��

	int area = 0;
	int maxArea=0;//��С��Ӿ������
	for (int i = 0; i < contours.size(); i++) // ��ʵ��С��Ӿ��γߴ磬ֻ�����������Ӿ���
	{
		RotatedRect minRect = minAreaRect(Mat(contours[i]));//�������С�������Ӿ��Σ��ؼ�����
		area = (int)minRect.size.height *(int)minRect.size.width;
		if (area > maxArea)
		{
			maxArea = area;
			m_maskrect = minRect;
		}
	}
	////������С��Ӿ���ʾ��ͼ
	//Mat src_erode_edge;
	//Canny(src_erode, src_erode_edge,4,8);
	//imshow("canny", src_erode_edge);
	//imwrite("canny_4_8.jpg", src_erode_edge);
	//drawContours(src_erode, contours, -1, Scalar(255, 0, 0), 1);
	//Point2f vertices[4];
	//m_maskrect.points(vertices);
	//for (int i = 0; i < 4; i++)
	//{
	//	line(src_erode_edge, vertices[i], vertices[(i + 1) % 4], Scalar(255, 0, 0), 2);
	//}
	//imshow("contours", src_erode_edge);
	//imwrite("minAreaRect.jpg", src_erode_edge);
	//waitKey(0);



	return 0;
}

//�������ܣ���б��������һ���ߴ�
//���룺src��8λ��ͨ��ͼ��maskrect��������С��Ӿ��Σ�Size()����һ�����γߴ硣
//�����Mat dst:�и���ת�����Ĺ�һ������ͼ��
int CMaskDefectProcessing::Normalization(Mat src, RotatedRect maskrect, int rows, int cols,Mat &dst)
{
	//��ƫб���ֻ���
	float r = (float)m_maskrect.size.width / (float)m_maskrect.size.height;//��߱�ֵ
	if (r < 1)//��ȸ߶̣�������ߣ�ƫת�Ǽ�90�ȡ�
	{
		m_maskrect.angle = 90 + m_maskrect.angle;
		std::swap(m_maskrect.size.width, m_maskrect.size.height);

	}
	Size rect_size = m_maskrect.size;
	Mat rotmat;//��ת����
	rotmat = getRotationMatrix2D(m_maskrect.center, m_maskrect.angle, 1);
	Mat img_rotated;//��ת��ͼ��
	warpAffine(src, img_rotated, rotmat, src.size(), CV_INTER_CUBIC);//��ת����������Ϊbw0
	//imshow("img_rotated", img_rotated);
	//waitKey(0);

	//�������ڿ��� resize ��һ����ͳһ�ߴ�� mat �У�Ŀ���ǻ�ñ任���󣬱任�ɸ�����ԭͼ������Ԥ��������
	Mat img_crop;//�ָ���
	getRectSubPix(img_rotated, rect_size, m_maskrect.center, img_crop);//�ָ���ȡ
	//imshow("img_crop", img_crop);
	//waitKey(0);

	Mat resultResized;//����һ����Mat
	resultResized.create(rows, cols, CV_8UC1);//�޸Ĵ�Mat�����Եõ���ͬ�Ĺ�һ����С����Ӧ��Resize����
	resize(img_crop, resultResized, resultResized.size(), 0, 0, INTER_CUBIC);//���ŵ�ͳһ��С
	//imshow("resultResized", resultResized);
	//waitKey(0);

	dst = resultResized;

	return 0;
}

int CMaskDefectProcessing::PreprocessBand(Mat src, Mat dst)//�޽�Ԥ����
{
	return 0;
}

//ͷ��Ԥ����
int CMaskDefectProcessing::PreprocessHair(Mat src,Mat &dst)
{
	int thresh = 110;
	int max_thresh = 255;
	RNG rng(12345);//�����������
	Mat nowImg = src.clone();
	IplImage *pNowImg = (&(IplImage)nowImg);//Mat �� Iplimage*����������

	cvAdaptiveThreshold(pNowImg, pNowImg, 255,
		CV_ADAPTIVE_THRESH_GAUSSIAN_C,
		CV_THRESH_BINARY_INV, 3, 5);//����Ӧ��ֵ��,��ֵ�����������С3,���ڳߴ�5,��˿ͼ����ǿ�ؼ�����
	Mat src_adaptive(pNowImg, 1);//Iplimage* �� Mat����������
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(4, 4));//��ʴ�������ӣ�15*15�ľ���
	dilate(src_adaptive, src_adaptive, element);//����.
	//blur(src_adaptive, src_adaptive, Size(3,3));//ģ��

	////��һ��
	//Normalization(src_adaptive, m_maskrect, m_normalrows, m_normalcols, src_adaptive);
	//imshow("src_adaptive", src_adaptive);

	////��Բ��Ͽ�ʼ
	//Mat threshold_output;
	//vector<vector<Point> > contours;
	//vector<Vec4i> hierarchy;
	///// ��ֵ�����߽�
	//threshold(src_adaptive, threshold_output, thresh, 255, THRESH_BINARY);
	///// Ѱ������
	//findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	///// ��ÿ���ҵ���������������б�ı߽�����Բ
	//vector<RotatedRect> minRect(contours.size());
	//vector<RotatedRect> minEllipse(contours.size());
	//for (int i = 0; i < contours.size(); i++)
	//{
	//	minRect[i] = minAreaRect(Mat(contours[i]));
	//	if (contours[i].size() > 5)
	//	{
	//		minEllipse[i] = fitEllipse(Mat(contours[i]));
	//	}
	//}
	//// ��������������б�ı߽��ͱ߽���Բ
	//Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);
	//for (int i = 0; i< contours.size(); i++)
	//{	//�������ɫ
	//	Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
	//	// contour
	//	drawContours(drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
	//	// ellipse
	//	ellipse(drawing, minEllipse[i], color, 2, 8);
	//	// rotated rectangle
	//	Point2f rect_points[4]; minRect[i].points(rect_points);
	//	for (int j = 0; j < 4; j++)
	//		line(drawing, rect_points[j], rect_points[(j + 1) % 4], color, 1, 8);
	//}

	//dst = drawing;
	Mat tempImg;
	src.copyTo(tempImg, src_adaptive);//����Ե������ͼ����Ϊmask���۳�ԭͼͼ��
	dst = tempImg;
	imshow("dst", dst);
	//������ȡ

	return 0;
}

// ͼ�����Ҿ���任
Mat CMaskDefectProcessing::MirrorTrans(Mat src)
{
	Mat dst;
	dst.create(src.rows, src.cols, src.type());

	int cols = src.cols;

	for (int i = 0; i < cols; i++)
		src.col(cols - i - 1).copyTo(dst.col(i));
	return dst;
}

//ͼ��ƽ���г����룬�ұ�����ת
int CMaskDefectProcessing::Cut2half(Mat src, Mat &imgL, Mat &imgR)
{
	int width = src.size().width;
	int height = src.size().height;
	Range R1, R2;
	R1.start = 0;
	R1.end = width / 2;
	imgL = Mat::Mat(src, Range::all(), R1);
	R2.start = width / 2;
	R2.end = width;
	imgR = Mat::Mat(src, Range::all(), R2);
	//imgR����任
	imgR = MirrorTrans(imgR);
	return 0;
}

int GammaProcess(Mat src, Mat &dst)//GammaУ��
{
	Mat gammaOutput, gammaInput;
	src.convertTo(gammaInput, CV_32FC1);
	float gamma = 1 / 2.2;
	pow(gammaInput, gamma, gammaOutput);
	normalize(gammaOutput, dst, 0, 255, NORM_MINMAX, CV_8UC1);
	return 0;
}