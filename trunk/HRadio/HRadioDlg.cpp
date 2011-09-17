#include "stdafx.h"
#include "HRadio.h"
#include "HRadioDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CHRadioDlg::CHRadioDlg(CWnd* pParent) : CDialog(CHRadioDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_fontText.CreateFont(12,0,0,0,FW_BOLD,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH|FF_SWISS,_T("宋体"));  
}

void CHRadioDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OCX1, m_wmpRadio);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_BTN_MINSIZE, m_btnMin);
	DDX_Control(pDX, IDC_TREE1, m_tcRadio);
}

BEGIN_MESSAGE_MAP(CHRadioDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CHRadioDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_MINSIZE, &CHRadioDlg::OnBnClickedBtnMinsize)
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_HITTREE,OnHitTreeList)
	ON_MESSAGE(WM_NOTIFYICON,OnNotifyIcon)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CHRadioDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);	

	//设置窗体矩形
	m_ImgBK.Load(_T("bk.bmp"));
	MoveWindow(0,0,m_ImgBK.GetWidth(),m_ImgBK.GetHeight());
	CenterWindow();
	HRGN hRgn = ::CreateRoundRectRgn(0,0,m_ImgBK.GetWidth(),m_ImgBK.GetHeight(),20,20);
	SetWindowRgn(hRgn,FALSE);
	SetWindowText("网络收音机");

	SetTimer(1121,1000,NULL);
	m_wmpRadio.put_uiMode(_T("Invisible"));
	m_wmpSetting = (CWMPSettings)m_wmpRadio.get_settings(); 
	m_wmpSetting.put_volume(100);

	m_btnClose.SetButtonImage("close.bmp");
	m_btnMin.SetButtonImage("minsize.bmp");



	//设置电台列表
	HTREEITEM hRootItem;
	HTREEITEM hNewItem,hHotItem;
	hRootItem = m_tcRadio.InsertItemEx(NULL,_T("网络电台"),_T("item.bmp"));
	hNewItem = m_tcRadio.InsertItemEx(hRootItem,_T("推荐电台"),_T("item.bmp"));
	hHotItem = m_tcRadio.InsertItemEx(hRootItem,_T("热播电台"),_T("item.bmp"));

	m_xmlRadio.LoadForRead("radio.xml");
	int nCount = m_xmlRadio.GetInt("ALJ_MAX","count");
	CString str;
	for (int i = 0; i < nCount; i++)
	{
		str.Format("ALJ_%d",i);
		CString strName = m_xmlRadio.GetString(str.GetBuffer(str.GetLength()),"name");
		CString strMMS = m_xmlRadio.GetString(str.GetBuffer(str.GetLength()),"mms");
		CString strType = m_xmlRadio.GetString(str.GetBuffer(str.GetLength()),"type");
		if (strType == "0")//推荐
		{
			m_tcRadio.InsertItemEx(hNewItem,strName,_T("item.bmp"));
			m_mapRadio[strName] = strMMS;
		}
		else if (strType == "1")//热播
		{
			m_tcRadio.InsertItemEx(hHotItem,strName,_T("item.bmp"));
			m_mapRadio[strName] = strMMS;
		}
	}

	m_tcRadio.Expand(hRootItem,TVE_EXPAND);
	m_tcRadio.EnsureVisible(hRootItem);
	m_tcRadio.SelectItem(hRootItem);

	m_tcRadio.SetTreeBkImage(_T("TreeBK.bmp"));
	m_tcRadio.SetTextColor(RGB(180,214,184),RGB(255,255,255));
	m_tcRadio.SetRowLineColor(RGB(0,0,0));
	m_tcRadio.SetSelectedAndHotColor(RGB(30,10,0),RGB(60,10,0));

	HScrollBar(m_tcRadio.GetSafeHwnd(),NULL);

	NOTIFYICONDATA nd;
	nd.cbSize = sizeof (NOTIFYICONDATA);
	nd.hWnd	= m_hWnd;
	nd.uID = IDI_ICON;
	nd.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	nd.uCallbackMessage = WM_NOTIFYICON;
	nd.hIcon = m_hIcon;
	strcpy(nd.szTip, "网络收音机");
	Shell_NotifyIcon(NIM_ADD, &nd);
	return TRUE; 
}

void CHRadioDlg::OnPaint()
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
		CPaintDC dc(this);
		//获取位置
		CRect rect;
		GetClientRect(&rect);
		//建双缓冲
		CImage		imgBuffer;
		CDC			dcBuffer;
		imgBuffer.Create(rect.Width(), rect.Height(), 24);
		dcBuffer.Attach(imgBuffer.GetDC());

		m_ImgBK.Draw(dcBuffer.GetSafeHdc(),0,0);

		CFont *pOldFont = dcBuffer.SelectObject(&m_fontText);
		dcBuffer.SetBkMode(TRANSPARENT);
		dcBuffer.SetTextColor(RGB(180,214,184));

		long lState = m_wmpRadio.get_playState();// 1 停止 2 暂停 3 播放 6 正在缓冲 9 正在连接 10 准备就绪
		if (lState == 0)
			dcBuffer.DrawText(_T("停止"),-1,CRect(32,50,224,100),DT_LEFT);
		else
			dcBuffer.DrawText(m_wmpRadio.get_status(),-1,CRect(32,50,224,100),DT_LEFT);
		if (!m_strCurPlay.IsEmpty())
			dcBuffer.DrawText(m_strCurPlay,-1,CRect(0,70,224,100),DT_RIGHT);
		dcBuffer.SelectObject(pOldFont);

		//清除缓冲
		dc.BitBlt(0, 0, rect.Width(), rect.Height(), &dcBuffer, 0, 0, SRCCOPY);
		dcBuffer.Detach();
		imgBuffer.ReleaseDC();

		CDialog::OnPaint();
	}
}

HCURSOR CHRadioDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHRadioDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1121)
		Invalidate();
	CDialog::OnTimer(nIDEvent);
}

void CHRadioDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CRect rcClient;
	GetClientRect(rcClient);
	if (m_btnClose.GetSafeHwnd())
		m_btnClose.MoveWindow(rcClient.right-30,15,12,12);
	if (m_btnMin.GetSafeHwnd())
		m_btnMin.MoveWindow(rcClient.right-40-12,15,12,12);
	if (m_tcRadio.GetSafeHwnd())
		m_tcRadio.MoveWindow(2,105,258,290);
}

void CHRadioDlg::OnBnClickedBtnClose()
{
	//释放资源
	m_wmpRadio.close();
	NOTIFYICONDATA nd;
	nd.cbSize = sizeof (NOTIFYICONDATA);
	nd.hWnd	= m_hWnd;
	Shell_NotifyIcon(NIM_DELETE, &nd);

	PostMessage(WM_CLOSE,0,0);
}

void CHRadioDlg::OnBnClickedBtnMinsize()
{
	ShowWindow(SW_HIDE);
}

void CHRadioDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonDown(nFlags, point);
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
}

HRESULT CHRadioDlg::OnHitTreeList( WPARAM wParam,LPARAM lParam )
{
	m_wmpRadio.close();
	CString strText = m_tcRadio.GetItemText((HTREEITEM)wParam);
	m_wmpRadio.put_URL(m_mapRadio[strText]);
	m_strCurPlay = strText;
	return S_OK;
}

BOOL CHRadioDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

HRESULT CHRadioDlg::OnNotifyIcon(WPARAM wParam, LPARAM lParam)
{
	if (wParam == IDI_ICON)
	{
		if (lParam == WM_LBUTTONDOWN)
			ShowWindow(SW_SHOWNORMAL);
		else if (lParam == WM_LBUTTONDBLCLK)
			ShowWindow(SW_HIDE);
	}
	return TRUE;
}

void CHRadioDlg::OnCancel()
{
	CDialog::OnCancel();
}
