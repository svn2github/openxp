// WebSnifferDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WebSniffer.h"
#include "WebSnifferDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWebSnifferDlg 对话框




CWebSnifferDlg::CWebSnifferDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWebSnifferDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWebSnifferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INFO, m_ctrList);
}

BEGIN_MESSAGE_MAP(CWebSnifferDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CWebSnifferDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCE_SNIFFER, &CWebSnifferDlg::OnBnClickedSniffer)
END_MESSAGE_MAP()


// CWebSnifferDlg 消息处理程序

BOOL CWebSnifferDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	

	m_SniffMgr.SetRev(this);

	m_ctrList.InsertColumn(0,"数据",LVCFMT_LEFT,525);
	m_ctrList.InsertColumn(0,"大小",LVCFMT_LEFT,80);
	m_ctrList.InsertColumn(0,"端口",LVCFMT_LEFT,40);
	m_ctrList.InsertColumn(0,"目的地址",LVCFMT_LEFT,100);
	m_ctrList.InsertColumn(0,"端口",LVCFMT_LEFT,40);
	m_ctrList.InsertColumn(0,"源地址",LVCFMT_LEFT,100);
	m_ctrList.InsertColumn(0,"协议",LVCFMT_LEFT,50);

	::SendMessage(m_ctrList.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);


	GetDlgItem(IDOK)->EnableWindow(TRUE);
	GetDlgItem(IDCANCE_SNIFFER)->EnableWindow(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWebSnifferDlg::OnPaint()
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
HCURSOR CWebSnifferDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CWebSnifferDlg::OnBnClickedOk()
{
	GetDlgItem(IDCANCE_SNIFFER)->EnableWindow(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	m_SniffMgr.StartSniffer();
}

void CWebSnifferDlg::OnBnClickedSniffer()
{
	m_SniffMgr.StopSniffer();

	m_ctrList.DeleteAllItems();

	GetDlgItem(IDCANCE_SNIFFER)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
}

void CWebSnifferDlg::AddData(CString s0,CString s1, CString s2, CString s3, CString s4, CString s5, CString s6)
{
	int index;

	index = m_ctrList.InsertItem(0,s0);
	m_ctrList.SetItem(index,1,LVIF_TEXT,s1, 0, 0, 0,0);
	m_ctrList.SetItem(index,2,LVIF_TEXT,s2, 0, 0, 0,0);
	m_ctrList.SetItem(index,3,LVIF_TEXT,s3, 0, 0, 0,0);
	m_ctrList.SetItem(index,4,LVIF_TEXT,s4, 0, 0, 0,0);
	m_ctrList.SetItem(index,5,LVIF_TEXT,s5, 0, 0, 0,0);
	m_ctrList.SetItem(index,6,LVIF_TEXT,s6, 0, 0, 0,0);

	SetDlgItemInt(IDC_STATIC_SIZE,m_SniffMgr.GetMgrSize());
}