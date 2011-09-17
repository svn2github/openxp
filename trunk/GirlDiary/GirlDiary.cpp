#include "stdafx.h"
#include "GirlDiary.h"
#include "GirlDiaryDlg.h"
#include "DlgLogon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CGirlDiaryApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CGirlDiaryApp::CGirlDiaryApp()
{
}

CGirlDiaryApp theApp;

BOOL CGirlDiaryApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	AfxEnableControlContainer();
	SetRegistryKey(_T("º—º—»’º«±æ"));

	CDlgLogon logo;
	INT_PTR nReturn = logo.DoModal();
	if (nReturn == IDCANCEL)
		return TRUE;

	CGirlDiaryDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	return FALSE;
}
