// XLSReader.cpp : 定义应用程序的类行为。
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


// CXLSReaderApp 构造





CXLSReaderApp::CXLSReaderApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中

	CString excel_path=_T("");
	CString con_str=_T("");

	excel_path = "c:\\AsmFun.xls";

	CoInitialize(NULL);

	//打开excel
	/*"HDR=Yes;" 表示工作表的第一行是表头，没有数据。 "HDR=No;"与之相反。 
	"IMEX=1;"告诉驱动程序始终将"intermixed"数据类型(numbers, dates, strings等等)作为文本型读取。 
	注意：该选项可能引起Excel工作表写权限的修改。如果想写入数据，创建新表等必须使其为0*/
	ConnectionString = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=");   
	ConnectionString += excel_path;   //excel   file   name   
	ConnectionString += _T(";Extended Properties=\"Excel 8.0;HDR=Yes;IMEX=0\""); 

	BSTR resultsString = ConnectionString.AllocSysString();
	pCon_ex.CreateInstance(__uuidof(Connection));

	resultsString = ConnectionString.AllocSysString();
	pCon_ex->Open(resultsString,"","",adModeUnknown);
}


// 唯一的一个 CXLSReaderApp 对象

CXLSReaderApp theApp;


// CXLSReaderApp 初始化

BOOL CXLSReaderApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CXLSReaderDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

CXLSReaderApp::~CXLSReaderApp()
{
	if(pCon_ex) pCon_ex.Release();
}