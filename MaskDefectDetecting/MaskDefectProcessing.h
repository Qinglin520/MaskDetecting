#pragma once
#include <cv.h>
#include <highgui.h>

using namespace cv;

class CMaskDefectProcessing
{
public:
	CMaskDefectProcessing();
	~CMaskDefectProcessing();

public:
	//int ThresholdOtsu(IplImage * src);// OTSU�����䷽������Ӧ��ֵ�ָ�(�Ҷ�ͼ),������ֵ
	///*===============================================*/
	//int BlockDetect(Mat src, int width, int height, int &x, int &y);//�жϻ���ͼ���Ƿ��ǽǡ�
	//int TotalData(Mat src);//ͳ�ƻҶ�ֵ
	//int BlockSearchCorner(Mat src);//����Ѱ�ǣ���ֵͼ�����õ�m_fourCorner[]��������һ��
	////=========
	//Point2f computeIntersect(Vec4i a, Vec4i b);//������
	//void sortCorners(std::vector<Point2f>& corners, Point2f center);//�ǵ�����
	//int Transformation(Mat src);// �����任
	////=========
	//bool LineDetect(Mat line,int coordinate);//���жϣ����ؽǵ����ꡣ
	//int LineScanCorrect(Mat src);//��ɨ�������һ��
	////=========
	int SkewCorrection(Mat src);//��б����




	/*===============================================*/
public:
	Mat m_NowImage;//show1��ǰ��ʾͼƬ
	//std::vector<Mat> pImageHistory;//ͼƬ����
	//int m_HistoryNum;//��ʼ��ͼƬ������
	//Point2f m_Corner[4];//�ĸ��ǵ�����
	//vector<Point2f> m_corners;////�ĸ��ǵ�����
	Point2f m_center;//���ĵ�����

	//float carCos[181];
	//float carSin[181];


	RotatedRect m_maskrect;//������С��Ӿ���

};

