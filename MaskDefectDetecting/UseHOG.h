//ʹ��SVMѵ��HoG
#pragma once
#include <iostream>//���������
#include "highgui.h"
#include <ml.h>//opencv����ѧϰͷ�ļ�������SVM
using namespace std;
using namespace cv;
#pragma once
class CUseHOG
{
public:
	CUseHOG();
	~CUseHOG(); 
	int computeGradient(cv::Mat src, cv::Mat &gradImage, cv::Mat &gradOrient);//����ͼ���ݶ�
	int TrainHOGwithSVM(string trainSetPath, int imgWidth ,int imgHight, string SVMSavePath);//����ѵ����Ŀ¼��ѵ��������������ȡHOG������ѵ��SVM������
	int Predict(string testSetPath, string SVMLoadPath, int imgWidth, int imgHight, string PredictSavePath);//������Լ�Ŀ¼��������Ŀ¼��ѵ������������Ԥ�������·������������Ԥ��
	int PredictSingleImage(cv::Mat src, int imgWidth, int imgHight, CvSVM &SVM);//����ͼ����������������SVM�����������е���ͼ��Ԥ��
};

