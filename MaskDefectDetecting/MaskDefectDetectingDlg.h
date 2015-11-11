
// MaskDefectDetectingDlg.h : ͷ�ļ�
//

#pragma once
#include <process.h>
#include <string>
#include "cv.h"
#include "highgui.h"
#include "CvvImage.h"
#include "MaskDefectProcessing.h"
#include <ctime> 

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
public:	
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
//My Code Begin
public:
	void DrawPicToHDC(IplImage *img, UINT ID);//��ͼƬ��ʾ���ؼ�
	void DrawPicToHDC(Mat img, UINT ID);//��ͼƬ��ʾ���ؼ�

public:
	CMaskDefectProcessing imgProcessing;
	IplImage *pNowImage=NULL;//��ǰͼƬ��һ��Ϊ���ͼ��
	double start_time, end_time;

	afx_msg void OnBnClickedButtonReadimg();//��ͼƬ
	bool openfiles = false;
	std::string path;//ͼƬ·��
	IplImage * pSrcImg=NULL;//��ȡͼ��

	afx_msg void OnBnClickedButtonCorrection();//����ͼ��

};

