// XLSReader.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CXLSReaderApp:
// �йش����ʵ�֣������ XLSReader.cpp
//

class CXLSReaderApp : public CWinApp
{
public:
	CXLSReaderApp();
	~CXLSReaderApp();

// ��д
	public:
	virtual BOOL InitInstance();

	_ConnectionPtr pCon_ex;
	CString ConnectionString;

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CXLSReaderApp theApp;