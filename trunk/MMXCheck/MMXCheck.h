// MMXCheck.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMMXCheckApp:
// �йش����ʵ�֣������ MMXCheck.cpp
//

class CMMXCheckApp : public CWinApp
{
public:
	CMMXCheckApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMMXCheckApp theApp;