#include "stdafx.h"
#include "DiskMonitor.h"
#include "DiskMonitorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CDiskMonitorApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


CDiskMonitorApp::CDiskMonitorApp()
{
}

CDiskMonitorApp theApp;

BOOL CDiskMonitorApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	CWinApp::InitInstance();
	AfxEnableControlContainer();
	
	skinppLoadSkin("spring.ssk");//�����滻Ƥ��

	SetRegistryKey(_T("ϵͳĿ¼�仯����"));

#ifndef _DEBUG
	HANDLE m_hMutex = CreateMutex(NULL,TRUE, m_pszAppName); 
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL,_T("���ӳ������������ݲ���ͬʱ���������"),_T("ϵͳ��ʾ"),MB_OK);
		return FALSE; 
	}
#endif

	CDiskMonitorDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	return FALSE;
}

int CDiskMonitorApp::ExitInstance()
{
	skinppExitSkin(); 
	return CWinApp::ExitInstance();
}
