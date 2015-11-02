
// MaskDefectDetectingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MaskDefectDetecting.h"
#include "MaskDefectDetectingDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace cv;

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
	: CDialogEx(CMaskDefectDetectingDlg::IDD, pParent)
	, m_editKmax(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMaskDefectDetectingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_KMAX, m_editKmax);
}

BEGIN_MESSAGE_MAP(CMaskDefectDetectingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMaskDefectDetectingDlg::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON_READIMG, &CMaskDefectDetectingDlg::OnBnClickedButtonReadimg)
	ON_BN_CLICKED(IDC_BUTTON_CORRECTION, &CMaskDefectDetectingDlg::OnBnClickedButtonCorrection)
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
void CMaskDefectDetectingDlg::DrawPicToHDC(Mat img1, UINT ID)//�� Mat ��ͼƬ���Ƶ���ʾ�ؼ�
{
	IplImage *img = (&(IplImage)img1);//Mat �� Iplimage*
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf(img); // ����ͼƬ
	cimg.DrawToHDC(hDC, &rect); // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������
	ReleaseDC(pDC);
}

void CMaskDefectDetectingDlg::OnBnClickedButtonReadimg()
{
	////�ֶ���ͼƬ
	//// IDB_SHOW1 ΪPicture�ؼ���ID��
	//CFileDialog dlg(TRUE);
	//if (dlg.DoModal() == IDOK)
	//{
	//	CString FilePathName;
	//	FilePathName = dlg.GetPathName();
	//	//CString��string��ת��
	//	CStringA stra(FilePathName.GetBuffer(0));
	//	FilePathName.ReleaseBuffer();
	//	path = stra.GetBuffer(0);
	//	stra.ReleaseBuffer();
	//	openfiles = true;
	//}
	//if (pSrcImg) cvReleaseImage(&pSrcImg);
	//const char *charpath = path.c_str();
	//pSrcImg = cvLoadImage(charpath, 1); //��ͼ
	//if (pSrcImg)
	//{
	//	//pSrcImgCopy = cvCloneImage(pSrcImg);//������ǰͼ��
	//	DrawPicToHDC(pSrcImg, IDB_SHOW1);
	//	pNowImage = pSrcImg;//��ǰͼƬ
	//}

	pSrcImg = cvLoadImage("D:\\[Experiment]\\����ͼ��\\pylon_2ed\\quemianjin\\10.tiff", 1); //��ͼ
	if (pSrcImg)
	{
		DrawPicToHDC(pSrcImg, IDB_SHOW1);
		pNowImage = pSrcImg;//��ǰͼƬ
	}
	else
	{
		CString str("��ʼ��δ����ͼ��������� MaskDefectDetectingDlg.cpp");
		MessageBox(str);
	}
}

IplImage *g_pGrayImage = NULL;
IplImage *g_pBinaryImage = NULL;
const char *pstrWindowsBinaryTitle = "��ֵ�ָ��ֵͼ��";
//void on_trackbar(int pos)
//{
//	// תΪ��ֵͼ  
//	cvThreshold(g_pGrayImage, g_pBinaryImage, pos, 255, CV_THRESH_BINARY);
//	// ��ʾ��ֵͼ  
//	cvShowImage(pstrWindowsBinaryTitle, g_pBinaryImage);
//}

void   Delay(int   time)//time*1000Ϊ���� 
{
	clock_t   now = clock();
	while (clock() - now   <   time);
}

void CMaskDefectDetectingDlg::OnBnClickedButtonCorrection()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (pSrcImg == NULL)
		return;
	start_time = clock();//����ʼ��ʱ

	Mat src(pSrcImg, true);//pSrcImg����
	//	imgProcessing.Transformation(src);
	imgProcessing.SkewCorrection(src);//��������src
	DrawPicToHDC(imgProcessing.pNowImage, IDB_SHOW2);//�������pNowImage
	m_editKmax = imgProcessing.kmax;//���༭�򴫵�kmaxֵ
	UpdateData(false);//���±༭������
	end_time = (clock() - start_time) / 1000;//��ʱ
	std::cout << "��ʱ " << end_time << " ��" << std::endl;
}

void CMaskDefectDetectingDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
//My Code End