// HookTraceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HookTrace.h"
#include "HookTraceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CHookTraceDlg 对话框




CHookTraceDlg::CHookTraceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHookTraceDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	g_pDlg = this;
}

void CHookTraceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lbHook);
}

BEGIN_MESSAGE_MAP(CHookTraceDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CHookTraceDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CHookTraceDlg 消息处理程序

BOOL CHookTraceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHookTraceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHookTraceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}






void WINAPI sysTrace(LPCSTR lpOutputString)
{
	AfxMessageBox(lpOutputString);
}

/*最后文件编译时间   //最后一次修改时间*/
#ifdef _DEBUG
	#define HDumpInfo(info)\
	{\
		CString strInfo;\
		strInfo.Format(_T("%s\n%s\n%d\n%s\n%s\n%s\n"),__FILE__,__FUNCTION__,__LINE__,__DATE__,__TIME__,__TIMESTAMP__);\
		strInfo += info;\
		strInfo += _T("\n");\
		OutputDebugStringA(strInfo);\
	}
#else
	#define HDumpInfo OutputDebugStringA
#endif



class Base;
typedef void (Base::*AOut)();
AOut func;

class Base
{
public:
	virtual void Output(){}
};

class A: public Base
{
public:
	void Output()
	{
		TRACE("Output class member\n");
	}
};

void newOutput()
{
	TRACE("hooked class member\n");
}

void CHookTraceDlg::OnBnClickedOk()
{
	HHOOK hk;
	static bool bStart = false;
	if (!bStart)
	{
		hk = m_hookKB.StartHook(&CHookTraceDlg::APICallBack);
		SetDlgItemText(IDOK,_T("卸载钩子"));
	}
	else
	{
		m_hookKB.StopHook();
		SetDlgItemText(IDOK,_T("开启钩子"));
	}
	bStart = !bStart;



	m_hookKB.InterceptDllMember(GetModuleHandle(NULL),"Kernel32.dll","OutputDebugStringA",(DWORD)&sysTrace);
	HDumpInfo("dll函数被注入成功！");

	HookMemberFunc(A::Output, newOutput, func);
	Base *a = new A();
	a->Output();  // 此时, 这行语句将会调用newOutput
	//(a->*func)(); // 这才是调用原来的A::Output

	delete a;
}

int CALLBACK CHookTraceDlg::APICallBack( WPARAM wParam,LPARAM lParam )
{
	switch (wParam)
	{
//这里键盘
	case VK_F5:
		{
			g_pDlg->m_lbHook.InsertString(0,_T("F5被按下"));
			break;
		}

//下面鼠标
	case WM_RBUTTONDBLCLK:
		{
			g_pDlg->m_lbHook.InsertString(0,_T("鼠标右键被按下"));
			break;
		}
	default:break;
	}
	return 0;
}