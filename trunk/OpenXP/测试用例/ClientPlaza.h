// ClientPlaza.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CClientPlazaApp:
// �йش����ʵ�֣������ ClientPlaza.cpp
//

class CClientPlazaApp : public CWinApp
{
public:
	CClientPlazaApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CClientPlazaApp theApp;