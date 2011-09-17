// RobotBeeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RobotBee.h"
#include "RobotBeeDlg.h"
#include <PowrProf.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ENABLE_CONTROL_DOWN(button,enable)\
	button.EnableWindow(enable)

#define TRACE_S(s)\
	TRACE("%s\n",s)

#define TRACE_N(n)\
	TRACE("%d\n",n)

#define TRACE_F(f)\
	TRACE("%f\n",f)

#define IDM_TIMER_RULE						111
#define IDM_TIMER_DOWN						112
#define IDM_TIMER_CHECKTIME					113

CRobotBeeDlg::CRobotBeeDlg(CWnd* pParent) : CDialog(CRobotBeeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRobotBeeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ACTION, m_cbAction);
	DDX_Control(pDX, IDC_CHECK_FORCE, m_ckForce);
	DDX_Control(pDX, IDC_DATETIMEPICKER_RULE, m_dtRule);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DOWN, m_dtDown);
	DDX_Control(pDX, IDC_CHECK_DOWN, m_btnDown);
	DDX_Control(pDX, IDC_BUTTON_START, m_btnStart);
	DDX_Control(pDX, IDC_BUTTON_PAUSE, m_btnPause);
}

BEGIN_MESSAGE_MAP(CRobotBeeDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_DOWN, &CRobotBeeDlg::OnBnClickedCheckDown)
	ON_BN_CLICKED(IDC_BUTTON_START, &CRobotBeeDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CRobotBeeDlg::OnBnClickedButtonPause)
	ON_WM_TIMER()
	ON_WM_SYSCOMMAND()
	ON_MESSAGE(WM_HOTKEY,OnHotKey)
END_MESSAGE_MAP()

BOOL CRobotBeeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		

	m_wndStatusBar.Create(WS_CHILD|WS_VISIBLE|CCS_BOTTOM,CRect(0,0,0,0),this,10000); 
	int strPartDim[2]={280,-1};
	m_wndStatusBar.SetParts(2,strPartDim);
	m_wndStatusBar.SetText("ESC：隐藏窗体 ALT+R：隐藏/显示窗体  免费授权",0,0);
	m_wndStatusBar.SetText("读取时间中...",1,0);
	SetTimer(IDM_TIMER_CHECKTIME,1000,NULL);

	m_cbAction.InsertString(0,"等待       |进入待机状态");
	m_cbAction.InsertString(0,"休眠       |将你的电脑转入休眠状态");
	m_cbAction.InsertString(0,"注销       |注销你的账户");
	m_cbAction.InsertString(0,"重启       |重新启动电脑");
	m_cbAction.InsertString(0,"关机       |切断电脑电源");
	m_cbAction.SetCurSel(0);
	
	m_ckForce.SetCheck(true);

	ENABLE_CONTROL_DOWN(m_btnPause,FALSE);

	::RegisterHotKey(m_hWnd,1121,MOD_ALT,'R');

#if 0
	//修改时间用的几个函数

	SYSTEMTIME    stime;   
	GetSystemTime(&stime);//得到系统当前时间相对应格林威治时间   
	TIME_ZONE_INFORMATION    zinfo;   
	GetTimeZoneInformation(&zinfo);//得到时区信息   
	SystemTimeToTzSpecificLocalTime(&zinfo,&stime,&stime);//将格林威治时间转换为对应的当前时区的时间   
	SetLocalTime(&stime);//设置时间   
	//GetSystemTime    SetSystemTime    //都是使用的格林威治时间   
	//GetLocalTime    SetLocalTime    //才是本时区时间    其对应关系还是要搞清楚最好
#endif
	return TRUE;  
}

void CRobotBeeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CRobotBeeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRobotBeeDlg::RobotBee()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (GetVersionEx(&osvi) == 0)
		return;

	if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		if (!OpenProcessToken(GetCurrentProcess(),
			TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
			return;
		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
		tkp.PrivilegeCount = 1;   
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	}

	UINT nStyle = 0;
	if (m_ckForce.GetCheck()) nStyle = EWX_FORCE;
	int nIndex = m_cbAction.GetCurSel();
	switch (nIndex)
	{
	case 0:
		nStyle |= EWX_POWEROFF;
		break;
	case 1:
		nStyle |= EWX_REBOOT;
		break;
	case 2:
		nStyle |= EWX_LOGOFF;
		break;
	case 3://休眠
		SetSuspendState(TRUE,TRUE,TRUE);
		return;
	case 4://等待(挂起键盘)
		SetSuspendState(FALSE,TRUE,TRUE);
		return;
	default:break;
	}
	ExitWindowsEx(nStyle, 0);
}

//倒计时
void CRobotBeeDlg::OnBnClickedCheckDown()
{
	if (m_btnDown.GetCheck())
	{
		ENABLE_CONTROL_DOWN(m_dtRule,FALSE);
		ENABLE_CONTROL_DOWN(m_dtDown,TRUE);
	}
	else
	{
		ENABLE_CONTROL_DOWN(m_dtRule,TRUE);
		ENABLE_CONTROL_DOWN(m_dtDown,FALSE);
	}
}

//开始
void CRobotBeeDlg::OnBnClickedButtonStart()
{
	ENABLE_CONTROL_DOWN(m_btnStart,FALSE);
	ENABLE_CONTROL_DOWN(m_btnPause,TRUE);
	ENABLE_CONTROL_DOWN(m_dtRule,FALSE);
	ENABLE_CONTROL_DOWN(m_dtDown,FALSE);
	ENABLE_CONTROL_DOWN(m_btnDown,FALSE);
	ENABLE_CONTROL_DOWN(m_cbAction,FALSE);
	ENABLE_CONTROL_DOWN(m_ckForce,FALSE);

	if (m_btnDown.GetCheck())//选择了倒计时
	{
		CTime t;
		m_dtDown.GetTime(t);
		CTimeSpan ts(0,t.GetHour(),t.GetMinute(),t.GetSecond());
		m_ctCur = CTime::GetCurrentTime();
		m_ctCur += ts;
		SetTimer(IDM_TIMER_DOWN,1000,NULL);
	}
	else//定时关机
	{
		m_dtRule.GetTime(m_ctCur);
		SetTimer(IDM_TIMER_RULE,1000,NULL);
	}
}

//暂停
void CRobotBeeDlg::OnBnClickedButtonPause()
{
	ENABLE_CONTROL_DOWN(m_btnStart,TRUE);
	ENABLE_CONTROL_DOWN(m_btnPause,FALSE);
	ENABLE_CONTROL_DOWN(m_dtRule,TRUE);
	ENABLE_CONTROL_DOWN(m_dtDown,TRUE);
	ENABLE_CONTROL_DOWN(m_btnDown,TRUE);
	ENABLE_CONTROL_DOWN(m_cbAction,TRUE);
	ENABLE_CONTROL_DOWN(m_ckForce,TRUE);

	KillTimer(IDM_TIMER_RULE);
	KillTimer(IDM_TIMER_DOWN);
}

//定时器
void CRobotBeeDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == IDM_TIMER_RULE)
	{
		CTime t = CTime::GetCurrentTime();  
		if (t.GetHour() == m_ctCur.GetHour() && t.GetMinute() == m_ctCur.GetMinute() && t.GetSecond() == m_ctCur.GetSecond())
		{
			KillTimer(IDM_TIMER_RULE);
			RobotBee();
		}
	}
	else if (nIDEvent == IDM_TIMER_DOWN)
	{	
		CTime t = CTime::GetCurrentTime();  
		if (t.GetHour() == m_ctCur.GetHour() && t.GetMinute() == m_ctCur.GetMinute() && t.GetSecond() == m_ctCur.GetSecond())
		{
			KillTimer(IDM_TIMER_DOWN);
			RobotBee();
		}
	}
	else if (nIDEvent == IDM_TIMER_CHECKTIME)
	{
		CTime time = CTime::GetCurrentTime(); 
		CString date = time.Format("%Y-%m-%d %H:%M:%S"); 
		m_wndStatusBar.SetText(date,1,0);
	}
	CDialog::OnTimer(nIDEvent);
}

//消息解析
BOOL CRobotBeeDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
			ShowWindow(SW_HIDE);
			return true;
		case VK_RETURN:
			return true;
		default: break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

//系统消息解析
void CRobotBeeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if(nID == SC_MINIMIZE)
	{
		ShowWindow(SW_HIDE);
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

//热键解析
LRESULT CRobotBeeDlg::OnHotKey( WPARAM wParam,LPARAM lParam )
{
	if(wParam == 1121)
	{
		if(IsWindowVisible())
			ShowWindow(SW_HIDE);
		else
			ShowWindow(SW_SHOWNORMAL);
	}
	return 1;
}