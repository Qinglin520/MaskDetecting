#include "stdafx.h"
#include "LBP.h"


LBP::LBP()
{
}


LBP::~LBP()
{
}

//srcImage:�Ҷ�ͼ
//LBPImage:LBPͼ
void LBP::LBP_256(const Mat &srcImage, Mat &LBPImage)
{
	// ������飬�ڴ����
	int heightOfLBP = srcImage.rows;
	int widthOfLBP = srcImage.cols;
	int depth = srcImage.depth();
	int channels = srcImage.channels();
	CV_Assert(depth == CV_8U && channels == 1);
	LBPImage.create(Size(widthOfLBP, heightOfLBP), CV_8UC1);

	// ����LBP����ͼ
	// ����ԭͼ��߽磬���ڱ߽紦��
	Mat extendedImage;
	copyMakeBorder(srcImage, extendedImage, 1, 1, 1, 1, BORDER_DEFAULT);

	// ����LBP
	int heightOfExtendedImage = extendedImage.rows;
	int widthOfExtendedImage = extendedImage.cols;
	uchar *rowPointer_ToExtended = extendedImage.data + widthOfExtendedImage + 1;
	uchar *rowPointer_ToLBP = LBPImage.data;
	int LBPValue = 0;// ÿ�����ص�LBPֵ
	for (int y = 1; y <= heightOfExtendedImage - 2; ++y)
	{
		// ��
		uchar *colPointer_ToExtended = rowPointer_ToExtended;
		uchar *colPointer_ToLBP = rowPointer_ToLBP;
		for (int x = 1; x <= widthOfExtendedImage - 2; ++x)
		{
			LBPValue = 0;
			if (colPointer_ToExtended[0 - widthOfExtendedImage - 1] >= colPointer_ToExtended[0])
				LBPValue += 128;
			if (colPointer_ToExtended[0 - widthOfExtendedImage] >= colPointer_ToExtended[0])
				LBPValue += 64;
			if (colPointer_ToExtended[0 - widthOfExtendedImage + 1] >= colPointer_ToExtended[0])
				LBPValue += 32;
			if (colPointer_ToExtended[0 + 1] >= colPointer_ToExtended[0])
				LBPValue += 16;
			if (colPointer_ToExtended[0 + widthOfExtendedImage + 1] >= colPointer_ToExtended[0])
				LBPValue += 8;
			if (colPointer_ToExtended[0 + widthOfExtendedImage] >= colPointer_ToExtended[0])
				LBPValue += 4;
			if (colPointer_ToExtended[0 + widthOfExtendedImage - 1] >= colPointer_ToExtended[0])
				LBPValue += 2;
			if (colPointer_ToExtended[0 - 1] >= colPointer_ToExtended[0])
				LBPValue += 1;

			// LBPͼ
			colPointer_ToLBP[0] = LBPValue;

			// ��һ������
			colPointer_ToExtended++;
			colPointer_ToLBP++;

		}

		// ��һ��
		rowPointer_ToExtended += widthOfExtendedImage;
		rowPointer_ToLBP += widthOfLBP;

	}// end of y


}

// ʹ�ò��ұ���ȡLBP����ͼ,ע��,Ϊ�˷����ʾ����ͼ��58�ֵȼ�ģʽ��ʾΪ1~58,��59�ֻ��ģʽ��ʾΪ0
void LBP::UniformLBP_LUT(const Mat &srcImage, Mat &LBPImage)
{
	// ������飬�ڴ����
	int heightOfLBP = srcImage.rows;
	int widthOfLBP = srcImage.cols;
	int depth = srcImage.depth();
	int channels = srcImage.channels();
	CV_Assert(depth == CV_8U && channels == 1);
	LBPImage.create(Size(widthOfLBP, heightOfLBP), CV_8UC1);

	// ����LBPͼ
	// ����ԭͼ��߽磬���ڱ߽紦��
	Mat extendedImage;
	copyMakeBorder(srcImage, extendedImage, 1, 1, 1, 1, BORDER_DEFAULT);

	// LUT
	static const int table[256] = { 1, 2, 3, 4, 5, 0, 6, 7, 8, 0, 0, 0, 9, 0, 10, 11, 12, 0, 0, 0, 0, 0, 0, 0, 13, 0, 0, 0, 14, 0, 15, 16, 17, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 0, 20, 0, 21, 22, 23, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25,
		0, 0, 0, 0, 0, 0, 0, 26, 0, 0, 0, 27, 0, 28, 29, 30, 31, 0, 32, 0, 0, 0, 33, 0, 0, 0, 0, 0, 0, 0, 34, 0, 0, 0, 0
		, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 36, 37, 38, 0, 39, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42
		, 43, 44, 0, 45, 0, 0, 0, 46, 0, 0, 0, 0, 0, 0, 0, 47, 48, 49, 0, 50, 0, 0, 0, 51, 52, 53, 0, 54, 55, 56, 57, 58 };

	// ����LBP
	int heightOfExtendedImage = extendedImage.rows;
	int widthOfExtendedImage = extendedImage.cols;
	uchar *rowPointer_ToExtended = extendedImage.data + widthOfExtendedImage + 1;
	uchar *rowPointer_ToLBP = LBPImage.data;
	int LBPValue = 0;// ÿ�����ص�LBPֵ
	for (int y = 1; y <= heightOfExtendedImage - 2; ++y)
	{
		// ��
		uchar *colPointer_ToExtended = rowPointer_ToExtended;
		uchar *colPointer_ToLBP = rowPointer_ToLBP;
		for (int x = 1; x <= widthOfExtendedImage - 2; ++x)
		{
			// ����LBP
			LBPValue = 0;
			if (colPointer_ToExtended[0 - widthOfExtendedImage - 1] >= colPointer_ToExtended[0])
				LBPValue += 128;
			if (colPointer_ToExtended[0 - widthOfExtendedImage] >= colPointer_ToExtended[0])
				LBPValue += 64;
			if (colPointer_ToExtended[0 - widthOfExtendedImage + 1] >= colPointer_ToExtended[0])
				LBPValue += 32;
			if (colPointer_ToExtended[0 + 1] >= colPointer_ToExtended[0])
				LBPValue += 16;
			if (colPointer_ToExtended[0 + widthOfExtendedImage + 1] >= colPointer_ToExtended[0])
				LBPValue += 8;
			if (colPointer_ToExtended[0 + widthOfExtendedImage] >= colPointer_ToExtended[0])
				LBPValue += 4;
			if (colPointer_ToExtended[0 + widthOfExtendedImage - 1] >= colPointer_ToExtended[0])
				LBPValue += 2;
			if (colPointer_ToExtended[0 - 1] >= colPointer_ToExtended[0])
				LBPValue += 1;

			colPointer_ToLBP[0] = table[LBPValue];

			// ��һ������
			colPointer_ToExtended++;
			colPointer_ToLBP++;

		}

		// ��һ��
		rowPointer_ToExtended += widthOfExtendedImage;
		rowPointer_ToLBP += widthOfLBP;

	}// end of y
}

//��ȡi��0,1���������
int LBP::GetHopCount(int i)
{
	// ת��Ϊ������
	int a[8] = { 0 };
	int k = 7;
	while (i)
	{
		// ��2ȡ��
		a[k] = i % 2;
		i /= 2;
		--k;
	}

	// �����������
	int count = 0;
	for (int k = 0; k<8; ++k)
	{
		// ע�⣬��ѭ��������
		if (a[k] != a[k + 1 == 8 ? 0 : k + 1])
		{
			++count;
		}
	}
	return count;

}

// �����ȼ�ģʽ��
// ����Ϊ�˱��ڽ���LBP����ͼ��58�ֵȼ�ģʽ��Ŵ�1��ʼ:1~58,��59����ģʽӳ��Ϊ0
void LBP::BuildUniformPatternTable(int *table)
{
	if (table == NULL)
	{
		return;
	}
	memset(table, 0, 256 * sizeof(int));
	uchar temp = 1;
	for (int i = 0; i<256; ++i)
	{
		if (GetHopCount(i) <= 2)
		{
			table[i] = temp;
			temp++;
		}
	}

	// ������
	//for (int i = 0; i < 256;++i)
	//printf("%d,",table[i]);

}

// ��ȡһ��ͼ��LBP����
// cellSize:ÿ��cell�Ĵ�С,��16*16
void LBP::ComputerLBPFeatureVector(const Mat &srcImage, Size cellSize, vector<float> &featureVector)
{
	// ����ÿ�����ڴ�С
	int widthOfCell = cellSize.width;
	int heightOfCell = cellSize.height;
	int numberOfCell_X = srcImage.cols / widthOfCell;// X����cell�ĸ���
	int numberOfCell_Y = srcImage.rows / heightOfCell;

	// ���������ĸ���
	int numberOfDimension = 58 * numberOfCell_X*numberOfCell_Y;
	featureVector.resize(numberOfDimension, 0);

	//ÿ��cell
	Mat ROI, cell, LBPImage;
	IplImage iplImage;

	// ����LBP��������
	for (int y = 0; y <= numberOfCell_Y - 1; ++y)
	{
		for (int x = 0; x <= numberOfCell_X - 1; ++x)
		{
			// ÿ��cell
			ROI = srcImage(Rect(x * widthOfCell, y * heightOfCell, widthOfCell, heightOfCell));
			// ����ÿ��cell
			iplImage = ROI;
			cell = cvarrToMat(&iplImage, true);
			// ����
			UniformLBP(cell, LBPImage, featureVector, (y*numberOfCell_X + x));
		}
	}
}

// ����ÿ��cell������ͼ����������
// LBPImage:LBP����ͼ(����Ϊ�˱��ڽ���LBP����ͼ��58�ֵȼ�ģʽ��Ŵ�1��ʼ:1~58,��59����ģʽӳ��Ϊ0)
// LBPFeature��ÿ��ͼ��LBP����
// indexOfCell:cell����
// ע������Խ���59����ģʽӳ��Ϊ������ֵ����������Ϊ0�ܹ�����ͻ����������ΪҪͻ���ȼ�ģʽ���������Էǵȼ�ģʽ����Ϊ0�ȽϺ�
void LBP::UniformLBP(const Mat &srcImage, Mat &LBPImage, vector<float> &LBPFeatureVector, int indexOfCell)
{
	// ������飬�ڴ����
	int heightOfLBP = srcImage.rows;
	int widthOfLBP = srcImage.cols;
	int depth = srcImage.depth();
	int channels = srcImage.channels();
	CV_Assert(depth == CV_8U && channels == 1);
	LBPImage.create(Size(widthOfLBP, heightOfLBP), CV_8UC1);

	// ����ԭͼ��߽磬���ڱ߽紦��
	Mat extendedImage;
	copyMakeBorder(srcImage, extendedImage, 1, 1, 1, 1, BORDER_DEFAULT);

	// ����LBP �ȼ�ģʽ���ұ�
	int table[256];
	BuildUniformPatternTable(table);

	// ����LBP����ͼ
	int heightOfExtendedImage = extendedImage.rows;
	int widthOfExtendedImage = extendedImage.cols;
	uchar *rowPointer_ToExtended = extendedImage.data + widthOfExtendedImage + 1;
	uchar *rowPointer_ToLBP = LBPImage.data;
	int LBPValue = 0;// ÿ�����ص�LBPֵ
	for (int y = 1; y <= heightOfExtendedImage - 2; ++y)
	{
		// ��
		uchar *colPointer_ToExtended = rowPointer_ToExtended;
		uchar *colPointer_ToLBP = rowPointer_ToLBP;
		for (int x = 1; x <= widthOfExtendedImage - 2; ++x)
		{
			// ����һ���256άLBPֵ
			LBPValue = 0;
			if (colPointer_ToExtended[0 - widthOfExtendedImage - 1] >= colPointer_ToExtended[0])
				LBPValue += 128;
			if (colPointer_ToExtended[0 - widthOfExtendedImage] >= colPointer_ToExtended[0])
				LBPValue += 64;
			if (colPointer_ToExtended[0 - widthOfExtendedImage + 1] >= colPointer_ToExtended[0])
				LBPValue += 32;
			if (colPointer_ToExtended[0 + 1] >= colPointer_ToExtended[0])
				LBPValue += 16;
			if (colPointer_ToExtended[0 + widthOfExtendedImage + 1] >= colPointer_ToExtended[0])
				LBPValue += 8;
			if (colPointer_ToExtended[0 + widthOfExtendedImage] >= colPointer_ToExtended[0])
				LBPValue += 4;
			if (colPointer_ToExtended[0 + widthOfExtendedImage - 1] >= colPointer_ToExtended[0])
				LBPValue += 2;
			if (colPointer_ToExtended[0 - 1] >= colPointer_ToExtended[0])
				LBPValue += 1;

			// ����Uniformģʽ(�ȼ�ģʽ)LBPֵ
			colPointer_ToLBP[0] = table[LBPValue];

			// ��һ������
			++colPointer_ToExtended;
			++colPointer_ToLBP;

		}// end of x

		// ��һ��
		rowPointer_ToExtended += widthOfExtendedImage;
		rowPointer_ToLBP += widthOfLBP;

	}// end of y

	// ����cell��LBP����
	uchar *imageDataOfLBP = LBPImage.data;
	int numberOfPixel = LBPImage.rows*LBPImage.cols;
	int index = 58 * indexOfCell;// ��cell������������ʼλ��
	int sum = 0;
	for (int i = 0; i <= numberOfPixel - 1; ++i)
	{
		// ֻͳ�Ƶȼ�ģʽ
		if (imageDataOfLBP[i] != 0)
		{
			// �ȼ�ģʽת��Ϊ0~57��������imageDataOfLBP[i] - 1
			++LBPFeatureVector[index + imageDataOfLBP[i] - 1];
			++sum;
		}
	}

	// ֱ��ͼ��һ��
	for (int i = 0; i <= 57; ++i)
	{
		LBPFeatureVector[index + i] /= sum;
	}
}

// ����9�ֵȼ�ģʽ
int LBP::ComputerValue9(int value58)
{
	int value9 = 0;
	switch (value58)
	{
	case 1:
		value9 = 1;
		break;
	case 2:
		value9 = 2;
		break;
	case 4:
		value9 = 3;
		break;
	case 7:
		value9 = 4;
		break;
	case 11:
		value9 = 5;
		break;
	case 16:
		value9 = 6;
		break;
	case 22:
		value9 = 7;
		break;
	case 29:
		value9 = 8;
		break;
	case 58:
		value9 = 9;
		break;
	}

	return value9;

}

// ��ȡѭ�������Ƶ���С������ģʽ
uchar LBP::GetMinBinary(uchar *binary)
{
	// ����8��������
	uchar LBPValue[8] = { 0 };
	for (int i = 0; i <= 7; ++i)
	{
		LBPValue[0] += binary[i] << (7 - i);
		LBPValue[1] += binary[(i + 7) % 8] << (7 - i);
		LBPValue[2] += binary[(i + 6) % 8] << (7 - i);
		LBPValue[3] += binary[(i + 5) % 8] << (7 - i);
		LBPValue[4] += binary[(i + 4) % 8] << (7 - i);
		LBPValue[5] += binary[(i + 3) % 8] << (7 - i);
		LBPValue[6] += binary[(i + 2) % 8] << (7 - i);
		LBPValue[7] += binary[(i + 1) % 8] << (7 - i);
	}

	// ѡ����С��
	uchar minValue = LBPValue[0];
	for (int i = 1; i <= 7; ++i)
	{
		if (LBPValue[i] < minValue)
		{
			minValue = LBPValue[i];
		}
	}

	return minValue;

}

// cellSize:ÿ��cell�Ĵ�С,��16*16
void LBP::ComputerLBPFeatureVector_Rotation(const Mat &srcImage, Size cellSize, vector<float> &featureVector)
{
	// ����ÿ�����ڴ�С
	int widthOfCell = cellSize.width;
	int heightOfCell = cellSize.height;
	int numberOfCell_X = srcImage.cols / widthOfCell;// X����cell�ĸ���
	int numberOfCell_Y = srcImage.rows / heightOfCell;

	// ���������ĸ���
	int numberOfDimension = 9 * numberOfCell_X*numberOfCell_Y;
	featureVector.resize(numberOfDimension, 0);

	//ÿ��cell
	Mat ROI, cell, LBPImage;
	IplImage iplImage;

	// ����LBP��������
	for (int y = 0; y <= numberOfCell_Y - 1; ++y)
	{
		for (int x = 0; x <= numberOfCell_X - 1; ++x)
		{
			// ÿ��cell
			ROI = srcImage(Rect(x * widthOfCell, y * heightOfCell, widthOfCell, heightOfCell));

			// ����ÿ��cell
			iplImage = ROI;
			cell = cvarrToMat(&iplImage, true);

			// ����
			RotationUniformLBP(cell, LBPImage, featureVector, (y*numberOfCell_X + x));
		}
	}
}

// ������ת����ȼ�ģʽ��LBPֵ
void LBP::RotationUniformLBP(const Mat &srcImage, Mat &LBPImage, vector<float> &LBPFeatureVector, int indexOfCell)
{
	// ������飬�ڴ����
	CV_Assert(srcImage.depth() == CV_8U&&srcImage.channels() == 1);
	LBPImage.create(srcImage.size(), srcImage.depth());

	// ����ͼ�񣬴���߽����
	Mat extendedImage;
	copyMakeBorder(srcImage, extendedImage, 1, 1, 1, 1, BORDER_DEFAULT);

	// ����LBP �ȼ�ģʽ���ұ�
	int table[256];
	BuildUniformPatternTable(table);

	uchar binary[8] = { 0 };// ��¼ÿ�����ص�LBPֵ
	int heigthOfExtendedImage = extendedImage.rows;
	int widthOfExtendedImage = extendedImage.cols;
	int widthOfLBPImage = LBPImage.cols;
	// ��
	uchar *rowPointer_ToExtendedImage = extendedImage.data + widthOfExtendedImage + 1;
	uchar *rowPointer_ToLBPImage = LBPImage.data;
	for (int y = 1; y <= heigthOfExtendedImage - 2; ++y)
	{
		// ��
		uchar *colPointer_ToExtendedImage = rowPointer_ToExtendedImage;
		uchar *colPointer_ToLBPImage = rowPointer_ToLBPImage;
		for (int x = 1; x <= widthOfExtendedImage - 2; ++x)
		{
			// ������ת����LBP(��С�Ķ�����ģʽ)
			binary[0] = colPointer_ToExtendedImage[0 - widthOfExtendedImage - 1] >= colPointer_ToExtendedImage[0] ? 1 : 0;
			binary[1] = colPointer_ToExtendedImage[0 - widthOfExtendedImage] >= colPointer_ToExtendedImage[0] ? 1 : 0;
			binary[2] = colPointer_ToExtendedImage[0 - widthOfExtendedImage + 1] >= colPointer_ToExtendedImage[0] ? 1 : 0;
			binary[3] = colPointer_ToExtendedImage[0 + 1] >= colPointer_ToExtendedImage[0] ? 1 : 0;
			binary[4] = colPointer_ToExtendedImage[0 + widthOfExtendedImage + 1] >= colPointer_ToExtendedImage[0] ? 1 : 0;
			binary[5] = colPointer_ToExtendedImage[0 + widthOfExtendedImage] >= colPointer_ToExtendedImage[0] ? 1 : 0;
			binary[6] = colPointer_ToExtendedImage[0 + widthOfExtendedImage - 1] >= colPointer_ToExtendedImage[0] ? 1 : 0;
			binary[7] = colPointer_ToExtendedImage[0 - 1] >= colPointer_ToExtendedImage[0] ? 1 : 0;
			int minValue = GetMinBinary(binary);

			// ����58�ֵȼ�ģʽLBP
			int value58 = table[minValue];

			// ����9�ֵȼ�ģʽ
			colPointer_ToLBPImage[0] = ComputerValue9(value58);

			// ��һ������
			++colPointer_ToExtendedImage;
			++colPointer_ToLBPImage;
		}

		// ��һ��
		rowPointer_ToExtendedImage += widthOfExtendedImage;
		rowPointer_ToLBPImage += widthOfLBPImage;
	}

	// ����cell��LBP����
	uchar *imageDataOfLBP = LBPImage.data;
	int numberOfPixel = LBPImage.rows*LBPImage.cols;
	int index = 9 * indexOfCell;// ��cell������������ʼλ��
	int sum = 0;
	for (int i = 0; i <= numberOfPixel - 1; ++i)
	{
		// ֻͳ�Ƶȼ�ģʽ
		if (imageDataOfLBP[i] != 0)
		{
			// �ȼ�ģʽת��Ϊ0~8��������imageDataOfLBP[i] - 1
			++LBPFeatureVector[index + imageDataOfLBP[i] - 1];
			++sum;
		}
	}

	// ֱ��ͼ��һ��
	for (int i = 0; i <= 8; ++i)
	{
		LBPFeatureVector[index + i] /= sum;
	}
}

// ��֤�ҶȲ���+��ת����+�ȼ�ģʽ����
void LBP::Test()
{
	uchar LBPValue[8] = { 0 };
	int k = 7, j;
	int temp;
	LBP lbp;
	int number[256] = { 0 };
	int numberOfMinBinary = 0;

	// ��ת����
	for (int i = 0; i < 256; ++i)
	{
		k = 7;
		temp = i;
		while (k >= 0)
		{
			LBPValue[k] = temp & 1;
			temp = temp >> 1;
			--k;
		}
		int minBinary = lbp.GetMinBinary(LBPValue);

		// ���������ظ���
		for (j = 0; j <= numberOfMinBinary - 1; ++j)
		{
			if (number[j] == minBinary)
				break;
		}
		if (j == numberOfMinBinary)
		{
			number[numberOfMinBinary++] = minBinary;
		}
	}
	cout << "��ת����һ���У�" << numberOfMinBinary << "��" << endl;

	// LUT
	static const int table[256] = { 1, 2, 3, 4, 5, 0, 6, 7, 8, 0, 0, 0, 9, 0, 10, 11, 12, 0, 0, 0, 0, 0, 0, 0, 13, 0, 0, 0, 14, 0, 15, 16, 17, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 0, 0, 0, 0, 0, 0, 0, 19, 0, 0, 0, 20, 0, 21, 22, 23, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25,
		0, 0, 0, 0, 0, 0, 0, 26, 0, 0, 0, 27, 0, 28, 29, 30, 31, 0, 32, 0, 0, 0, 33, 0, 0, 0, 0, 0, 0, 0, 34, 0, 0, 0, 0
		, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 36, 37, 38, 0, 39, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42
		, 43, 44, 0, 45, 0, 0, 0, 46, 0, 0, 0, 0, 0, 0, 0, 47, 48, 49, 0, 50, 0, 0, 0, 51, 52, 53, 0, 54, 55, 56, 57, 58 };
	cout << "��ת����+�ȼ�ģʽ��" << endl;
	for (int i = 0; i <= numberOfMinBinary - 1; ++i)
	{
		cout << number[i] << " " << table[number[i]] << endl;
	}

}