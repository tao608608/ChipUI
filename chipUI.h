
// chipUI.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CchipUIApp: 
// �йش����ʵ�֣������ chipUI.cpp
//

class CchipUIApp : public CWinApp
{
public:
	CchipUIApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CchipUIApp theApp;