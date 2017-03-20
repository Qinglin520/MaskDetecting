#pragma once
#include <cv.h>
#include <highgui.h>

using namespace cv;

class CMaskDefectProcessing
{
public:
	CMaskDefectProcessing();
	~CMaskDefectProcessing();

	int GetMaskRect(Mat src);//���뵥ͨ��src�����m_maskrect��ת�������ں���������
	int Normalization(Mat src, RotatedRect maskrect, int rows, int cols,Mat &dst);//��б����+�߶ȹ�һ��(8λ��ͨ��ͼ��m_maskrect)
	int PreprocessBand(Mat src, Mat dst);//�޽�Ԥ����
	int PreprocessHair(Mat src, Mat &dst);//ͷ��Ԥ����	
	Mat MirrorTrans(Mat src);// ͼ����任
	int Cut2half(Mat src, Mat &imgL, Mat &imgR);//ͼ��ƽ���г����룬�ұ�����ת
	int GammaProcess(Mat src, Mat &dst);//GammaУ��

public:
	Mat m_NowImage;//��ǰ���ͼ  
	int m_normalrows = 138;//��һ��ͼ���С�� rows:cols = 11:20
	int m_normalcols = 250;//��һ��ͼ���С��rows:cols = 11:20
	RotatedRect m_maskrect;//������С��Ӿ��Σ����ڿ������д���ͼ��Ĺ�һ����
};

