
// MaskDefectDetecting.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMaskDefectDetectingApp: 
// �йش����ʵ�֣������ MaskDefectDetecting.cpp
//

class CMaskDefectDetectingApp : public CWinApp
{
public:
	CMaskDefectDetectingApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMaskDefectDetectingApp theApp;