#include "stdafx.h"
#include "RobotBee.h"
#include "RobotBeeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CRobotBeeApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


CRobotBeeApp::CRobotBeeApp()
{
}

CRobotBeeApp theApp;

BOOL CRobotBeeApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

#ifndef _DEBUG
	skinppLoadSkin("spring.ssk");//�����滻Ƥ��
#endif

	SetRegistryKey(_T("�Զ��ػ�ħ��ʦ"));

#ifndef _DEBUG
	HANDLE m_hMutex = CreateMutex(NULL,TRUE, m_pszAppName); 
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL,_T("���ӳ������������ݲ���ͬʱ���������"),_T("ϵͳ��ʾ"),MB_OK);
		return FALSE; 
	}
#endif

	CRobotBeeDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	return FALSE;
}

int CRobotBeeApp::ExitInstance()
{
#ifndef _DEBUG
	skinppExitSkin(); 
#endif
	return CWinApp::ExitInstance();
}