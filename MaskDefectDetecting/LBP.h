#pragma once
#ifndef __LBP_H__
#define __LBP_H__
#include "opencv2/opencv.hpp"
#include<vector>
using namespace std;
using namespace cv;

class LBP
{
public:
	LBP();
	~LBP();

	// �ҶȲ���LBP(256��ģʽ)
	void LBP_256(const Mat &srcImage, Mat &LBPImage);// ����256άLBP����ͼ


	// �ҶȲ���+�ȼ�ģʽLBP(58��ģʽ),û�п�����ת����
	void ComputerLBPFeatureVector(const Mat &srcImage, Size cellSize, vector<float> &featureVector);// ����LBP����
	void UniformLBP(const Mat &srcImage, Mat &LBPImage, vector<float> &LBPFeatureVector, int indexOfCell); // ����ÿ��cell������ͼ����������

	void BuildUniformPatternTable(int *table);
	int GetHopCount(int i);// ��ȡi��0,1���������
	void UniformLBP_LUT(const Mat &srcImage, Mat &LBPImage);// ʹ�ò��ұ���ȡLBP����ͼ

	// �ҶȲ���+��ת����+�ȼ�ģʽLBP(9��ģʽ)
	void ComputerLBPFeatureVector_Rotation(const Mat &srcImage, Size cellSize, vector<float> &featureVector);// ����LBP����
	void RotationUniformLBP(const Mat &srcImage, Mat &LBPImage, vector<float> &LBPFeatureVector, int indexOfCell);// ��������ͼ
	int ComputerValue9(int value58);// ����9�ֵȼ�ģʽ
	uchar GetMinBinary(uchar *binary);

	void Test();// ���ԻҶȲ���+��ת����+�ȼ�ģʽLBP
};

#endif