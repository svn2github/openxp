// XLSReader.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "XLSReader.h"
#include "XLSReaderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CXLSReaderApp

BEGIN_MESSAGE_MAP(CXLSReaderApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CXLSReaderApp ����





CXLSReaderApp::CXLSReaderApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��

	CString excel_path=_T("");
	CString con_str=_T("");

	excel_path = "c:\\AsmFun.xls";

	CoInitialize(NULL);

	//��excel
	/*"HDR=Yes;" ��ʾ������ĵ�һ���Ǳ�ͷ��û�����ݡ� "HDR=No;"��֮�෴�� 
	"IMEX=1;"������������ʼ�ս�"intermixed"��������(numbers, dates, strings�ȵ�)��Ϊ�ı��Ͷ�ȡ�� 
	ע�⣺��ѡ���������Excel������дȨ�޵��޸ġ������д�����ݣ������±�ȱ���ʹ��Ϊ0*/
	ConnectionString = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=");   
	ConnectionString += excel_path;   //excel   file   name   
	ConnectionString += _T(";Extended Properties=\"Excel 8.0;HDR=Yes;IMEX=0\""); 

	BSTR resultsString = ConnectionString.AllocSysString();
	pCon_ex.CreateInstance(__uuidof(Connection));

	resultsString = ConnectionString.AllocSysString();
	pCon_ex->Open(resultsString,"","",adModeUnknown);
}


// Ψһ��һ�� CXLSReaderApp ����

CXLSReaderApp theApp;


// CXLSReaderApp ��ʼ��

BOOL CXLSReaderApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CXLSReaderDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

CXLSReaderApp::~CXLSReaderApp()
{
	if(pCon_ex) pCon_ex.Release();
}