
// MaskDefectDetectingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MaskDefectDetecting.h"
#include "MaskDefectDetectingDlg.h"
#include "afxdialogex.h"
#include "LBP.h"
#include <fstream>
#include <ml.h> 


#ifdef _DEBUG
#define new DEBUG_NEW

//#define CELLSIZE_LBP  8  // LBP�Ĵ��ڴ�С��4,8��16
#endif

using namespace cv;
using namespace std;
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMaskDefectDetectingDlg �Ի���



CMaskDefectDetectingDlg::CMaskDefectDetectingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMaskDefectDetectingDlg::IDD, pParent)//�ؼ�����������ʼ��

	, m_pathOpenBand(_T(""))
	//, m_pathSaveBandNeg(_T(""))
	//, m_pathSaveBandPos(_T(""))
	, m_pathTrainBand(_T(""))

	, num0(1)
	, num1(1)
	, numTestSet(1)
	, dstrows(128)
	, CELLSIZE_LBP(4)
	, TrainImgWidth(64)
	, m_checkUseGamma(1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMaskDefectDetectingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//��������
	DDX_Control(pDX, IDC_INFO, m_info);
	DDX_Text(pDX, IDC_PATH_TRAIN_BAND, m_pathTrainBand);
	DDX_Text(pDX, IDC_PATH_OPEN_BAND, m_pathOpenBand);
	DDX_Text(pDX, IDC_EDIT_BANDNUM0, num0);
	DDX_Text(pDX, IDC_EDIT_BANDNUM1, num1);
	DDX_Text(pDX, IDC_EDIT_BANDNUMTESTSET, numTestSet);
	DDX_Text(pDX, IDC_EDIT_DSTROWS, dstrows);
	dstcols = 2 * dstrows;//��Ҫ�����︳ֵ������ÿ��ˢ�¿����
	DDX_Text(pDX, IDC_EDIT_LBPCELLSIZE, CELLSIZE_LBP);
	DDX_Text(pDX, IDC_EDIT_HOGIMGSIZE, TrainImgWidth);
	TrainImgHeight = TrainImgWidth;
}

BEGIN_MESSAGE_MAP(CMaskDefectDetectingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_READIMG, &CMaskDefectDetectingDlg::OnBnClickedButtonReadimg)
	ON_BN_CLICKED(IDC_BUTTON_CORRECTION, &CMaskDefectDetectingDlg::OnBnClickedButtonCorrection)
	ON_BN_CLICKED(IDC_BUTTON_BANDPROCESS, &CMaskDefectDetectingDlg::OnBnClickedButtonBandprocess)
	ON_BN_CLICKED(IDC_BUTTON_HAIRPROCESS, &CMaskDefectDetectingDlg::OnBnClickedButtonHairprocess)
	ON_EN_CHANGE(IDC_PATH_OPEN_BAND, &CMaskDefectDetectingDlg::OnEnChangePathOpenBand)

	ON_BN_CLICKED(IDC_BUTTON_BATCH_BAND, &CMaskDefectDetectingDlg::OnBnClickedButtonBatchBand)
	ON_BN_CLICKED(IDC_BUTTON_BATCH_BAND2, &CMaskDefectDetectingDlg::OnBnClickedButtonBatchBand2)
	ON_BN_CLICKED(IDC_BUTTON_BATCH_BAND3, &CMaskDefectDetectingDlg::OnBnClickedButtonBatchBand3)
	ON_BN_CLICKED(IDC_BUTTON_BATCH_BAND4, &CMaskDefectDetectingDlg::OnBnClickedButtonBatchBand4)
	ON_BN_CLICKED(IDC_BUTTON_BATCH_BANDTESTSET, &CMaskDefectDetectingDlg::OnBnClickedButtonBatchBandtestset)
	ON_BN_CLICKED(IDC_BUTTON_READIMG_2, &CMaskDefectDetectingDlg::OnBnClickedButtonReadimg2)
	ON_BN_CLICKED(IDC_BUTTON_JUDGE, &CMaskDefectDetectingDlg::OnBnClickedButtonJudge)
	ON_BN_CLICKED(IDC_BUTTON_SVM, &CMaskDefectDetectingDlg::OnBnClickedButtonSvm)
	ON_BN_CLICKED(IDC_BUTTON_SVMPREDICT, &CMaskDefectDetectingDlg::OnBnClickedButtonSvmpredict)
	ON_EN_CHANGE(IDC_PATH_TRAIN_BAND, &CMaskDefectDetectingDlg::OnEnChangePathTrainBand)

	ON_BN_CLICKED(IDC_BUTTON_SVM_LBP_Rotation, &CMaskDefectDetectingDlg::OnBnClickedButtonSvmLbpRotation)
	ON_BN_CLICKED(IDC_BUTTON_SVM_LBP_Rotation_Predict, &CMaskDefectDetectingDlg::OnBnClickedButtonSvmLbpRotationPredict)
	ON_BN_CLICKED(IDC_BUTTON_TRAIN_HOG_SVM, &CMaskDefectDetectingDlg::OnBnClickedButtonTrainHogSvm)
	ON_BN_CLICKED(IDC_BUTTON_PREDICT_HOG_SVM, &CMaskDefectDetectingDlg::OnBnClickedButtonPredictHogSvm)
	ON_BN_CLICKED(IDC_BUTTON_PREHOG, &CMaskDefectDetectingDlg::OnBnClickedButtonPrehog)

	ON_BN_CLICKED(IDC_BUTTON_SAVESHOW2, &CMaskDefectDetectingDlg::OnBnClickedButtonSaveshow2)
	ON_BN_CLICKED(IDC_BUTTON_SAVESHOW3, &CMaskDefectDetectingDlg::OnBnClickedButtonSaveshow3)
	ON_BN_CLICKED(IDC_BUTTON_SAVESHOW4, &CMaskDefectDetectingDlg::OnBnClickedButtonSaveshow4)
	ON_BN_CLICKED(IDC_CHECK_USEGRAYIMG, &CMaskDefectDetectingDlg::OnBnClickedCheckUsegrayimg)
	ON_BN_CLICKED(IDC_CHECK_USEGAMMA, &CMaskDefectDetectingDlg::OnBnClickedCheckUsegamma)
	ON_BN_CLICKED(IDC_BUTTON_CREATDIR, &CMaskDefectDetectingDlg::OnBnClickedButtonCreatdir)
	ON_EN_CHANGE(IDC_EDIT_LBPCELLSIZE, &CMaskDefectDetectingDlg::OnEnChangeEditLbpcellsize)
	ON_EN_CHANGE(IDC_EDIT_HOGIMGSIZE, &CMaskDefectDetectingDlg::OnEnChangeEditHogimgsize)
	ON_EN_CHANGE(IDC_EDIT_DSTROWS, &CMaskDefectDetectingDlg::OnEnChangeEditDstrows)
	ON_BN_CLICKED(IDC_BUTTON_INFO_CLC, &CMaskDefectDetectingDlg::OnBnClickedButtonInfoClc)
	ON_BN_CLICKED(IDC_BUTTON_FUSIONTRAIN, &CMaskDefectDetectingDlg::OnBnClickedButtonFusiontrain)
	ON_BN_CLICKED(IDC_BUTTON_FUSIONPREDICT, &CMaskDefectDetectingDlg::OnBnClickedButtonFusionpredict)
END_MESSAGE_MAP()


// CMaskDefectDetectingDlg ��Ϣ�������

BOOL CMaskDefectDetectingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	m_info.AddString(L"����״̬��Ϣ��");
	((CButton *)GetDlgItem(IDC_CHECK_USEGAMMA))->SetCheck(1);//�Ҷ�ͼGammaУ����ʼ��Ϊѡ��
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMaskDefectDetectingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMaskDefectDetectingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMaskDefectDetectingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//My Code Begin
string ws2s(const wstring& ws)//CString ת string
{
	_bstr_t t = ws.c_str();
	char* pchar = (char*)t;
	string result = pchar;
	return result;
}

void CMaskDefectDetectingDlg::saveImgWithNoBand(Mat src, Mat srcOrigin)//����Ԥ��������޽�ѵ����
{
	CString cstrNum0;
	cstrNum0.Format(_T("%d"), num0);
	CString cstrSaveDir0 = m_pathSaveBandNeg + "\\" + "neg" + cstrNum0 + ".bmp";//Ԥ����󱣴�·��
	CString cstrSaveDirOrigin = m_pathSaveBandNegGray + "\\" + "neg" + cstrNum0 + ".bmp";//ԭͼ����·��
	string strSaveDir0 = ws2s(cstrSaveDir0.GetString());
	string strSaveDir0Origin = ws2s(cstrSaveDirOrigin.GetString());
	imwrite(strSaveDir0, src);
	imwrite(strSaveDir0Origin, srcOrigin);//����ԭͼ���
	num0++;
}

void CMaskDefectDetectingDlg::saveImgWithBand(Mat src, Mat srcOrigin)//����Ԥ��������޽�ѵ����
{
	CString cstrNum1;
	cstrNum1.Format(_T("%d"), num1);
	CString cstrSaveDir1 = m_pathSaveBandPos + "\\" + "pos" + cstrNum1 + ".bmp";
	CString cstrSaveDirOrigin = m_pathSaveBandPosGray + "\\" + "pos" + cstrNum1 + ".bmp";
	string strSaveDir1 = ws2s(cstrSaveDir1.GetString());
	string strSaveDir1Origin = ws2s(cstrSaveDirOrigin.GetString());
	imwrite(strSaveDir1, src);
	imwrite(strSaveDir1Origin, srcOrigin);
	num1++;
}

void CMaskDefectDetectingDlg::DrawPicToHDC(IplImage *img, UINT ID)//�� Iplimage* ͼƬ���Ƶ���ʾ�ؼ�
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf(img); // ����ͼƬ
	cimg.DrawToHDC(hDC, &rect); // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������
	ReleaseDC(pDC);
}
void CMaskDefectDetectingDlg::DrawPicToHDC(Mat src, UINT ID)//�� Mat ��ͼƬ���Ƶ���ʾ�ؼ�
{
	IplImage *img = (&(IplImage)src);//Mat �� Iplimage*
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf(img); // ����ͼƬ
	cimg.DrawToHDC(hDC, &rect); // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������
	ReleaseDC(pDC);
}
void CMaskDefectDetectingDlg::DrawPicToHDC(Mat src, UINT ID, Mat &imgID)//�� Mat ��ͼƬ���Ƶ���ʾ�ؼ�
{
	imgID = src.clone();
	IplImage *img = (&(IplImage)src);//Mat �� Iplimage*
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf(img); // ����ͼƬ
	cimg.DrawToHDC(hDC, &rect); // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������
	ReleaseDC(pDC);
}

void CMaskDefectDetectingDlg::SavePicToWin(Mat img)
{
	// ���ù�����   
	TCHAR szFilter[] = _T("ͼ���ļ�(*.bmp)|*.bmp|�����ļ�(*.*)|*.*||");
	// ���챣���ļ��Ի���   
	CFileDialog fileDlg(FALSE, _T("bmp"), _T("saveShow"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString FilePath;

	// ��ʾ�����ļ��Ի���   
	if (IDOK == fileDlg.DoModal())
	{
		// ���������ļ��Ի����ϵġ����桱��ť����ѡ����ļ�·����ʾ���༭����   
		FilePath = fileDlg.GetPathName();

		CStringA stra(FilePath.GetBuffer(0));
		string strFilePath = stra.GetBuffer(0);
		imwrite(strFilePath, img);
	}
}

void CMaskDefectDetectingDlg::OnBnClickedButtonReadimg()
{
	//�ֶ���ͼƬ
	// IDB_SHOW1 ΪPicture�ؼ���ID��
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK)
	{
		CString FilePathName;
		FilePathName = dlg.GetPathName();
		//CString��string��ת��
		CStringA stra(FilePathName.GetBuffer(0));
		FilePathName.ReleaseBuffer();
		m_path = stra.GetBuffer(0);
		stra.ReleaseBuffer();
		openfiles = true;
	}
	if (!m_src.empty()) m_src.release();
	const char *charpath = m_path.c_str();
	m_src = imread(charpath, 0); //��ͼ

	////m_src = imread("D:\\Pictures\\����ͼ��\\pylon_2ed\\quemianjin\\10.bmp", 0); //������бͼ,0Ϊ��ͨ���Ҷ�ͼ��1Ϊԭͼ
	//m_src = imread("D:\\Pictures\\����ͼ��\\band01\\10.tiff", 0);
	////m_src = imread("D:\\Pictures\\����ͼ��\\gn_1st\\IMG_20141229_105945.jpg", 0); //���ֻ��ĵ�ͼ
	////m_src = imread("D:\\Pictures\\����ͼ��\\pylon_2ed\\toufa\\10.tiff", 0);//��ͷ��ͼ
	////m_src = imread("D:\\Pictures\\����ͼ��\\pylon_1st\\4ul3.bmp", 0);

	if (!m_src.empty())
	{
		DrawPicToHDC(m_src, IDB_SHOW1);
		b_correction = false;
	}
	else
	{
		CString str("��ʼ��δ����ͼ��������� OnBnClickedButtonReadimg");
		MessageBox(str);
	}
}


void   Delay(int   time)//time*1000Ϊ���� 
{
	clock_t   now = clock();
	while (clock() - now < time);
}

void CMaskDefectDetectingDlg::OnBnClickedButtonCorrection()//��m_src������һ���������m_nowImg
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_src.empty())
		return;
	//start_time = clock();//����ʼ��ʱ
	Mat src = m_src;
	imgProcess.GetMaskRect(src);//��ý����任����
	imgProcess.Normalization(src, imgProcess.m_maskrect, imgProcess.m_normalrows, imgProcess.m_normalcols, m_nowImg);
	DrawPicToHDC(m_nowImg, IDB_SHOW2, m_show2);//�������m_nowImg
	b_correction = true;
	//Mat L, R;
	//imgProcess.Cut2half(m_nowImg,L,R);
	//imgProcess.MirrorTrans(R);
	//imwrite("�з���.jpg",L);
	//imwrite("�з���.jpg", R);
	//imwrite("������һ��.jpg",m_nowImg);
	//end_time = (clock() - start_time) / 1000;//��ʱ
	//std::cout << "��ʱ " << end_time << " ��" << std::endl;
}

void CMaskDefectDetectingDlg::OnBnClickedButtonBandprocess()//��m_src���������m_nowImg,m_imgL��m_imgR
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int blockSize = 25;
	int constValue = 10;

	if (m_src.empty())
		return;
	if (b_correction == false)
	{
		MessageBox(L"δ������̬У����");
		return;
	}
	//OnBnClickedButtonCorrection();//�ó������任��maskrect��m_nowImg��ʱΪ������һ�����ͼ��


	////1.�̶���ֵ��
	//threshold(m_src, m_nowImg, 110, 255, CV_THRESH_BINARY);//CV_THRESH_BINARY  ,CV_THRESH_OTSU    ʵ�����
	//imgProcess.Normalization(m_nowImg, imgProcess.m_maskrect, imgProcess.m_normalrows, imgProcess.m_normalcols, m_nowImg);

	////2.�ֲ�����Ӧ��ֵ��
	////adaptiveThreshold(m_src, m_nowImg, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, blockSize, constValue);
	//////imwrite("adaptiveThreshold.jpg", m_nowImg);
	////imgProcess.Normalization(m_nowImg, imgProcess.m_maskrect, imgProcess.m_normalrows, imgProcess.m_normalcols, m_nowImg);

	//imgProcess.Normalization(m_src, imgProcess.m_maskrect, imgProcess.m_normalrows, imgProcess.m_normalcols, m_nowImg);
	//Mat gammaOutput, gammaInput;
	//m_nowImg.convertTo(gammaInput, CV_32FC1);
	//float gamma = 1 / 2.2;
	//pow(gammaInput, gamma, gammaOutput);
	//normalize(gammaOutput, m_nowImg, 0, 255, NORM_MINMAX, CV_8UC1);
	//DrawPicToHDC(m_nowImg, IDB_SHOW3,m_show3);
	//adaptiveThreshold(m_nowImg, m_nowImg, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, blockSize, constValue);
	////imwrite("normalization_adaptiveThreshold.jpg", m_nowImg);
	////m_info.AddString(L"�ֲ�����Ӧ��ֵ�������");
	////UpdateData(FALSE);

	//3.ƽ����ֵ��
	imgProcess.Normalization(m_src, imgProcess.m_maskrect, imgProcess.m_normalrows, imgProcess.m_normalcols, m_nowImg);
	Mat gammaOutput, gammaInput, dst;
	m_nowImg.convertTo(gammaInput, CV_32FC1);
	float gamma = 1 / 2.2;
	pow(gammaInput, gamma, gammaOutput); 
	normalize(gammaOutput, m_nowImg, 0, 255, NORM_MINMAX, CV_8UC1);
	DrawPicToHDC(m_nowImg, IDB_SHOW3,m_show3);
	Scalar cs = mean(m_nowImg);
	cv::threshold(m_nowImg, m_nowImg, cs.val[0], 255, CV_THRESH_BINARY);
	m_info.AddString(L"ƽ����ֵ���������");
	UpdateData(FALSE);

	//imshow("before resize", m_nowImg);
	//resize(m_nowImg, m_nowImg, Size(dstcols, dstrows));
	DrawPicToHDC(m_nowImg, IDB_SHOW4, m_show4);
	cv::resize(m_nowImg, m_nowImg, Size(dstcols, dstrows));
	//ͼ����У����ҹ�һ��
	imgProcess.Cut2half(m_nowImg, m_imgL, m_imgR);
}

void CMaskDefectDetectingDlg::Bandprocess()
{
	int blockSize = 25;
	int constValue = 10;

	if (m_src.empty())
		return;
	if (b_correction == false)
	{
		MessageBox(L"δ������̬У����");
		return;
	}
	//3.ƽ����ֵ��
	imgProcess.Normalization(m_src, imgProcess.m_maskrect, imgProcess.m_normalrows, imgProcess.m_normalcols, m_nowImg);
	Scalar cs = mean(m_nowImg);
	cv::threshold(m_nowImg, m_nowImg, cs.val[0], 255, CV_THRESH_BINARY);

	DrawPicToHDC(m_nowImg, IDB_SHOW4, m_show4);
	cv::resize(m_nowImg, m_nowImg, Size(dstcols, dstrows));
	//ͼ����У����ҹ�һ��
	imgProcess.Cut2half(m_nowImg, m_imgL, m_imgR);
}


void CMaskDefectDetectingDlg::OnBnClickedButtonHairprocess()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_src.empty())
		return;
	Mat src = m_src;
	OnBnClickedButtonCorrection();//���m_nowImg
	imgProcess.PreprocessHair(m_nowImg, m_nowImg);
	DrawPicToHDC(m_nowImg, IDB_SHOW4, m_show4);//�������pNowImage
	m_nowImg.release();
}

void CMaskDefectDetectingDlg::OnEnChangePathOpenBand()
{
	UpdateData();
	UpdateData(FALSE);
}




void CMaskDefectDetectingDlg::OnEnChangePathTrainBand()
{
	//�˴����뱣֤·���������ĸ��ļ��У���Ϊûд�жϺ������ļ��еĴ��롣
	UpdateData();//�ռ����Ϊ m_pathTrainBand
	m_pathTrainBand += "\\";
	m_pathSaveBandPos = m_pathTrainBand + _T("pos");
	m_pathSaveBandNeg = m_pathTrainBand + _T("neg");
	m_pathSaveBandTestSet = m_pathTrainBand + _T("TestSet");
	m_pathSaveBandPosGray = m_pathTrainBand + _T("posGray");;//���޽�ԭͼ��һ������·��
	m_pathSaveBandNegGray = m_pathTrainBand + _T("negGray");;//���޽�ԭͼ��һ������·��
	m_pathSaveBandTestSetGray = m_pathTrainBand + _T("TestSetGray");

	m_pathTrainBand = m_pathTrainBand.Left(m_pathTrainBand.GetLength() - 1);//ɾ�������ַ�"//"
	CStringA stra(m_pathTrainBand.GetBuffer(0));
	m_strPathTrainBand = stra.GetBuffer(0);
	stra.ReleaseBuffer();
	UpdateData(FALSE);
}



void CMaskDefectDetectingDlg::OnBnClickedButtonBatchBand()//��������
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CFileFind finder;
	CString fileName;//����ҵ������ļ��� ����ļ���·��

	UpdateData();
	BOOL haveNext = finder.FindFile(m_pathOpenBand + "\\*.*"); //�ж��Ƿ�ɹ��ҵ��ļ�,�޸�" "�����ݸ��޶������ļ�����

	//����ļ���·��
	if (haveNext)
	{
		if (!PathIsDirectory(m_pathOpenBand))
		{
			MessageBox(L"ͼƬԴĿ¼·������ȷ��");
			return;
		}
		if (!PathIsDirectory(m_pathSaveBandNeg) && !CreateDirectory(m_pathSaveBandNeg, NULL))
		{
			MessageBox(L"��������ļ��в���ȷ��");
			return;
		}
		if (!PathIsDirectory(m_pathSaveBandNegGray) && !CreateDirectory(m_pathSaveBandNegGray, NULL))
		{
			MessageBox(L"��������ļ��в���ȷ��");
			return;
		}
	}
	else
	{
		MessageBox(L"û�з���ͼƬ�ļ���");
		return;
	}

	//��ʼ��������
	while (haveNext)
	{
		haveNext = finder.FindNextFile();//��һ��ִ��FindNextFile��ѡ�񵽵�һ���ļ����Ժ�ִ��Ϊѡ��
		if (finder.IsDirectory())//�ҵ������ļ��У���������ļ����µ��ļ�
			continue;
		fileName = finder.GetFilePath();
		// �ڴ˴���Ӷ��ҵ��ļ��Ĵ���
		m_src = imread(ws2s(fileName.GetString()), 0);//��ȡ�Ҷ�ͼ
		if (!m_src.empty())
		{
			DrawPicToHDC(m_src, IDB_SHOW1);
			b_correction = false;

			//�Ҷ�ͼУ�����Ų��У�����Ҷ�ͼm_nowImg��m_imgLGray��m_imgRGray
			OnBnClickedButtonCorrection();//����Ҷ�ͼm_nowImg
			cv::resize(m_nowImg, m_nowImg, Size(dstcols, dstrows));
			imgProcess.Cut2half(m_nowImg, m_imgLGray, m_imgRGray);
			if (m_checkUseGamma == true)//��ѡGammaУ��
			{
				//	imgProcess.GammaProcess(m_nowImg, m_nowImg);
				Mat gammaOutput, gammaInput, dst;
				m_nowImg.convertTo(gammaInput, CV_32FC1);
				float gamma = 1 / 2.2;
				pow(gammaInput, gamma, gammaOutput);
				normalize(gammaOutput, dst, 0, 255, NORM_MINMAX, CV_8UC1);
				DrawPicToHDC(dst, IDB_SHOW3, m_show3);
			}

			//��һ��Ԥ�����ָ����Ҷ�ͼm_src�������ֵ��ǿͼm_nowImg��m_imgL��m_imgR
			Bandprocess();
			//����ͼƬ�����Ҷ����޽�
			saveImgWithNoBand(m_imgL, m_imgLGray);
			saveImgWithNoBand(m_imgR, m_imgRGray);	

		}
	}
	finder.Close();
	m_info.AddString(L"�������޴������");
	UpdateData(FALSE);
	return;
}

void CMaskDefectDetectingDlg::OnBnClickedButtonBatchBand2()//��������
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CFileFind finder;
	CString fileName;//����ҵ������ļ��� ����ļ���·��

	UpdateData();
	BOOL haveNext = finder.FindFile(m_pathOpenBand + "\\*.*"); //�ж��Ƿ�ɹ��ҵ��ļ�,�޸�" "�����ݸ��޶������ļ�����

	//����ļ���·��
	if (haveNext)
	{
		if (!PathIsDirectory(m_pathOpenBand))
		{
			MessageBox(L"ͼƬԴĿ¼·������ȷ��");
			return;
		}
		if (!PathIsDirectory(m_pathSaveBandNeg) && !CreateDirectory(m_pathSaveBandNeg, NULL))
		{
			MessageBox(L"��������ļ��в���ȷ��");
			return;
		}

		if (!PathIsDirectory(m_pathSaveBandPos) && !CreateDirectory(m_pathSaveBandPos, NULL))
		{
			MessageBox(L"��������ļ��в���ȷ��");
			return;
		}
		if (!PathIsDirectory(m_pathSaveBandNegGray) && !CreateDirectory(m_pathSaveBandNegGray, NULL))
		{
			MessageBox(L"��������ļ��в���ȷ��");
			return;
		}
		if (!PathIsDirectory(m_pathSaveBandPosGray) && !CreateDirectory(m_pathSaveBandPosGray, NULL))
		{
			MessageBox(L"��������ļ��в���ȷ��");
			return;
		}
	}
	else
	{
		MessageBox(L"û�з���ͼƬ�ļ���");
		return;
	}

	//��ʼ��������
	while (haveNext)
	{
		haveNext = finder.FindNextFile();//��һ��ִ��FindNextFile��ѡ�񵽵�һ���ļ����Ժ�ִ��Ϊѡ��
		if (finder.IsDirectory())//�ҵ������ļ��У���������ļ����µ��ļ�
			continue;
		fileName = finder.GetFilePath();
		// �ڴ˴���Ӷ��ҵ��ļ��Ĵ���
		m_src = imread(ws2s(fileName.GetString()), 0);//��ȡ�Ҷ�ͼ
		if (!m_src.empty())
		{
			DrawPicToHDC(m_src, IDB_SHOW1);
			b_correction = false;

			//ԭ�Ҷ�ͼУ�����Ų��У�������ֵ��������Ҷ�ͼm_nowImg��m_imgLGray��m_imgRGray
			OnBnClickedButtonCorrection();//����Ҷ�ͼm_nowImg
			cv::resize(m_nowImg, m_nowImg, Size(dstcols, dstrows));
			imgProcess.Cut2half(m_nowImg, m_imgLGray, m_imgRGray);
			if (m_checkUseGamma == true)//��ѡGammaУ��
			{
				//	imgProcess.GammaProcess(m_nowImg, m_nowImg);
				Mat gammaOutput, gammaInput, dst;
				m_nowImg.convertTo(gammaInput, CV_32FC1);
				float gamma = 1 / 2.2;
				pow(gammaInput, gamma, gammaOutput);
				normalize(gammaOutput, dst, 0, 255, NORM_MINMAX, CV_8UC1);
				DrawPicToHDC(dst, IDB_SHOW3, m_show3);
			}
			//��һ��Ԥ�����ָ����Ҷ�ͼm_src�������ֵ��ǿͼm_nowImgm_imgL��m_imgR
			Bandprocess();
			//����ͼƬ����������
			saveImgWithNoBand(m_imgL, m_imgLGray);
			saveImgWithBand(m_imgR, m_imgRGray);
		}

	}
	finder.Close();
	m_info.AddString(L"�������д������");
	UpdateData(FALSE);
	return;
}

void CMaskDefectDetectingDlg::OnBnClickedButtonBatchBand3()//��������
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CFileFind finder;
	CString fileName;//����ҵ������ļ��� ����ļ���·��

	UpdateData();
	BOOL haveNext = finder.FindFile(m_pathOpenBand + "\\*.*"); //�ж��Ƿ�ɹ��ҵ��ļ�,�޸�" "�����ݸ��޶������ļ�����

	//����ļ���·��
	if (haveNext)
	{
		if (!PathIsDirectory(m_pathOpenBand))
		{
			MessageBox(L"ͼƬԴĿ¼·������ȷ��");
			return;
		}
		if (!PathIsDirectory(m_pathSaveBandNeg) && !CreateDirectory(m_pathSaveBandNeg, NULL))
		{
			MessageBox(L"��������ļ��в���ȷ��");
			return;
		}
		if (!PathIsDirectory(m_pathSaveBandPos) && !CreateDirectory(m_pathSaveBandPos, NULL))
		{
			MessageBox(L"��������ļ��в���ȷ��");
			return;
		}
		if (!PathIsDirectory(m_pathSaveBandNegGray) && !CreateDirectory(m_pathSaveBandNegGray, NULL))
		{
			MessageBox(L"��������ļ��в���ȷ��");
			return;
		}
		if (!PathIsDirectory(m_pathSaveBandPosGray) && !CreateDirectory(m_pathSaveBandPosGray, NULL))
		{
			MessageBox(L"��������ļ��в���ȷ��");
			return;
		}
	}
	else
	{
		MessageBox(L"û�з���ͼƬ�ļ���");
		return;
	}

	//��ʼ��������
	while (haveNext)
	{
		haveNext = finder.FindNextFile();//��һ��ִ��FindNextFile��ѡ�񵽵�һ���ļ����Ժ�ִ��Ϊѡ��
		if (finder.IsDirectory())//�ҵ������ļ��У���������ļ����µ��ļ�
			continue;
		fileName = finder.GetFilePath();
		// �ڴ˴���Ӷ��ҵ��ļ��Ĵ���
		m_src = imread(ws2s(fileName.GetString()), 0);//��ȡ�Ҷ�ͼ
		if (!m_src.empty())
		{
			DrawPicToHDC(m_src, IDB_SHOW1);
			b_correction = false;

			//ԭ�Ҷ�ͼУ�����Ų��У�������ֵ��������Ҷ�ͼm_nowImg��m_imgLGray��m_imgRGray
			OnBnClickedButtonCorrection();//����Ҷ�ͼm_nowImg
			cv::resize(m_nowImg, m_nowImg, Size(dstcols, dstrows));
			imgProcess.Cut2half(m_nowImg, m_imgLGray, m_imgRGray);

			if (m_checkUseGamma == true)//��ѡGammaУ��
			{
				//	imgProcess.GammaProcess(m_nowImg, m_nowImg);
				Mat gammaOutput, gammaInput, dst;
				m_nowImg.convertTo(gammaInput, CV_32FC1);
				float gamma = 1 / 2.2;
				pow(gammaInput, gamma, gammaOutput);
				normalize(gammaOutput, dst, 0, 255, NORM_MINMAX, CV_8UC1);
				DrawPicToHDC(dst, IDB_SHOW3, m_show3);
			}
			//��һ��Ԥ�����ָ����Ҷ�ͼm_src�������ֵ��ǿͼm_nowImgm_imgL��m_imgR
			Bandprocess();
			//����ͼƬ����������
			saveImgWithBand(m_imgL, m_imgLGray);
			saveImgWithNoBand(m_imgR, m_imgRGray);
		}

	}
	finder.Close();
	m_info.AddString(L"�������޴������");
	UpdateData(FALSE);
	return;
}

void CMaskDefectDetectingDlg::OnBnClickedButtonBatchBand4()//��������
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CFileFind finder;
	CString fileName;//����ҵ������ļ��� ����ļ���·��

	UpdateData();
	BOOL haveNext = finder.FindFile(m_pathOpenBand + "\\*.*"); //�ж��Ƿ�ɹ��ҵ��ļ�,�޸�" "�����ݸ��޶������ļ�����

	//����ļ���·��
	if (haveNext)
	{
		if (!PathIsDirectory(m_pathOpenBand))
		{
			MessageBox(L"ͼƬԴĿ¼·������ȷ��");
			return;
		}
		if (!PathIsDirectory(m_pathSaveBandPos) && !CreateDirectory(m_pathSaveBandPos, NULL))
		{
			MessageBox(L"��������ļ��в���ȷ��");
			return;
		}
		if (!PathIsDirectory(m_pathSaveBandPosGray) && !CreateDirectory(m_pathSaveBandPosGray, NULL))
		{
			MessageBox(L"��������ļ��в���ȷ��");
			return;
		}
	}
	else
	{
		MessageBox(L"û�з���ͼƬ�ļ���");
		return;
	}
	//��ʼ��������
	while (haveNext)
	{
		haveNext = finder.FindNextFile();//��һ��ִ��FindNextFile��ѡ�񵽵�һ���ļ����Ժ�ִ��Ϊѡ��
		if (finder.IsDirectory())//�ҵ������ļ��У���������ļ����µ��ļ�
			continue;
		fileName = finder.GetFilePath();
		// �ڴ˴���Ӷ��ҵ��ļ��Ĵ���
		m_src = imread(ws2s(fileName.GetString()), 0);//��ȡ�Ҷ�ͼ
		if (!m_src.empty())
		{
			DrawPicToHDC(m_src, IDB_SHOW1);
			b_correction = false;

			//ԭ�Ҷ�ͼУ�����Ų��У�������ֵ��������Ҷ�ͼm_nowImg��m_imgLGray��m_imgRGray
			OnBnClickedButtonCorrection();//����Ҷ�ͼm_nowImg
			cv::resize(m_nowImg, m_nowImg, Size(dstcols, dstrows));
			imgProcess.Cut2half(m_nowImg, m_imgLGray, m_imgRGray);

			if (m_checkUseGamma == true)//��ѡGammaУ��
			{
				//	imgProcess.GammaProcess(m_nowImg, m_nowImg);
				Mat gammaOutput, gammaInput, dst;
				m_nowImg.convertTo(gammaInput, CV_32FC1);
				float gamma = 1 / 2.2;
				pow(gammaInput, gamma, gammaOutput);
				normalize(gammaOutput, dst, 0, 255, NORM_MINMAX, CV_8UC1);
				DrawPicToHDC(dst, IDB_SHOW3, m_show3);
			}
			//��һ��Ԥ�����ָ����Ҷ�ͼm_src�������ֵ��ǿͼm_nowImgm_imgL��m_imgR
			Bandprocess();
			//����ͼƬ����������
			saveImgWithBand(m_imgL, m_imgLGray);
			saveImgWithBand(m_imgR, m_imgRGray);
		}
	}
	finder.Close();
	m_info.AddString(L"�������д������");
	UpdateData(FALSE);
	return;
}

void CMaskDefectDetectingDlg::OnBnClickedButtonBatchBandtestset()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CFileFind finder;
	CString fileName;//����ҵ������ļ��� ����ļ���·��

	UpdateData();
	BOOL haveNext = finder.FindFile(m_pathOpenBand + "\\*.*"); //�ж��Ƿ�ɹ��ҵ��ļ�,�޸�" "�����ݸ��޶������ļ�����

	//����ļ���·��
	if (haveNext)
	{
		if (!PathIsDirectory(m_pathOpenBand))
		{
			MessageBox(L"ͼƬԴĿ¼·������ȷ��");
			return;
		}
		if (!PathIsDirectory(m_pathSaveBandTestSet) && !CreateDirectory(m_pathSaveBandTestSet, NULL))
		{
			MessageBox(L"��������ļ��в���ȷ��");
			return;
		}
		if (!PathIsDirectory(m_pathSaveBandTestSetGray) && !CreateDirectory(m_pathSaveBandTestSetGray, NULL))
		{
			MessageBox(L"��������ļ��в���ȷ��");
			return;
		}
	}
	else
	{
		MessageBox(L"û�з���ͼƬ�ļ���");
		return;
	}

	//��ʼ��������
	while (haveNext)
	{
		haveNext = finder.FindNextFile();//��һ��ִ��FindNextFile��ѡ�񵽵�һ���ļ����Ժ�ִ��Ϊѡ��
		if (finder.IsDirectory())//�ҵ������ļ��У���������ļ����µ��ļ�
			continue;
		fileName = finder.GetFilePath();
		// �ڴ˴���Ӷ��ҵ��ļ��Ĵ���
		m_src = imread(ws2s(fileName.GetString()), 0);//��ȡ�Ҷ�ͼ
		if (!m_src.empty())
		{

			DrawPicToHDC(m_src, IDB_SHOW1);
			b_correction = false;


			OnBnClickedButtonCorrection();//����Ҷ�ͼm_nowImg
			cv::resize(m_nowImg, m_nowImg, Size(dstcols, dstrows));
			imgProcess.Cut2half(m_nowImg, m_imgLGray, m_imgRGray);//ԭͼУ�����Ų��У�������ֵ��
			// ��һ��Ԥ�����ָ����Ҷ�ͼm_src�������ֵ��ǿͼm_nowImg, m_imgL��m_imgR
			Bandprocess();

			//����ͼƬ
			CString cstrNumTestSet;
			cstrNumTestSet.Format(_T("%d"), numTestSet);
			CString cstrSaveDir0 = m_pathSaveBandTestSet + "\\" + cstrNumTestSet + ".bmp";//Ԥ����󱣴�·��
			CString cstrSaveDir0Gray = m_pathSaveBandTestSetGray + "\\" + cstrNumTestSet + ".bmp";//ԭͼ����·��
			string strSaveDir0 = ws2s(cstrSaveDir0.GetString());
			string strSaveDirOrigin = ws2s(cstrSaveDir0Gray.GetString());
			imwrite(strSaveDir0, m_imgL);//����Ԥ���������
			imwrite(strSaveDirOrigin, m_imgLGray);//����ԭͼ���
			numTestSet++;


			cstrNumTestSet.Format(_T("%d"), numTestSet);
			cstrSaveDir0 = m_pathSaveBandTestSet + _T("\\") + cstrNumTestSet + _T(".bmp");
			cstrSaveDir0Gray = m_pathSaveBandTestSetGray + _T("\\") + cstrNumTestSet + _T(".bmp");//ԭͼ����·��
			strSaveDir0 = ws2s(cstrSaveDir0.GetString());
			strSaveDirOrigin = ws2s(cstrSaveDir0Gray.GetString());
			imwrite(strSaveDir0, m_imgR);//����Ԥ�������ұ�
			imwrite(strSaveDirOrigin, m_imgRGray);//����ԭͼ�ұ�
			numTestSet++;
		}
	}
	finder.Close();
	m_info.AddString(L"�޽�����Լ�Ԥ�������");
	UpdateData(FALSE);
	return;
}


void CMaskDefectDetectingDlg::OnBnClickedButtonReadimg2()
{
	//�ֶ���ͼƬ
	// IDB_SHOW1 ΪPicture�ؼ���ID��
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK)
	{
		CString FilePathName;
		FilePathName = dlg.GetPathName();
		//CString��string��ת��
		CStringA stra(FilePathName.GetBuffer(0));
		FilePathName.ReleaseBuffer();
		m_path = stra.GetBuffer(0);
		stra.ReleaseBuffer();
		openfiles = true;
	}
	if (!m_src.empty()) m_src.release();
	const char *charpath = m_path.c_str();
	m_src = imread(charpath, 0); //��ͼ
	//m_src = imread("D:\\Pictures\\����ͼ��\\pylon_2ed\\quemianjin\\10.bmp", 0); //������бͼ,0Ϊ��ͨ���Ҷ�ͼ��1Ϊԭͼ
	//m_src = imread("D:\\Pictures\\����ͼ��\\gn_1st\\IMG_20141229_105945.jpg", 0); //���ֻ��ĵ�ͼ
	//m_src = imread("D:\\Pictures\\����ͼ��\\pylon_2ed\\toufa\\10.tiff", 0);//��ͷ��ͼ
	//m_src = imread("D:\\Pictures\\����ͼ��\\pylon_1st\\4ul3.bmp", 0);

	if (!m_src.empty())
	{
		DrawPicToHDC(m_src, IDB_SHOW1);
		b_correction = false;
	}
}

int CMaskDefectDetectingDlg::SVMpredict(Mat src, CvSVM &SVM)//����ͼ��SVM�жϷ���ֵ1Ϊ����-1Ϊ��
{
	LBP lbp;
	int lengthOfFeatureVector = (TrainImgWidth / CELLSIZE_LBP)*(TrainImgHeight / CELLSIZE_LBP) * 58; // ����������ά��
	// ʶ��
	vector<float> featureVectorOfTestImage;
	Mat _featureVectorOfTestImage(1, lengthOfFeatureVector, CV_32FC1);
	char line[512];

	//ofstream resultOfPrediction(m_strPathTrainBand + "\\PredictResult.txt", ios::out);
	resize(src, src, cv::Size(TrainImgWidth, TrainImgHeight), 0, 0, INTER_CUBIC);//��������
	// ����LBP��������
	lbp.ComputerLBPFeatureVector(src, Size(CELLSIZE_LBP, CELLSIZE_LBP), featureVectorOfTestImage);
	for (vector<float>::size_type j = 0; j <= featureVectorOfTestImage.size() - 1; ++j)
	{
		_featureVectorOfTestImage.at<float>(0, j) = featureVectorOfTestImage[j];
	}

	int predict = SVM.predict(_featureVectorOfTestImage);

	return predict;

	////����ͼ����ת����ȼ�ģʽ����
	//int lengthOfFeatureVector = (TrainImgWidth / CELLSIZE_LBP)*(TrainImgHeight / CELLSIZE_LBP) * 9; // ����������ά��������ֻ��9��
	//LBP lbp;
	//vector<float> featureVectorOfTestImage;
	//Mat _featureVectorOfTestImage(1, lengthOfFeatureVector, CV_32FC1);
	//char line[512];
	//	resize(src, src, cv::Size(TrainImgWidth, TrainImgHeight), 0, 0, INTER_CUBIC);//��������
	//	// ��������LBP��������
	//	lbp.ComputerLBPFeatureVector_Rotation(src, Size(CELLSIZE_LBP, CELLSIZE_LBP), featureVectorOfTestImage);
	//	for (vector<float>::size_type j = 0; j <= lengthOfFeatureVector - 1; ++j)
	//	{
	//		_featureVectorOfTestImage.at<float>(0, j) = featureVectorOfTestImage[j];
	//	}
	//	imshow("_featureVectorOfTestImage",_featureVectorOfTestImage);
	//	waitKey(0);
	//	FileStorage fs(".\\featureVectorOfTestImage.xml", FileStorage::WRITE);
	//	fs << "featureVectorOfTestImage" << _featureVectorOfTestImage;
	//	fs.release();
	//return 1;

}

int CMaskDefectDetectingDlg::binaryHogPredict(Mat src, CvSVM &SVM)//����ͼ��SVM�жϷ���ֵ1Ϊ����-1Ϊ��
{
	int predict = useHog.PredictSingleImage(src, TrainImgWidth, TrainImgWidth, SVM);
	return predict;	
}

void CMaskDefectDetectingDlg::OnBnClickedButtonJudge()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_src.empty())
		return;
	OnBnClickedButtonCorrection();

	//��һ��٤��У�����ò�����ֻ��Ϊ�����н������ۣ�
	Mat gammaOutput, gammaInput, dst;
	m_nowImg.convertTo(gammaInput, CV_32FC1);
	float gamma = 1 / 2.2;
	pow(gammaInput, gamma, gammaOutput);
	normalize(gammaOutput, gammaOutput, 0, 255, NORM_MINMAX, CV_8UC1);
	DrawPicToHDC(gammaOutput, IDB_SHOW3, m_show3);
	//٤��У���������

	Bandprocess();

	if (m_pathTrainBand.IsEmpty())
	{
		MessageBox(L"��ѡ��ѵ����Ŀ¼");
		return;
	}
	CvSVM SVM;
	//SVM.load((m_strPathTrainBand + "\\ClassifierBinaryLBP.xml").data());//����ѵ���õ�SVM������
	//int predictBandL = SVMpredict(m_imgL, SVM);
	//int predictBandR = SVMpredict(m_imgR, SVM);
	SVM.load((m_strPathTrainBand + "\\HOGClassifierBinary.xml").data());//����ѵ���õ�SVM������

	int predictBandL = binaryHogPredict(m_imgL, SVM);
	int predictBandR = binaryHogPredict(m_imgR, SVM);

	if (predictBandL == 1 && predictBandR == 1)
		m_info.AddString(L"���ִ���ȫ");
	if (predictBandL == -1 && predictBandR == 1)
		m_info.AddString(L"����ִ�ȱʧ");
	if (predictBandL == 1 && predictBandR == -1)
		m_info.AddString(L"�ҿ��ִ�ȱʧ");
	if (predictBandL == -1 && predictBandR == -1)
		m_info.AddString(L"���ҿ��ִ���ȱʧ");

	UpdateData(FALSE);
}

int CMaskDefectDetectingDlg::GetDir(CString path, CString txtPath, int cat)
{
	CFileFind finder;
	CString fileName;//�ļ�����·����ȫ��
	ofstream output(ws2s(txtPath.GetString()), ios::app);//�����ļ�
	BOOL haveNext = finder.FindFile(path + "\\*.bmp"); //�ж��Ƿ�ɹ��ҵ��ļ�," *.*"Ϊ�����ļ�

	//��ʼ��������
	if (!haveNext)
	{
		m_info.AddString(L"��bmpͼ���ļ�");
		return 0;
	}
	while (haveNext)
	{
		haveNext = finder.FindNextFile();//��һ��ִ��FindNextFile��ѡ�񵽵�һ���ļ����Ժ�ִ��Ϊѡ��
		//if (finder.IsDirectory())//�ҵ������ļ��У���������ļ����µ��ļ�
		//	continue;
		//fileName = finder.GetFilePath();
		// �ڴ˴���Ӷ��ҵ��ļ��Ĵ���
		CString imgDir = finder.GetFilePath();
		string str = CStringA(imgDir);
		output << str.c_str() << "\n";
		output << cat << "\n";
	}
	finder.Close();
	//m_info.AddString(L"ѵ��Ŀ¼�������");
	UpdateData(FALSE);
	return 1;
}

int CMaskDefectDetectingDlg::GetDir(CString path, CString txtPath)
{
	CFileFind finder;
	CString fileName;//�ļ�����·����ȫ��
	ofstream output(ws2s(txtPath.GetString()), ios::app);//�����ļ�
	BOOL haveNext = finder.FindFile(path + "\\*.bmp"); //�ж��Ƿ�ɹ��ҵ��ļ�," *.*"Ϊ�����ļ�

	//��ʼ��������
	if (!haveNext)
	{
		m_info.AddString(L"��bmpͼ���ļ�");
		return 0;
	}
	while (haveNext)
	{
		haveNext = finder.FindNextFile();//��һ��ִ��FindNextFile��ѡ�񵽵�һ���ļ����Ժ�ִ��Ϊѡ��
		//if (finder.IsDirectory())//�ҵ������ļ��У���������ļ����µ��ļ�
		//	continue;
		//fileName = finder.GetFilePath();
		// �ڴ˴���Ӷ��ҵ��ļ��Ĵ���
		CString imgDir = finder.GetFilePath();
		string str = CStringA(imgDir);
		output << str.c_str() << "\n";
	}

	finder.Close();
	//m_info.AddString(L"Ŀ¼�������");
	UpdateData(FALSE);
	return 1;
}

int CMaskDefectDetectingDlg::DeleteLastLine(string path)
{
	vector<string> tmp_files;
	ifstream winfile(path);
	if (!winfile)
	{
		cout << "fail" << endl;
		return 0;
	}
	string lineContent;
	while (getline(winfile, lineContent, '\n'))
	{
		tmp_files.push_back(lineContent + "\n");
	}

	winfile.close();

	tmp_files.back().erase(tmp_files.back().end() - 1);//ɾ�����һ��\n

	ofstream offile(path, ios::out);
	vector<string>::iterator siter = tmp_files.begin();

	copy(tmp_files.begin(), tmp_files.end(), ostream_iterator<string>(offile));

	//cout << "ok!" << endl;

	offile.close();
	return 1;
}

void CMaskDefectDetectingDlg::OnBnClickedButtonSvm()//�˴���txtPath�Ĵ������̴�����һ��
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	// ����ѵ������·�������
	UpdateData();
	clock_t   startTime, endTime;
	startTime = clock();//��ʼʱ��
	vector<string> imagePaths;
	vector<int> imageClass;
	string buffer;
	int numberOfLine = 0;
	if (m_pathTrainBand.IsEmpty())
	{
		MessageBox(L"��ѡ��ѵ����Ŀ¼");
		return;
	}

	string TrainSetTxtPath;
	CString txtPath;
	if (m_checkUseGrayimg == 0)//ʹ�ö�ֵ��ǿͼ
	{
		 txtPath = m_pathTrainBand +_T("\\TrainSetBinary.txt");
	}
	else//ʹ�ûҶ�ͼ
	{
		txtPath = m_pathTrainBand + _T("\\TrainSetGray.txt");
	}

	TrainSetTxtPath = ws2s(txtPath.GetString());//·���ַ�������ת��
	ifstream file(TrainSetTxtPath, ios::in);
	
	if (file.peek() == EOF)
	{
		m_info.AddString(L"Ŀ¼Ϊ�գ�������������Ŀ¼");
		UpdateData(false);
		return;
	}
	while (!file.eof())
	{
		if (getline(file, buffer))
		{
			++numberOfLine;
			if (numberOfLine % 2 == 0)//�����������
			{
				imageClass.push_back(atoi(buffer.c_str()));
			}
			else
			{
				imagePaths.push_back(buffer);
			}
		}
	}

	file.close();

	// ��������LBP�������������������
	int lengthOfFeatureVector = (TrainImgWidth / CELLSIZE_LBP)*(TrainImgHeight / CELLSIZE_LBP) * 58; // ����������ά��()
	Mat featureVectorOfSample(imagePaths.size(), lengthOfFeatureVector, CV_32FC1); // ������������������
	Mat classOfSample(imagePaths.size(), 1, CV_32SC1);
	vector<string>::size_type numberOfSample = imagePaths.size();
	Mat srcImage;
	LBP lbp;
	vector<float> featureVector;
	for (vector<string>::size_type i = 0; i <= numberOfSample - 1; ++i)
	{
		// ����ͼƬifstream
		srcImage = imread(imagePaths[i], 0);
		if (m_checkUseGrayimg == true) // ���ʹ�ûҶ�ͼ
		{
			if (m_checkUseGamma == true)//�Ƿ�ԻҶ�ͼ��GammaУ��
			{
				//	imgProcess.GammaProcess(srcImage, srcImage);
				Mat gammaOutput, gammaInput;
				srcImage.convertTo(gammaInput, CV_32FC1);
				float gamma = 1 / 2.2;
				pow(gammaInput, gamma, gammaOutput);
				normalize(gammaOutput, srcImage, 0, 255, NORM_MINMAX, CV_8UC1);
			}
		}
		resize(srcImage, srcImage, cv::Size(TrainImgWidth, TrainImgHeight), 0, 0, INTER_CUBIC);//��������
		// ��������LBP��������
		lbp.ComputerLBPFeatureVector(srcImage, Size(CELLSIZE_LBP, CELLSIZE_LBP), featureVector);
		for (vector<float>::size_type j = 0; j <= lengthOfFeatureVector - 1; ++j)
		{
			featureVectorOfSample.at<float>(i, j) = featureVector[j];
		}
		classOfSample.at<int>(i, 0) = imageClass[i];
	}

	// ʹ��SVM������ѵ��
	// ��������
	CvSVMParams params;
	params.svm_type = CvSVM::C_SVC;
	params.kernel_type = CvSVM::LINEAR;
	params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);
	/*Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));*/

	//ѵ��SVM
	CvSVM SVM;
	SVM.train(featureVectorOfSample, classOfSample, Mat(), Mat(), params);
	if (m_checkUseGrayimg == 0)//ʹ�ö�ֵ��ǿͼ
	{
		SVM.save((m_strPathTrainBand + "\\ClassifierBinaryLBP.xml").data());
		m_info.AddString(L"��ֵ��ǿLBP����ѵ�����");
	}
	else
	{
		SVM.save((m_strPathTrainBand + "\\ClassifierGrayLBP.xml").data());
		m_info.AddString(L"�Ҷ�ͼLBP����ѵ�����");
	}
	//svm->train(featureVectorOfSample, ROW_SAMPLE, classOfSample);
	//svm->save(string(PATH) + "Classifier.xml");
	endTime = clock();//����ʱ��
	double runTime = (double)(endTime - startTime)/1000;
	//wstringstream ss;
	//wstring str;
	//ss << end_time;
	//ss >> str;	
	//str = "ѵ����ʱ��" + str;
	//m_info.AddString(str.c_str());
	UpdateData(FALSE);

}

void CMaskDefectDetectingDlg::OnBnClickedButtonSvmpredict()//
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	// ʹ��ѵ���õķ���������ʶ��
	UpdateData();
	clock_t   startTime, endTime;
	startTime = clock();//��ʼʱ��

	if (m_pathTrainBand.IsEmpty())
	{
		MessageBox(L"��ѡ��ѵ����Ŀ¼");
		return;
	}
	string TestSetTxtPath;
	if (m_checkUseGrayimg == 0)//ʹ�ö�ֵ��ǿͼ
	{
		CString txtPath = m_pathTrainBand + "\\TestSetBinary.txt";
		TestSetTxtPath = ws2s(txtPath.GetString());//·���ַ�������ת��
	}
	else//ʹ�ûҶ�ͼ
	{
		CString txtPath = m_pathTrainBand + "\\TestSetGray.txt";
		TestSetTxtPath = ws2s(txtPath.GetString());//·���ַ�������ת��
	}


	vector<string> testImagePath;
	string buffer;
	int lengthOfFeatureVector = (TrainImgWidth / CELLSIZE_LBP)*(TrainImgHeight / CELLSIZE_LBP) * 58; // ����������ά��
	LBP lbp;
	CvSVM SVM;

	if (m_checkUseGrayimg == 0)//ʹ�ö�ֵ��ǿͼ
	{
		SVM.load((m_strPathTrainBand + "\\ClassifierBinaryLBP.xml").data());//����ѵ���õ�SVM������
	}
	else
	{
		SVM.load((m_strPathTrainBand + "\\ClassifierGrayLBP.xml").data());//����ѵ���õ�SVM������
	}

	ifstream testFile(TestSetTxtPath, ios::in);// ע��Ҫȥ�����һ�еĻ��У��������һ��ͼƬ���������ǿյ�
	if (!testFile)
	{
		m_info.AddString(L"���ȶ�ѵ������Ԥ����");
		UpdateData(FALSE);
		return;
	}
	while (!testFile.eof())	{
		getline(testFile, buffer);
		testImagePath.push_back(buffer);
	}
	// ʶ��
	Mat testImage;
	vector<string>::size_type numberOfTestImage = testImagePath.size();
	vector<float> featureVectorOfTestImage;
	Mat _featureVectorOfTestImage(1, lengthOfFeatureVector, CV_32FC1);
	char line[512];

	string PredictTxtPath;
	if (m_checkUseGrayimg == 0)//ʹ�ö�ֵ��ǿͼ
	{
		CString txtPath = m_pathTrainBand + "\\PredictBinaryLBP.txt";
		PredictTxtPath = ws2s(txtPath.GetString());
	}
	else
	{
		CString txtPath = m_pathTrainBand + "\\PredictGrayLBP.txt";
		PredictTxtPath = ws2s(txtPath.GetString());
	}
	ofstream resultOfPrediction(PredictTxtPath, ios::out);

	// ע�⽫ѭ�����ڵĺ�ʱ�����Ͳ�����ȡ��ѭ������
	for (vector<string>::size_type i = 0; i <= numberOfTestImage - 1; ++i)
	{
		testImage = imread(testImagePath[i], 0);
		if (m_checkUseGrayimg == true) // ���ʹ�ûҶ�ͼ
		{
			if (m_checkUseGamma == true)//�Ƿ�ԻҶ�ͼ��GammaУ��
			{
				//	imgProcess.GammaProcess(srcImage, srcImage);
				Mat gammaOutput, gammaInput;
				testImage.convertTo(gammaInput, CV_32FC1);
				float gamma = 1 / 2.2;
				pow(gammaInput, gamma, gammaOutput);
				normalize(gammaOutput, testImage, 0, 255, NORM_MINMAX, CV_8UC1);
			}
		}
		resize(testImage, testImage, cv::Size(TrainImgWidth, TrainImgHeight), 0, 0, INTER_CUBIC);//��������
		// ����LBP��������
		lbp.ComputerLBPFeatureVector(testImage, Size(CELLSIZE_LBP, CELLSIZE_LBP), featureVectorOfTestImage);
		for (vector<float>::size_type j = 0; j <= featureVectorOfTestImage.size() - 1; ++j)
		{
			_featureVectorOfTestImage.at<float>(0, j) = featureVectorOfTestImage[j];
		}

		int predict = SVM.predict(_featureVectorOfTestImage);
		sprintf(line, "%s %d\n", testImagePath[i].c_str(), predict);
		resultOfPrediction << line;
	}
	resultOfPrediction.close();

	m_info.AddString(L"�ȼ�LBPʶ�����");
	
	endTime = clock();//����ʱ��
	double runTime = (double)(endTime - startTime) / 1000;
	UpdateData(FALSE);
}

void CMaskDefectDetectingDlg::OnBnClickedButtonSvmLbpRotation()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	// ����ѵ������·�������
	UpdateData();
	clock_t   startTime, endTime;
	startTime = clock();//��ʼʱ��

	vector<string> imagePaths;
	vector<int> imageClass;
	string buffer;
	int numberOfLine = 0;
	if (m_pathTrainBand.IsEmpty())
	{
		MessageBox(L"��ѡ��ѵ����Ŀ¼");
		return;
	}
	string TrainSetTxtPath;
	if (m_checkUseGrayimg == 0)//ʹ�ö�ֵ��ǿͼ
	{
		CString txtPath = m_pathTrainBand + "\\TrainSetBinary.txt";
		TrainSetTxtPath = ws2s(txtPath.GetString());//·���ַ�������ת��
	}
	else//ʹ�ûҶ�ͼ
	{
		CString txtPath = m_pathTrainBand + "\\TrainSetGray.txt";
		TrainSetTxtPath = ws2s(txtPath.GetString());
	}

	ifstream file(TrainSetTxtPath, ios::in);

	if (file.peek() == EOF)
	{
		m_info.AddString(L"Ŀ¼Ϊ�գ�������������Ŀ¼");
		UpdateData(false);
		return;
	}
	while (!file.eof())
	{
		if (getline(file, buffer))
		{
			++numberOfLine;
			if (numberOfLine % 2 == 0)//�����������
			{
				imageClass.push_back(atoi(buffer.c_str()));
			}
			else
			{
				imagePaths.push_back(buffer);
			}
		}
	}

	file.close();

	// ��������LBP�������������������
	int lengthOfFeatureVector = (TrainImgWidth / CELLSIZE_LBP)*(TrainImgHeight / CELLSIZE_LBP) * 9; // ����������ά��������ֻ��9��
	Mat featureVectorOfSample(imagePaths.size(), lengthOfFeatureVector, CV_32FC1); // ������������������
	Mat classOfSample(imagePaths.size(), 1, CV_32SC1);
	vector<string>::size_type numberOfSample = imagePaths.size();
	Mat srcImage;
	LBP lbp;
	vector<float> featureVector;
	for (vector<string>::size_type i = 0; i <= numberOfSample - 1; ++i)
	{
		// ����ͼƬ
		srcImage = imread(imagePaths[i], 0);
		if (m_checkUseGrayimg == true) // ���ʹ�ûҶ�ͼ
		{
			if (m_checkUseGamma == true)//�Ƿ�ԻҶ�ͼ��GammaУ��
			{
				//	imgProcess.GammaProcess(srcImage, srcImage);
				Mat gammaOutput, gammaInput;
				srcImage.convertTo(gammaInput, CV_32FC1);
				float gamma = 1 / 2.2;
				pow(gammaInput, gamma, gammaOutput);
				normalize(gammaOutput, srcImage, 0, 255, NORM_MINMAX, CV_8UC1);
			}
		}
		resize(srcImage, srcImage, cv::Size(TrainImgWidth, TrainImgHeight), 0, 0, INTER_CUBIC);//��������
		// ��������LBP��������
		lbp.ComputerLBPFeatureVector_Rotation(srcImage, Size(CELLSIZE_LBP, CELLSIZE_LBP), featureVector);
		for (vector<float>::size_type j = 0; j <= lengthOfFeatureVector - 1; ++j)
		{
			featureVectorOfSample.at<float>(i, j) = featureVector[j];
		} 
		classOfSample.at<int>(i, 0) = imageClass[i];
	}

	// ʹ��SVM������ѵ��
	// ��������
	CvSVMParams params;
	params.svm_type = CvSVM::C_SVC;
	params.kernel_type = CvSVM::LINEAR;
	params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);
	/*Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));*/

	//ѵ��SVM
	CvSVM SVM;
	SVM.train(featureVectorOfSample, classOfSample, Mat(), Mat(), params);
	if (m_checkUseGrayimg == 0)//ʹ�ö�ֵ��ǿͼ
	{
		SVM.save((m_strPathTrainBand + "\\ClassifierBinaryRotLBP.xml").data());
		m_info.AddString(L"��ֵ��ǿ��ת����LBP����ѵ�����");
	}
	else
	{
		SVM.save((m_strPathTrainBand + "\\ClassifierGrayRotLBP.xml").data());
		m_info.AddString(L"�Ҷ�ͼ��ת����LBP����ѵ�����");
	}
	//svm->train(featureVectorOfSample, ROW_SAMPLE, classOfSample);
	//svm->save(string(PATH) + "Classifier.xml");
	endTime = clock();//����ʱ��
	double runTime = (double)(endTime - startTime) / 1000;
	UpdateData(FALSE);
}


void CMaskDefectDetectingDlg::OnBnClickedButtonSvmLbpRotationPredict()
{
	// ʹ��ѵ���õķ���������ʶ��
	UpdateData();
	clock_t   startTime, endTime;
	startTime = clock();//��ʼʱ��

	if (m_pathTrainBand.IsEmpty())
	{
		MessageBox(L"��ѡ��ѵ����Ŀ¼");
		return;
	}
	string TestSetTxtPath;
	if (m_checkUseGrayimg == 0)//ʹ�ö�ֵ��ǿͼ
	{
		CString txtPath = m_pathTrainBand + "\\TestSetBinary.txt";
		TestSetTxtPath = ws2s(txtPath.GetString());//·���ַ�������ת��
	}
	else//ʹ�ûҶ�ͼ
	{
		CString txtPath = m_pathTrainBand + "\\TestSetGray.txt";
		TestSetTxtPath = ws2s(txtPath.GetString());//·���ַ�������ת��
	}


	vector<string> testImagePath;
	string buffer;
	int lengthOfFeatureVector = (TrainImgWidth / CELLSIZE_LBP)*(TrainImgHeight / CELLSIZE_LBP) * 9; // ����������ά��,����ֻ��9��
	LBP lbp;
	CvSVM SVM;
	//����ѵ���õ�SVM������
	if (m_checkUseGrayimg == 0)//ʹ�ö�ֵ��ǿͼ
	{
		SVM.load((m_strPathTrainBand + "\\ClassifierBinaryRotLBP.xml").data());
	}
	else
	{
		SVM.load((m_strPathTrainBand + "\\ClassifierGrayRotLBP.xml").data());
	}

	ifstream testFile(TestSetTxtPath, ios::in);// ע��Ҫȥ�����һ�еĻ��У��������һ��ͼƬ���������ǿյ�
	if (!testFile)
	{
		m_info.AddString(L"���ȶ�ѵ������Ԥ����");
		UpdateData(FALSE);
		return;
	}
	while (!testFile.eof())
	{
		getline(testFile, buffer);
		testImagePath.push_back(buffer);
	}
	// ʶ��
	Mat testImage;
	vector<string>::size_type numberOfTestImage = testImagePath.size();
	vector<float> featureVectorOfTestImage;
	Mat _featureVectorOfTestImage(1, lengthOfFeatureVector, CV_32FC1);
	char line[512];
//�������·��
	string PredictTxtPath;
	if (m_checkUseGrayimg == 0)//ʹ�ö�ֵ��ǿͼ
	{
		CString txtPath = m_pathTrainBand + "\\PredictBinaryRotLBP.txt";
		PredictTxtPath = ws2s(txtPath.GetString());
	}
	else
	{
		CString txtPath = m_pathTrainBand + "\\PredictGrayRotLBP.txt";
		PredictTxtPath = ws2s(txtPath.GetString());
	}
	ofstream resultOfPrediction(PredictTxtPath, ios::out);


	// ע�⽫ѭ�����ڵĺ�ʱ�����Ͳ�����ȡ��ѭ������
	for (vector<string>::size_type i = 0; i <= numberOfTestImage - 1; ++i)
	{
		testImage = imread(testImagePath[i], 0);
		if (m_checkUseGrayimg == true) // ���ʹ�ûҶ�ͼ
		{
			if (m_checkUseGamma == true)//�Ƿ�ԻҶ�ͼ��GammaУ��
			{
				//	imgProcess.GammaProcess(srcImage, srcImage);
				Mat gammaOutput, gammaInput;
				testImage.convertTo(gammaInput, CV_32FC1);
				float gamma = 1 / 2.2;
				pow(gammaInput, gamma, gammaOutput);
				normalize(gammaOutput, testImage, 0, 255, NORM_MINMAX, CV_8UC1);
			}
		}
		resize(testImage, testImage, cv::Size(TrainImgWidth, TrainImgHeight), 0, 0, INTER_CUBIC);//��������
		// ����LBP��������
		lbp.ComputerLBPFeatureVector_Rotation(testImage, Size(CELLSIZE_LBP, CELLSIZE_LBP), featureVectorOfTestImage);
		for (vector<float>::size_type j = 0; j <= featureVectorOfTestImage.size() - 1; ++j)
		{
			_featureVectorOfTestImage.at<float>(0, j) = featureVectorOfTestImage[j];
		}

		int predict = SVM.predict(_featureVectorOfTestImage);
		sprintf(line, "%s %d\n", testImagePath[i].c_str(), predict);
		resultOfPrediction << line;
	}
	resultOfPrediction.close();
	m_info.AddString(L"��ת����ȼ�LBPʶ�����");

	endTime = clock();//����ʱ��
	double runTime = (double)(endTime - startTime) / 1000;
	UpdateData(FALSE);
}


void CMaskDefectDetectingDlg::OnBnClickedButtonTrainHogSvm()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	clock_t   startTime, endTime;
	startTime = clock();//��ʼʱ��

	if (m_pathTrainBand.IsEmpty())
	{
		MessageBox(L"��ѡ��ѵ����Ŀ¼");
		return;
	}
	int imgWidth = TrainImgWidth;//��ʼֵΪ64
	int imgHight = TrainImgHeight;
	string TrainSetTxtPath;
	string SVMSavePath;
	if (m_checkUseGrayimg == 0)//ʹ�ö�ֵ��ǿͼ
	{
		CString txtPath = m_pathTrainBand + "\\TrainSetBinary.txt";
		CString svmPath = m_pathTrainBand + "\\HOGClassifierBinary.xml";
		TrainSetTxtPath = ws2s(txtPath.GetString());//·���ַ�������ת��
		SVMSavePath = ws2s(svmPath.GetString());
	}
	else//ʹ�ûҶ�ͼ
	{
		CString txtPath = m_pathTrainBand + "\\TrainSetGray.txt";
		CString svmPath = m_pathTrainBand + "\\HOGClassifierGray.xml";
		TrainSetTxtPath = ws2s(txtPath.GetString());
		SVMSavePath = ws2s(svmPath.GetString());
	}
	ifstream file(TrainSetTxtPath, ios::in);
	if (file.peek() == EOF)
	{
		m_info.AddString(L"Ŀ¼Ϊ�գ�������������Ŀ¼");
		UpdateData(false);
		return;
	}
	useHog.TrainHOGwithSVM(TrainSetTxtPath, imgWidth, imgHight, SVMSavePath);
	if (m_checkUseGrayimg == 0)//ʹ�ö�ֵ��ǿͼ
	{
		m_info.AddString(L"��ֵ��ǿ����HOG����ѵ�����");
	}
	else//ʹ�ûҶ�ͼ
	{
		m_info.AddString(L"�Ҷ�����HOG����ѵ�����");
	}

	endTime = clock();//����ʱ��
	double runTime = (double)(endTime - startTime) / 1000;
	UpdateData(FALSE);
}


void CMaskDefectDetectingDlg::OnBnClickedButtonPredictHogSvm()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	clock_t   startTime, endTime;
	startTime = clock();//��ʼʱ��

	if (m_pathTrainBand.IsEmpty())
	{
		MessageBox(L"��ѡ��ѵ����Ŀ¼");
		return;
	}
	int imgWidth = TrainImgWidth;//��ʼֵΪ64
	int imgHight = TrainImgHeight;

	string TestSetTxtPath;
	string SVMLoadPath;
	string PredictSavePath;
	if (m_checkUseGrayimg == 0)//ʹ�ö�ֵ��ǿͼ
	{
		CString txtPath = m_pathTrainBand + "\\TestSetBinary.txt";
		CString svmPath = m_pathTrainBand + "\\HOGClassifierBinary.xml";
		CString predictPath = m_pathTrainBand + "\\HOGPredictBinary.txt";
		TestSetTxtPath = ws2s(txtPath.GetString());//·���ַ�������ת��
		SVMLoadPath = ws2s(svmPath.GetString());
		PredictSavePath = ws2s(predictPath.GetString());
		m_info.AddString(L"��ֵͼHOG�����б����");
	}
	else//ʹ�ûҶ�ͼ
	{
		CString txtPath = m_pathTrainBand + "\\TestSetGray.txt";
		CString svmPath = m_pathTrainBand + "\\HOGClassifierGray.xml";
		CString predictPath = m_pathTrainBand + "\\HOGPredictGray.txt";
		TestSetTxtPath = ws2s(txtPath.GetString());
		SVMLoadPath = ws2s(svmPath.GetString());
		PredictSavePath = ws2s(predictPath.GetString());
		m_info.AddString(L"�Ҷ�ͼHOG�����б����");
	}

	useHog.Predict(TestSetTxtPath, SVMLoadPath, imgWidth, imgHight, PredictSavePath);

	endTime = clock();//����ʱ��
	double runTime = (double)(endTime - startTime) / 1000;
	UpdateData(FALSE);
}


void CMaskDefectDetectingDlg::OnBnClickedButtonPrehog()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_src.empty())
		return;
	OnBnClickedButtonCorrection();//�ó������任��maskrect��m_nowImg��ʱΪ������һ�����ͼ��
	//GammaУ��
	Mat gammaOutput, gammaInput, dst;
	m_nowImg.convertTo(gammaInput, CV_32FC1);
	float gamma = 1 / 2.2;
	pow(gammaInput, gamma, gammaOutput);
	normalize(gammaOutput, dst, 0, 255, NORM_MINMAX, CV_8UC1);
	//GammaУ�����
	DrawPicToHDC(dst, IDB_SHOW3, m_show3);
	//imwrite("Gamma.jpg", dst);
	m_info.AddString(L"GammaУ�����");
	UpdateData(FALSE);

	Mat gradImage, gradOrient;
	useHog.computeGradient(dst, gradImage, gradOrient);
	//imwrite("gradImage.jpg", gradImage);
	//imwrite("gradOrient.jpg", gradOrient);

	DrawPicToHDC(gradImage, IDB_SHOW4, m_show4);
	m_info.AddString(L"�ݶȼ������");
	UpdateData(FALSE);

}



void CMaskDefectDetectingDlg::OnBnClickedButtonSaveshow2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	SavePicToWin(m_show2);
}


void CMaskDefectDetectingDlg::OnBnClickedButtonSaveshow3()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	SavePicToWin(m_show3);
}


void CMaskDefectDetectingDlg::OnBnClickedButtonSaveshow4()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	SavePicToWin(m_show4);
}


void CMaskDefectDetectingDlg::OnBnClickedCheckUsegrayimg()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_USEGRAYIMG))//��ѡ��
	{
		m_checkUseGrayimg = 1;
	}
	else//û��ѡ��
	{
		m_checkUseGrayimg = 0;
	}
}


void CMaskDefectDetectingDlg::OnBnClickedCheckUsegamma()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_USEGAMMA))//��ѡ��
	{
		m_checkUseGamma = 1;
	}
	else//û��ѡ��
	{
		m_checkUseGamma = 0;
	}
}

//����ѵ����Ŀ¼����ֵ��ǿĿ¼ΪTrainSetBinary.txt���Ҷ�ͼĿ¼ΪTrainSetGray.txt
void CMaskDefectDetectingDlg::OnBnClickedButtonCreatdir()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (m_pathTrainBand.IsEmpty())
	{
		MessageBox(L"��ѡ��ѵ����Ŀ¼");
		return;
	}
	string TrainSetBinaryTxtPath;
	string TrainSetGrayTxtPath;

	CString txtPath = m_pathTrainBand + "\\TrainSetBinary.txt";
	ofstream output(ws2s(txtPath.GetString()), ios::out);//�����ļ�,��������
	GetDir(m_pathSaveBandPos, txtPath, 1);
	GetDir(m_pathSaveBandNeg, txtPath, -1);
	DeleteLastLine(ws2s(txtPath.GetString()));
	m_info.AddString(L"��ֵ��ǿѵ����Ŀ¼�������");



	CString txtPath2 = m_pathTrainBand + "\\TrainSetGray.txt";
	ofstream output2(ws2s(txtPath2.GetString()), ios::out);//�����ļ�,��������
	GetDir(m_pathSaveBandPosGray, txtPath2, 1);
	GetDir(m_pathSaveBandNegGray, txtPath2, -1);
	DeleteLastLine(ws2s(txtPath2.GetString()));
	m_info.AddString(L"�Ҷ�����ѵ����Ŀ¼�������");


	////-----------------��Ϊѵ����Ŀ¼����Ϊ���Լ�Ŀ¼----------------////

	string TestSetBinaryTxtPath;
	string TestSetGrayTxtPath;

	CString txtPath3 = m_pathTrainBand + "\\TestSetBinary.txt";
	ofstream output3(ws2s(txtPath3.GetString()), ios::out);//�����ļ�,��������
	GetDir(m_pathSaveBandTestSet, txtPath3);
	DeleteLastLine(ws2s(txtPath3.GetString()));
	m_info.AddString(L"��ֵ��ǿ���Լ�Ŀ¼�������");


	CString txtPath4 = m_pathTrainBand + "\\TestSetGray.txt";
	ofstream output4(ws2s(txtPath4.GetString()), ios::out);//�����ļ�,��������
	GetDir(m_pathSaveBandTestSet, txtPath4);
	DeleteLastLine(ws2s(txtPath4.GetString()));
	m_info.AddString(L"�Ҷ��������Լ�Ŀ¼�������");

	UpdateData(FALSE);
}


void CMaskDefectDetectingDlg::OnEnChangeEditLbpcellsize()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	UpdateData();
	UpdateData(FALSE);
}


void CMaskDefectDetectingDlg::OnEnChangeEditHogimgsize()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	UpdateData();
	UpdateData(FALSE);
}


void CMaskDefectDetectingDlg::OnEnChangeEditDstrows()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	UpdateData();
	UpdateData(FALSE);
}


void CMaskDefectDetectingDlg::OnBnClickedButtonInfoClc()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_info.ResetContent();
	m_info.AddString(L"����״̬��Ϣ��");
}


void CMaskDefectDetectingDlg::OnBnClickedButtonFusiontrain()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

}


void CMaskDefectDetectingDlg::OnBnClickedButtonFusionpredict()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
