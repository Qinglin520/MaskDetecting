
// MaskDefectDetectingDlg.h : ͷ�ļ�
//

#pragma once
#include <process.h>
#include <string>
#include "cv.h"
#include "highgui.h"
#include "CvvImage.h"
#include <ctime> 
#include "afxwin.h"
#include "MaskDefectProcessing.h"
#include "UseHOG.h"

// CMaskDefectDetectingDlg �Ի���
class CMaskDefectDetectingDlg : public CDialogEx
{
// ����
public:
	CMaskDefectDetectingDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MASKDEFECTDETECTING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


//My Code Begin
public:
	void DrawPicToHDC(IplImage *img, UINT ID);//��ͼƬ��ʾ���ؼ�
	void DrawPicToHDC(Mat img, UINT ID);//��ͼƬ��ʾ���ؼ�
	void DrawPicToHDC(Mat img, UINT ID, Mat &imgID);//��ͼƬ��ʾ���ؼ��������渱��
	void SavePicToWin(Mat img);//������ʾ��ͼƬ
	void saveImgWithNoBand(Mat src, Mat srcOrigin);//����ָ�İ�����޽����ͼ��
	void saveImgWithBand(Mat src, Mat srcOrigin);//����ָ�İ�����޽����ͼ��
	int SVMpredict(Mat src, CvSVM &SVM);//����ͼ��Ԥ�⣬ʹ��Binary������ȡ��LBP����������
	int binaryHogPredict(Mat src, CvSVM &SVM);//����ͼ��Ԥ�⣬ʹ��Binary������ȡ��HOG����������
	int GetDir(CString path, CString savePath, int cat);//����Ŀ¼����д���ǣ������txt
	int GetDir(CString path, CString savePath);//����Ŀ¼�������txt
	int DeleteLastLine(string path);//pathΪtxt·����ɾ��txt�ļ������һ��/n


public:
	CMaskDefectProcessing imgProcess;//����һ��ͼ������ʵ������Ҫ��ͼ��Ԥ������д�������
	CUseHOG useHog;////����һ��HoGѵ��Ԥ����ʵ��
	Mat m_src;//Դͼ��
	Mat m_srcImg;//����ͼ��
	Mat m_nowImg;//���ͼ��
	Mat m_show2;//��2��ͼ����ʾ��ͼ�񱸷�
	Mat m_show3;//��3��ͼ����ʾ��ͼ�񱸷�
	Mat m_show4;//��4��ͼ����ʾ��ͼ�񱸷�
	int m_checkUseGamma = 0;//checkboxѡ��״̬���
	int m_checkUseGrayimg = 0;//checkboxѡ��״̬���
	bool b_correction = false;//��̬У����ʶ��
	
	double start_time, end_time;
	bool openfiles = false;
	CListBox m_info;//״̬��
	string m_path;//��ȡͼƬ·��

	Mat m_imgL;//�����԰���к�����
	Mat m_imgLGray;//�����԰���к�����
	Mat m_imgR;//�����԰���о���ת����ұ�
	Mat m_imgRGray;//�����԰�����о���ת����ұ�

	CString m_pathOpenBand;
	CString m_pathTrainBand;// �޽���ѵ����Ŀ¼
	string m_strPathTrainBand;//m_pathTrainBand �� string ��ʽ
	CString m_pathSaveBandPos;//���޽��·��
	CString m_pathSaveBandNeg;//���޽��·��
	CString m_pathSaveBandTestSet;//���Լ�����·��
	CString m_pathSaveBandPosGray;//���޽�ԭͼ��һ������·��
	CString m_pathSaveBandNegGray;//���޽�ԭͼ��һ������·��
	CString m_pathSaveBandTestSetGray;//���Լ�ԭͼ��һ������·��

//���²������� .cpp �� CDialogEx �������ٴθ�ֵ���ʼ�����Ըú����еĸ�ֵ��ֵΪ׼��
	int CELLSIZE_LBP=8;// LBP�Ĵ��ڴ�С��4,8��16
	int dstrows = 128;//�԰�ָ�ǰ�����ŷֱ��ʵĸ�,����������ȡ������ͼ��߶�,����Ϊ2^n
	int dstcols = 2 * dstrows;//�԰�ָ�ǰ�����ŷֱ��ʵĿ�������������ȡ������ͼ����
	int TrainImgWidth = 64;
	int TrainImgHeight = TrainImgWidth;
	int num0 = 1;//���ڿ��ֶ԰�ָ�����޽˳�򱣴�
	int num1 = 1;//���ڿ��ֶ԰�ָ�����޽˳�򱣴�
	int numTestSet = 1;//���ڲ���ͼ��԰�ָ�󣬰�˳�򱣴�

	//MFC�Զ����ɵĿؼ���Ӧ����
	afx_msg void OnBnClickedButtonReadimg();//��ͼƬ
	afx_msg void OnBnClickedButtonCorrection();//����ͼ��
	afx_msg void OnBnClickedButtonBandprocess();
	void Bandprocess();//��������������һ�£�����������
	afx_msg void OnBnClickedButtonHairprocess();
	afx_msg void OnEnChangePathOpenBand();


	afx_msg void OnBnClickedButtonBatchBand();
	afx_msg void OnBnClickedButtonBatchBand2();
	afx_msg void OnBnClickedButtonBatchBand3();
	afx_msg void OnBnClickedButtonBatchBand4();
	afx_msg void OnBnClickedButtonBatchBandtestset();
	afx_msg void OnBnClickedButtonReadimg2();
	afx_msg void OnBnClickedButtonJudge();
	afx_msg void OnBnClickedButtonSvm();
	afx_msg void OnBnClickedButtonSvmpredict();
	afx_msg void OnEnChangePathTrainBand();

	afx_msg void OnBnClickedButtonSvmLbpRotation();
	afx_msg void OnBnClickedButtonSvmLbpRotationPredict();
	afx_msg void OnBnClickedButtonTrainHogSvm();
	afx_msg void OnBnClickedButtonPredictHogSvm();
	afx_msg void OnBnClickedButtonPrehog();
	afx_msg void OnBnClickedButtonSaveshow2();
	afx_msg void OnBnClickedButtonSaveshow3();
	afx_msg void OnBnClickedButtonSaveshow4();
	afx_msg void OnBnClickedCheckUsegrayimg();
	afx_msg void OnBnClickedCheckUsegamma();
	afx_msg void OnBnClickedButtonCreatdir();

	afx_msg void OnEnChangeEditLbpcellsize();
	afx_msg void OnEnChangeEditHogimgsize();
	afx_msg void OnEnChangeEditDstrows();
	afx_msg void OnBnClickedButtonInfoClc();
	afx_msg void OnBnClickedButtonFusiontrain();
	afx_msg void OnBnClickedButtonFusionpredict();
};