#pragma once
#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		
#include ".\inc\SkinPPWTL.h"

class CDiskMonitorApp : public CWinApp
{
public:
	CDiskMonitorApp();
public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CDiskMonitorApp theApp;