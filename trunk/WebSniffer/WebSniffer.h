// WebSniffer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CWebSnifferApp:
// �йش����ʵ�֣������ WebSniffer.cpp
//

class CWebSnifferApp : public CWinApp
{
public:
	CWebSnifferApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CWebSnifferApp theApp;