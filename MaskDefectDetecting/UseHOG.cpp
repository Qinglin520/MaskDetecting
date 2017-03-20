#include "stdafx.h"
#include "UseHOG.h"
#include <cv.h>
#include <highgui.h>

#include <fstream> //�ļ�д��Ͷ���



CUseHOG::CUseHOG()
{
}


CUseHOG::~CUseHOG()
{
}

int CUseHOG::computeGradient(Mat src, Mat& gradImage, Mat &gradOrient)
{
	//==========================================================�ݶȷ�ֵ�ļ���
	cv::Mat grad_x, grad_y;
	cv::Mat abs_grad_x, abs_grad_y;
	cv::Mat kerl_x = (Mat_<float>(1, 3) << -1, 0, 1);
	cv::filter2D(src, grad_x, CV_16S, kerl_x, Point(-1, -1), 0, BORDER_DEFAULT);
	cv::convertScaleAbs(grad_x, abs_grad_x);

	cv::Mat kerl_y = (Mat_<float>(3, 1) << -1, 0, 1);
	cv::filter2D(src, grad_y, CV_16S, kerl_y, Point(-1, -1), 0, BORDER_DEFAULT);
	cv::convertScaleAbs(grad_y, abs_grad_y);
	gradImage.create(cv::Size(grad_x.cols, grad_y.rows), CV_8U);

	uchar *totllGrad = gradImage.ptr<uchar>(0);
	uchar *_x_grad = abs_grad_x.ptr<uchar>(0);
	uchar *_y_grad = abs_grad_y.ptr<uchar>(0);
	for (int i = 0; i < grad_x.rows*grad_x.cols; i++)
	{
		totllGrad[i] = sqrt((double)_x_grad[i] * _x_grad[i] + (double)_y_grad[i] * _y_grad[i]);
	}
	//==========================================================�ݶȷ�ֵ�ļ������

	//==========================================================�ݶȷ���ļ���
	gradOrient.create(cv::Size(grad_x.cols, grad_y.rows), CV_8U);
	cv::divide(grad_y, grad_x, gradOrient, 1, -1);

	uchar *gradOrData = gradOrient.ptr<uchar>(0);
	uchar *xgrad = grad_x.ptr<uchar>(0);
	uchar *ygrad = grad_y.ptr<uchar>(0);

	for (int i = 0; i < grad_x.rows*grad_x.cols; i++)
	{
		double orient = atan2((double)ygrad[i], (double)xgrad[i]);                  ///�ݶ�ֵû�и��ģ����·���ȫ��
		//double orient=atan(double(gradOrData[i]));
		gradOrData[i] = 180 * orient / CV_PI + 180;                    //////////////////////////////��֪���ǲ�����ȷ ����0-90�� ��0-270֮�� 
	}
	//==========================================================�ݶȷ���ļ������
	//cout<<gradOrient<<endl;
	return 1;
}

int CUseHOG::TrainHOGwithSVM(string trainSetPath, int imgWidth, int imgHight, string SVMSavePath)
{
	//int imgWidth = 120;
	//int imgHight = 120;
	vector<string> imagePaths;
	vector<int> imageClass;
	string buffer;
	int numberOfLine = 0;


	//ifstream svm_data("E:/apple/SVM_DATA.txt");
	ifstream svm_data(trainSetPath);
	unsigned long n;

	while (svm_data)
	{
		if (getline(svm_data, buffer))
		{
			numberOfLine++;
			if (numberOfLine % 2 == 0)//�����������
			{
				imageClass.push_back(atoi(buffer.c_str()));
			}
			else//ͼƬ·��
			{
				imagePaths.push_back(buffer);
			}
		}
	}
	svm_data.close();//�ر��ļ�    

	Mat data_mat, res_mat;
	int nImgNum = numberOfLine / 2;            //������������    
	////��������nImgNum�������������������� WIDTH * HEIGHT������������������ͼ���С    
	//data_mat = Mat::zeros( nImgNum, 12996, CV_32FC1 );      
	//���;���,�洢ÿ�����������ͱ�־    
	res_mat = Mat::zeros(nImgNum, 1, CV_32FC1);

	//Mat trainImg = Mat::zeros(imgHight, imgWidth, CV_8UC3);//��Ҫ������ͼƬ    
	Mat trainImg;
	for (string::size_type i = 0; i != imagePaths.size(); i++)
	{
		trainImg = imread(imagePaths[i].c_str(), 0);
		//cout << " processing " << imagePaths[i].c_str() << endl;
		resize(trainImg, trainImg, cv::Size(imgWidth, imgHight), 0, 0, INTER_CUBIC);

		HOGDescriptor *hog = new HOGDescriptor(Size(imgWidth, imgHight), Size(16, 16), Size(8, 8), Size(8, 8), 9);//(��ⴰ�ڴ�С�����С��������������Ԫ���Сֻ֧��8*8��ֱ��ͼbin��ֻ֧��9)
		vector<float>descriptors;//�������       
		hog->compute(trainImg, descriptors, Size(1, 1), Size(0, 0)); //���ü��㺯����ʼ���� (ѵ��ͼ�񣬽�������������)  
		if (i == 0)
		{
			data_mat = Mat::zeros(nImgNum, descriptors.size(), CV_32FC1); //��������ͼƬ��С���з���ռ�   
		}
		//cout << "HOG dims: " << descriptors.size() << endl;
		n = 0;
		for (vector<float>::iterator iter = descriptors.begin(); iter != descriptors.end(); iter++)
		{
			data_mat.at<float>(i, n) = *iter;
			n++;
		}
		//cout<<SVMtrainMat->rows<<endl;    
		res_mat.at<float>(i, 0) = imageClass[i];
		//cout << " end processing " << imagePaths[i].c_str() << " " << imageClass[i] << endl;
	}

	CvSVM SVM;
	CvSVMParams param;
	CvTermCriteria criteria;
	criteria = cvTermCriteria(CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
	param = CvSVMParams(CvSVM::C_SVC, CvSVM::RBF, 10.0, 0.09, 1.0, 10.0, 0.5, 1.0, NULL, criteria);

	/*
	SVM���ࣺCvSVM::C_SVC
	Kernel�����ࣺCvSVM::RBF
	degree��10.0���˴β�ʹ�ã�
	gamma��8.0
	coef0��1.0���˴β�ʹ�ã�
	C��10.0
	nu��0.5���˴β�ʹ�ã�
	p��0.1���˴β�ʹ�ã�
	Ȼ���ѵ���������滯����������CvMat�͵������
	*/
	//����������(5)SVMѧϰ�������������           
	SVM.train(data_mat, res_mat, Mat(), Mat(), param);
	//�������ѵ�����ݺ�ȷ����ѧϰ����,����SVMѧϰ�����       
	SVM.save(SVMSavePath.data());
	return 1;
}

int CUseHOG::Predict(string testSetPath, string SVMLoadPath, int imgWidth, int imgHight, string PredictSavePath)
{
	//�������    
	vector<string> testImagePath;
	//ifstream testFile("E:/apple/SVM_TEST.txt");
	ifstream testFile(testSetPath);

	string buffer;
	unsigned long n;

	while (testFile)
	{
		if (getline(testFile, buffer))
		{
			testImagePath.push_back(buffer);
		}
	}
	testFile.close();

	Mat testImg;
	char line[512];
	//ofstream predict_txt("E:/apple/SVM_PREDICT.txt");
	ofstream predict_txt(PredictSavePath);
	CvSVM SVM;
	SVM.load(SVMLoadPath.data());//����ѵ���õ�SVM������

	for (string::size_type j = 0; j != testImagePath.size(); j++)
	{
		testImg = imread(testImagePath[j].c_str(), 0);//����ͼ��     
		resize(testImg, testImg, cv::Size(imgWidth, imgHight), 0, 0, INTER_CUBIC);//Ҫ���ͬ���Ĵ�С�ſ��Լ�⵽         
		HOGDescriptor *hog = new HOGDescriptor(cvSize(imgWidth, imgHight), cvSize(16, 16), cvSize(8, 8), cvSize(8, 8), 9);  //������˼���ο�����1,2       
		vector<float>descriptors;//�������       
		hog->compute(testImg, descriptors, Size(1, 1), Size(0, 0)); //���ü��㺯����ʼ����   
		//cout << "The Detection Result:" << endl;
		//cout << "HOG dims: " << descriptors.size() << endl;
		Mat SVMtrainMat = Mat::zeros(1, descriptors.size(), CV_32FC1);
		n = 0;
		for (vector<float>::iterator iter = descriptors.begin(); iter != descriptors.end(); iter++)
		{
			SVMtrainMat.at<float>(0, n) = *iter;
			n++;
		}
		int ret = SVM.predict(SVMtrainMat);
		std::sprintf(line, "%s %d\n", testImagePath[j].c_str(), ret);
		printf("%s %d\n", testImagePath[j].c_str(), ret);
		getchar();
		predict_txt << line;
	}
	predict_txt.close();

	return 1;
}
int CUseHOG::PredictSingleImage(Mat src, int imgWidth, int imgHight, CvSVM &SVM)
{
	Mat testImg;
	unsigned long n;
	resize(src, testImg, cv::Size(imgWidth, imgHight), 0, 0, INTER_CUBIC);//Ҫ���ͬ���Ĵ�С�ſ��Լ�⵽         
	HOGDescriptor *hog = new HOGDescriptor(cvSize(imgWidth, imgHight), cvSize(16, 16), cvSize(8, 8), cvSize(8, 8), 9);
	vector<float>descriptors;//�������       
	hog->compute(testImg, descriptors, Size(1, 1), Size(0, 0)); //���ü��㺯����ʼ����   
	//cout << "The Detection Result:" << endl;
	//cout << "HOG dims: " << descriptors.size() << endl;
	Mat SVMtrainMat = Mat::zeros(1, descriptors.size(), CV_32FC1);
	n = 0;
	for (vector<float>::iterator iter = descriptors.begin(); iter != descriptors.end(); iter++)
	{
		SVMtrainMat.at<float>(0, n) = *iter;
		n++;
	}
	int predict = SVM.predict(SVMtrainMat);

	return predict;
}