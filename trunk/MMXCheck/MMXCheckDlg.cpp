#include "stdafx.h"
#include "MMXCheck.h"
#include "MMXCheckDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMMXCheckDlg::CMMXCheckDlg(CWnd* pParent) : CDialog(CMMXCheckDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_mapXML.clear();
}

CMMXCheckDlg::~CMMXCheckDlg()
{
	ALJ_GET_FUNC_TIME
	for (size_t i = 0; i < m_mapXML.size(); ++i)
	{
		pXml x = NULL;
		x = m_mapXML[i];
		if (x != NULL) delete x;
	}
}

void CMMXCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INFO, m_lcInfo);
	DDX_Control(pDX, IDC_EDIT_FUNC, m_ecFunc);
	DDX_Control(pDX, IDC_BUTTON1, m_btCheck);
	DDX_Control(pDX, IDC_EDIT_INTRO, m_ecIntro);
}

BEGIN_MESSAGE_MAP(CMMXCheckDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_HITCARLISTCTRL,OnHitListCtrl)
	ON_BN_CLICKED(IDC_BUTTON1, &CMMXCheckDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

//初始化
BOOL CMMXCheckDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);

	CString strMDF;
	strMDF = GetExeModulePath(AfxGetInstanceHandle());
	strMDF += "res.epk";
	HMDFile mf;
	mf.Open(strMDF,EPK_PLUG,"liujia");

	//载入背景图片
	mf.GetImageFromEpk("bk.jpg",&m_ImgBK);
	
	//载入gif
	m_gifLogo.Create(NULL,WS_VISIBLE|WS_CHILD,CRect(60,33,0,0),this);
	DWORD dwDataLeght = 0;
	LPBYTE lpDataBuffer = mf.GetBufferFromEpk(_T("dian.gif"),&dwDataLeght);
	m_gifLogo.Load(lpDataBuffer,dwDataLeght);	
	m_gifLogo.Draw();

	//载入应用程序功能名称
	DWORD dwLenght = 0;
	LPBYTE lpData = mf.GetBufferFromEpk("name.txt",&dwLenght);
	//拷贝缓冲区数据
	char *t = new char[dwLenght+1];
	ZeroMemory(t,dwLenght+1);
	memcpy(t,lpData,dwLenght+1);
	m_strAppName.Format("%s",t);
	delete [] t;
	t = NULL;

	//载入应用程序名称
	lpData = mf.GetBufferFromEpk("appname.txt",&dwLenght);
	//拷贝缓冲区数据
	t = new char[dwLenght+1];
	ZeroMemory(t,dwLenght+1);
	memcpy(t,lpData,dwLenght+1);
	m_strAppName1.Format("%s",t);
	delete [] t;
	t = NULL;

	//关闭文件
	mf.Close();


	InitDlgItem();
	LoadMMX();
	ReadOSInfo();

	return TRUE; 
}

//绘制
void CMMXCheckDlg::OnPaint()
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
		CRect rcClient;
		GetClientRect(rcClient);
		HMemDC memDC(&dc,rcClient);
		if (!m_ImgBK.IsNull())
			m_ImgBK.Draw(memDC.GetSafeHdc(),rcClient.left,rcClient.top,rcClient.Width(),rcClient.Height(),0,0,m_ImgBK.GetWidth(),m_ImgBK.GetHeight());
		CDialog::OnPaint();
	}
}

//取得光标
HCURSOR CMMXCheckDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//初始化Dlg控件
void CMMXCheckDlg::InitDlgItem()
{
	//ListCtrl
	m_lcInfo.SetRowHeigt(25);              
	m_lcInfo.SetHeaderHeight(1.0f);         
	m_lcInfo.SetTextColor(RGB(0,79,120));
	m_lcInfo.SetHeaderLock();
	m_lcInfo.InsertColumn(0,m_strAppName,LVCFMT_LEFT,115);
	HScrollBar(m_lcInfo.GetSafeHwnd(),NULL);

	//创建SkinEdit
	m_ecIn.CreateSkinEdit(158,30,335,this,189);

	//设置编辑框前面的提示STATIC
	SetDlgItemText(IDC_STATIC_TIP,m_strAppName);

	SetDlgItemText(IDC_STATIC_APP_TIP,m_strAppName1);
	SetWindowText(m_strAppName1);

	//挂滚动条(郁闷挂之前要找其他的先挂上，自己才有效)
	HScrollBar(GetSafeHwnd(),NULL);
	HScrollBar(m_ecFunc.GetSafeHwnd(),NULL);
	HScrollBar(GetSafeHwnd(),NULL);
	HScrollBar(m_ecIntro.GetSafeHwnd(),NULL);
}

//导入mmx指令集
void CMMXCheckDlg::LoadMMX()
{
	m_xmlLoad.LoadForRead("info.xml");
	int nCount = m_xmlLoad.GetInt("ALJ_MAX","count");
	CString str;
	for (int i = 0; i < nCount; i++)
	{
		str.Format("ALJ_%d",i);
		CString strList = m_xmlLoad.GetString(str.GetBuffer(str.GetLength()),"name");
		CString strFunc = m_xmlLoad.GetString(str.GetBuffer(str.GetLength()),"func");
		CString strIntro = m_xmlLoad.GetString(str.GetBuffer(str.GetLength()),"intro");
		m_lcInfo.InsertItem(i,strList);

		pXml x = new tagXML;
		memset(&x->szName,0,sizeof(x->szName));
		memset(&x->szFunc,0,sizeof(x->szFunc));
		memset(&x->szIntro,0,sizeof(x->szIntro));
		strcpy(x->szName,strList);
		strcpy(x->szFunc,strFunc);
		strcpy(x->szIntro,strIntro);
		m_mapXML[i] = x;
	}
}

//读取OS信息
void CMMXCheckDlg::ReadOSInfo()
{
	SYSINFO ss;
	HSystemInfo hsi;
	hsi.GetSystemInfo(ss);
	CString strSinfo;
#ifdef _DEBUG
	strSinfo.Format("OS:%s  CPU:%s",ss.chOS,ss.chCPU);
#else
	strSinfo.Format("OS:%s  IP:%s",ss.chOS,ss.chIP);
#endif
	SetDlgItemText(IDC_STATI_OS,strSinfo);
}

//设置控件颜色
HBRUSH CMMXCheckDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetSafeHwnd() == m_ecFunc.GetSafeHwnd() || pWnd->GetSafeHwnd() == m_ecIntro.GetSafeHwnd())
	{
		pDC->SetBkColor(RGB(255,255,255)); 
		HBRUSH br = ::CreateSolidBrush(RGB(255,255,255));
		return br;
	}
	else if (nCtlColor == CTLCOLOR_STATIC) 
	{ 
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(0,0,255));
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH); 
	} 
	return hbr;
}

//格式化字符串（将\lj转换为\r\n）
void ConvertString(CString &strIn)
{
	int nIndex = 0;
	while(true)
	{
		nIndex = strIn.Find("\lj");
		if (nIndex == -1) return;
		CString strBufferL = strIn.Left(nIndex-1);
		CString strBufferR = strIn.Right(strIn.GetLength()-nIndex-2);
		strBufferL += "\r\n";
		strBufferL += strBufferR;
		strIn = strBufferL;
	}
}

//点击ListCtrl
HRESULT CMMXCheckDlg::OnHitListCtrl( WPARAM wParam,LPARAM lParam )
{
	int nSelectedIndex = (int)wParam;
	CString strBuffer;
	
	strBuffer.Format("%s",m_mapXML[nSelectedIndex]->szFunc);
	ConvertString(strBuffer);
	m_ecFunc.SetWindowText(strBuffer);

	strBuffer.Format("%s",m_mapXML[nSelectedIndex]->szIntro);
	ConvertString(strBuffer);
	m_ecIntro.SetWindowText(strBuffer);
	
	return S_OK;
}

//点击查询按钮
void CMMXCheckDlg::OnBnClickedButton1()
{
	CString strIn;
	m_ecIn.GetWindowText(strIn);
	if (strIn.IsEmpty()) return;
	strIn.MakeLower();

	for (size_t i = 0; i < m_mapXML.size(); i++)
	{
		CString strBuf = m_mapXML[i]->szName;
		if (strIn == strBuf)
		{
			m_lcInfo.SetRowSelected(i);
			break;
		}
	}
}

//消息解析
BOOL CMMXCheckDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE) return FALSE;
		if (pMsg->wParam == VK_RETURN)
		{
			OnBnClickedButton1();
			return FALSE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
