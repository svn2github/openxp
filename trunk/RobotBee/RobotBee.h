#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"	
#ifndef _DEBUG
	#include ".\inc\SkinPPWTL.h"	
#endif


class CRobotBeeApp : public CWinApp
{
public:
	CRobotBeeApp();
public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CRobotBeeApp theApp;