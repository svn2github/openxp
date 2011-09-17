// ClientPlazaDlg.cpp 
//

#include "stdafx.h"
#include "ClientPlaza.h"
#include "ClientPlazaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CClientPlazaDlg::CClientPlazaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientPlazaDlg::IDD, pParent),
	m_nLPos(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientPlazaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_EMAIL, m_staticDefault);
	DDX_Control(pDX, IDC_LIST1, m_listDefault);
	DDX_Control(pDX, IDC_BUTTON_D_UN, m_btnUnDL);
	DDX_Control(pDX, IDC_BUTTON_D, m_btnDL);
	DDX_Control(pDX, IDC_TREE1, m_tcDefault);
}

BEGIN_MESSAGE_MAP(CClientPlazaDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_D_UN, &CClientPlazaDlg::OnBnClickedButtonDUn)
	ON_BN_CLICKED(IDC_BUTTON_D, &CClientPlazaDlg::OnBnClickedButtonD)
	ON_BN_CLICKED(2000, &CClientPlazaDlg::OnBnClickedDrawItemButton)
	ON_MESSAGE(WM_UNBREAKNPTIFYPROCESS,OnUnBreakDownLoadNotify)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()




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


BOOL CClientPlazaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);		
	SetIcon(m_hIcon, FALSE);	

//////////////////////////////控件测试///////////////////////////////////

//1 -- button
	m_btnDefault.Create(_T("默认按钮"),WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,CRect(270,150,326,169),this,2000);
	m_btnDefault.SetTextColor(RGB(255,255,0));

//2 -- static
	m_staticDefault.SetURL(_T("http://www.g.cn"));

//3 -- listctrl
	m_listDefault.SetRowHeigt(25);               //设置行高度
	m_listDefault.SetHeaderHeight(1.0f);          //设置头部高度
	m_listDefault.SetTextColor(RGB(0,79,120));
	m_listDefault.SetHeaderLock(FALSE);

	m_listDefault.InsertColumn(0,"类别",LVCFMT_CENTER,72);
	m_listDefault.InsertColumn(1,"物品名称",LVCFMT_CENTER,146);
	m_listDefault.InsertColumn(2,"使用次数",LVCFMT_CENTER,56);
	m_listDefault.InsertColumn(3,"单价",LVCFMT_CENTER,60);
	m_listDefault.InsertColumn(4,"蓝钻贵族价",LVCFMT_CENTER,75);

	for (BYTE i = 0; i < 10; ++i)
	{
		m_listDefault.InsertItem(i,"道具");
		m_listDefault.SetItemText(i,1,"小喇叭");
		m_listDefault.SetItemText(i,2,"1");
		m_listDefault.SetItemText(i,3,"3.56");
		m_listDefault.SetItemText(i,4,"1.56");
	}

	HScrollBar(m_listDefault.GetSafeHwnd(),NULL);

//4 -- combobox
	m_comboxDefault.Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST|CBS_AUTOHSCROLL,CRect(50,50,150,14*7+50),this,689);
	m_comboxDefault.AddString(_T("第一项"));
	m_comboxDefault.AddString(_T("第二项"));
	m_comboxDefault.AddString(_T("第三项"));
	m_comboxDefault.AddString(_T("第四项"));
	m_comboxDefault.AddString(_T("第五项"));
	m_comboxDefault.AddString(_T("第六项"));
	m_comboxDefault.SetCurSel(0);

//5 -- edit
	m_editDefault.CreateSkinEdit(90,85+16,100,this,1879);
	m_editWndDefault.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(50, 0, 200, 30), this,564);	
	m_editWndDefault.SetString("123456");

//6 -- sliderctrl
	m_sliderDefault.CreateSkinSlide(TRUE,CRect(10,137,147,155),this);

//7 -- checkbox
	m_cbDefault.Create(_T("默认CheckBox"),WS_VISIBLE|WS_CHILD,CRect(50,80,150,96),this,478);
	m_cbDefault.SetTextColor(RGB(255,0,0));

//8 -- treectrl
	CString strTree;
	strTree = GetExeModulePath(AfxGetInstanceHandle());
	HTREEITEM hRootItem;
	HTREEITEM hItem;

	hRootItem = m_tcDefault.InsertItemEx(NULL,_T("最热游戏"),strTree+_T("1.bmp"));

	hItem = m_tcDefault.InsertItemEx(hRootItem,_T("欢乐斗地主－圣诞好礼送不停(38191)"),strTree+_T("3.bmp"));
	m_tcDefault.InsertItemEx(hItem,_T("欢乐领奖台"),strTree+_T("2.bmp"));
	m_tcDefault.InsertItemEx(hItem,_T("欢乐充值点"),strTree+_T("5.bmp"));

	HTREEITEM hTmp= m_tcDefault.InsertItemEx(hItem,_T("斗地主市场"),strTree+_T("1.bmp"));
	m_tcDefault.InsertItemEx(hTmp,_T("欢乐新手区"),strTree+_T("4.bmp"));
	m_tcDefault.InsertItemEx(hTmp,_T("欢乐高手区"),strTree+_T("3.bmp"));
	m_tcDefault.Expand(hItem,TVE_EXPAND);

	hItem = m_tcDefault.InsertItemEx(hRootItem,_T("中国象棋"),strTree+_T("6.bmp"));
	m_tcDefault.InsertItemEx(hItem,_T("新手区一区"),strTree+_T("4.bmp"));
	m_tcDefault.InsertItemEx(hItem,_T("新手区二区"),strTree+_T("1.bmp"));
	m_tcDefault.InsertItemEx(hItem,_T("新手区三区"),strTree+_T("4.bmp"));
	m_tcDefault.InsertItemEx(hItem,_T("新手区四区"),strTree+_T("6.bmp"));
	m_tcDefault.InsertItemEx(hItem,_T("新手区五区"),strTree+_T("4.bmp"));
	m_tcDefault.InsertItemEx(hItem,_T("新手区六区"),strTree+_T("3.bmp"));
	m_tcDefault.InsertItemEx(hItem,_T("新手区七区"),strTree+_T("5.bmp"));
	m_tcDefault.InsertItemEx(hItem,_T("新手区八区"),strTree+_T("4.bmp"));
	m_tcDefault.InsertItemEx(hItem,_T("新手区九区"),strTree+_T("1.bmp"));
	m_tcDefault.InsertItemEx(hItem,_T("新手区十区"),strTree+_T("3.bmp"));
	m_tcDefault.InsertItemEx(hItem,_T("高手区一区"),strTree+_T("4.bmp"));
	m_tcDefault.InsertItemEx(hItem,_T("高手区二区"),strTree+_T("2.bmp"));
	m_tcDefault.InsertItemEx(hItem,_T("高手区三区"),strTree+_T("4.bmp"));
	m_tcDefault.InsertItemEx(hItem,_T("高手区四区"),strTree+_T("1.bmp"));
	m_tcDefault.InsertItemEx(hItem,_T("高手区五区"),strTree+_T("6.bmp"));
	m_tcDefault.InsertItemEx(hItem,_T("高手区六区"),strTree+_T("4.bmp"));
	m_tcDefault.InsertItemEx(hItem,_T("高手区七区"),strTree+_T("3.bmp"));
	m_tcDefault.InsertItemEx(hItem,_T("高手区八区"),strTree+_T("4.bmp"));
	m_tcDefault.InsertItemEx(hItem,_T("高手区九区"),strTree+_T("1.bmp"));
	m_tcDefault.InsertItemEx(hItem,_T("高手区十区"),strTree+_T("5.bmp"));
	m_tcDefault.Expand(hRootItem,TVE_EXPAND);


	m_tcDefault.EnsureVisible(hRootItem);
	m_tcDefault.SelectItem(hRootItem);

	HScrollBar(this->GetSafeHwnd(),NULL);
	HScrollBar(m_tcDefault.GetSafeHwnd(),NULL);

//////////////////////////////调试测试/////////////////////////////////////
	
//1 -- log	
	if (HWriteLog(_T("测试Log...\n")))
		HWindowWrite(_T("Log测试正常...\n"));
	else
		HWindowWrite(_T("Log测试失败...\n"));

//2 -- memorycheck
#ifdef _DEBUG
	HCreateCheckPoint();
	int *p;
	p = new int[100];
	HOutPutMemoryInfo();
	HWindowWrite(_T("内存检测成功... 请查看‘输出’...\n"));
	delete p;
	p = NULL;
#endif

//////////////////////////////文件操作////////////////////////////////////

// -- ini文件
	HSetOperateFile(_T("c:\\test.ini"));
	HWriteDataToIni(_T("default"),_T("name"),_T("Wong San-ping"));
	HWriteDataToIni(_T("default"),_T("age"),_T("21"));
	HWriteDataToIni(_T("default"),_T("sex"),_T("man"));

// -- zip、rar文件
	HINSTANCE hInstance = theApp.m_hInstance;
	CString str_url = GetExeModulePath(hInstance);
	str_url += "MDFile工具.zip";
	m_UnZipFile.Open(str_url);
	m_UnZipFile.UnZipAll("c:\\");
	m_UnZipFile.Close();

// -- epk、不加密zip、不加密rar
	str_url = GetExeModulePath(AfxGetInstanceHandle());
	str_url += "test.epk";
	m_mdFile.Open(str_url,EPK_PLUG);
	DWORD dwLenght = 0;
	LPBYTE lpData = m_mdFile.GetBufferFromEpk("test.txt",&dwLenght);
	//拷贝缓冲区数据
	char *t = new char[dwLenght+1];
	ZeroMemory(t,dwLenght+1);
	memcpy(t,lpData,dwLenght+1);
	HWindowWrite(_T("加密文件读取的 -- "));
	HWindowWrite(t);
	HWindowWrite(_T("\n"));
	delete [] t;
	t = NULL;
	//得到压缩文件中的图像
	m_mdFile.GetImageFromEpk("ok.png",&m_imgZip,TRUE);
	HWindowWrite(_T("File测试正常...\n"));

//////////////////////////////下载组件////////////////////////////////////
	SetDlgItemText(IDC_EDIT_URL_UN,_T("http://www.google.com.hk/intl/zh-CN/images/logo_cn.gif"));
	SetDlgItemText(IDC_EDIT_NAME_UN,_T("UnBreakDownLoad.gif"));
	CProgressCtrl *pProcessUn = (CProgressCtrl *)GetDlgItem(IDC_PROGRESS_D_UN);
	pProcessUn->SetRange(0,100);	

	SetDlgItemText(IDC_EDIT_URL,_T("http://48.duote.org/uucall.exe"));
	SetDlgItemText(IDC_EDIT_NAME,_T("uucall.exe"));
	HDownloaderInit(this->GetSafeHwnd());
	CProgressCtrl *pProcess = (CProgressCtrl *)GetDlgItem(IDC_PROGRESS_D);
	pProcess->SetRange(0,100);

///////////////////////////////模板组件///////////////////////////////////
	m_gifDefault.Create(NULL,WS_VISIBLE|WS_CHILD,CRect(200,0,100,100),this);
	DWORD dwDataLeght = 0;
	LPBYTE lpDataBuffer = m_mdFile.GetBufferFromEpk(_T("dover.gif"),&dwDataLeght);
	m_gifDefault.Load(lpDataBuffer,dwDataLeght);	
	m_gifDefault.Draw();
	m_mdFile.Close();

	m_htmlDefault.CreateFromStatic(IDC_STATIC_HTML,this);
	m_htmlDefault.OpenURL(_T("http://www.baidu.com"));

	CString strFlash = GetExeModulePath(hInstance);
	strFlash += "1.swf";
	BOOL bCode = FALSE;
	bCode = m_flashDefault.CreateFlashWnd(this,CRect(650,20,200,300));
	m_flashDefault.SetPath(strFlash);
	m_flashDefault.StartPlay();

///////////////////////////////数据库组件///////////////////////////////////
	ClearLog();
	try
	{
		m_adoDB.SetConnectionInfo(_T("127.0.0.1"),1433,_T("LJGameOnline"),_T("sa"),_T("123456"));
		m_adoDB.OpenConnection();
	}
	catch(CADOError *pAdoError)
	{
		HTrace(pAdoError->GetErrorDescribe());
	}
	catch(...)
	{
		HTrace(_T("Ado连接发生未知错误，请稍后再重试！"));
	}
	HTrace(_T("(IP：127.0.0.1   DB：LJGameOnline) 数据库连接成功！"));

	m_adoDB.CloseConnection();


///////////////////////////////系统组件///////////////////////////////////
	m_hookKB.InterceptDllMember(GetModuleHandle(NULL),"Kernel32.dll","OutputDebugStringA",(DWORD)&sysTrace);

	HSystemInfo si;
	si.GetSystemInfo(m_ssiLook);
	m_nciLook.GetSetting();


///////////////////////////////音乐组件///////////////////////////////////
	CString strAD;
	strAD = GetExeModulePath(AfxGetInstanceHandle());
	m_mdFile.Open(strAD+"test.epk",EPK_PLUG);
	BYTE *pbyS = NULL;
	DWORD dwLength = 0;
	pbyS = m_mdFile.GetBufferFromEpk("Coco_Lee_-_A_Love_Before_Time.wav",&dwLength);
	m_adPlay.Play2D(pbyS,dwLength);
	//m_adPlay.Play2D(strAD+"Coco_Lee_-_A_Love_Before_Time.wav");
	m_mdFile.Close();

	POINT ptP;
	ptP.x = 0;
	ptP.y = 0;
	POINT ptL;
	ptL.x = m_nLPos;
	ptL.y = 0;
	//m_adPlay.Play3D(strAD+"Coco_Lee_-_A_Love_Before_Time.wav",ptP,ptL);
	return TRUE; 
}

void CClientPlazaDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

void CClientPlazaDlg::OnPaint()
{
	ALJ_GET_FUNC_TIME//函数执行时间测试

	CPaintDC dc(this);
	if (!m_imgZip.IsNull())
		m_imgZip.Draw(dc.GetSafeHdc(),0,0);
	dc.SetBkMode(TRANSPARENT);
	DrawTextString(&dc,_T("角度转换字符串"),RGB(255,255,255),0,30,0,-90);
	CDialog::OnPaint();
}

HCURSOR CClientPlazaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//////////////////////////////////////////////////////////////////////////
void CClientPlazaDlg::OnRButtonDown(UINT nFlags, CPoint point)
{

	HDumpInfo(m_ssiLook.chMemSize);
	HDumpInfo(m_nciLook.m_Cards[0].szDescription);


///////////////////////测试CMenu///////////////////////////////////////////
	
	m_menuDefault.DestroyMenu();
	m_menuDefault.CreatePopupMenu ();
	m_menuDefault.SetIconSize (25,25);
	m_menuDefault.SetHighlightStyle (Normal); 
	m_menuDefault.SetTextColor (RGB (0,0,0));
	m_menuDefault.SetBackColor (RGB (255,255,255));
	m_menuDefault.SetHighlightColor (RGB (49,136,198));

	lstrcpy (menuData[0].menuText , " 头像显示");
	menuData[0].menuIconNormal= -1;
	m_menuDefault.AppendMenu (MF_OWNERDRAW,1001,(LPCTSTR)&menuData[0]);
	
	lstrcpy (menuData[1].menuText , " 名称显示");
	menuData[1].menuIconNormal= -1;
	m_menuDefault.AppendMenu (MF_OWNERDRAW,1002,(LPCTSTR)&menuData[1]);

	lstrcpy (menuData[2].menuText , " 排序显示");
	menuData[2].menuIconNormal= IDI_DEFAULT;
	m_menuDefault.AppendMenu (MF_OWNERDRAW,1003,(LPCTSTR)&menuData[2]);

	m_menuDefault.AppendMenu(MF_OWNERDRAW,SEPARATOR_HMENU,SEPARATOR_TEXT);

	lstrcpy (menuData[3].menuText , " 刷新好友列表");
	menuData[3].menuIconNormal= -1;
	m_menuDefault.AppendMenu (MF_OWNERDRAW,1004,(LPCTSTR)&menuData[3]);

	lstrcpy (menuData[4].menuText , " 显示在线联系人");
	menuData[4].menuIconNormal= -1;
	m_menuDefault.AppendMenu (MF_OWNERDRAW,1005,(LPCTSTR)&menuData[4]);

	lstrcpy (menuData[5].menuText , " 显示陌生人分组");
	menuData[5].menuIconNormal= -1;
	m_menuDefault.AppendMenu (MF_OWNERDRAW,1006,(LPCTSTR)&menuData[5]);

	lstrcpy (menuData[6].menuText , " 显示黑名单");
	menuData[6].menuIconNormal= -1;
	m_menuDefault.AppendMenu (MF_OWNERDRAW,1007,(LPCTSTR)&menuData[6]); 

	EnableMenuItem(m_menuDefault.GetSafeHmenu(),1005,MF_GRAYED);
	CPoint screenP(point.x,point.y);
	ClientToScreen(&screenP);
	m_menuDefault.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,screenP.x,screenP.y,this);
	CDialog::OnRButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

void CClientPlazaDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1000)
	{
		int nTaskNum = HDownloaderGetTaskNum();
		if(nTaskNum < 1)
			return;

		int nFileTotaleSize = 0, nTotalDownloadedSize = 0, nTotalDownloadedSize_ThisTimes = 0, nDownloadElapsedTime = 0;
		for(int i = 1; i <= nTaskNum; i++)
		{
			HDownloaderQueryState(i,nFileTotaleSize,nTotalDownloadedSize,nTotalDownloadedSize_ThisTimes,nDownloadElapsedTime);
			
			float f = float(nTotalDownloadedSize/nFileTotaleSize*100.0);
			CString strBuf;
			strBuf.Format(_T("进度：%.2f%%"),f);  
			GetDlgItem(IDC_STATIC_SHOW)->SetWindowText(strBuf);

			CProgressCtrl *p = (CProgressCtrl *)GetDlgItem(IDC_PROGRESS_D);
			p->SetPos((short)f);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CClientPlazaDlg::OnClose()
{
	HDownloaderEnd();
	CDialog::OnClose();
}

void CClientPlazaDlg::OnBnClickedButtonDUn()
{
	HDumpInfo("将要非断点续传下载");

	CString strBuf,strBuf2;
	GetDlgItemText(IDC_EDIT_URL_UN,strBuf);
	GetDlgItemText(IDC_EDIT_NAME_UN,strBuf2);
	HUnBreakDownLoaderAddTask(
			strBuf.GetBuffer(strBuf.GetLength()),
			"c:",
			strBuf2.GetBuffer(strBuf2.GetLength()),
			this,
			1
		);
	GetDlgItem(IDC_BUTTON_D_UN)->EnableWindow(FALSE);
}

void CClientPlazaDlg::OnBnClickedButtonD()
{
	CString strBuf,strBuf2;
	GetDlgItemText(IDC_EDIT_URL,strBuf);
	GetDlgItemText(IDC_EDIT_NAME,strBuf2);
	HDownloaderAddTask(strBuf.GetBuffer(strBuf.GetLength()),"c:\\",strBuf2.GetBuffer(strBuf2.GetLength()),1);
	SetTimer(1000, 1000, NULL);
	GetDlgItem(IDC_BUTTON_D)->EnableWindow(FALSE);
}

HRESULT CClientPlazaDlg::OnUnBreakDownLoadNotify( WPARAM w,LPARAM l )
{
	if ((long)w == 10000)
	{
		GetDlgItem(IDC_STATIC_SHOW_UN)->SetWindowText(_T("下载成功"));
		return S_OK;
	} 
	else if ((long)l == -10000)
	{
		GetDlgItem(IDC_STATIC_SHOW_UN)->SetWindowText(_T("下载失败"));
		return S_OK;
	}
	short ulMax = (short)w;
	short ulNow = (short)l;

	CProgressCtrl *p = (CProgressCtrl *)GetDlgItem(IDC_PROGRESS_D_UN);
	p->SetPos(ulNow);

	CString strBuf;
	strBuf.Format(_T("进度：%.2f%%"),(float)ulNow/(float)ulMax*100.0);
	GetDlgItem(IDC_STATIC_SHOW_UN)->SetWindowText(strBuf);
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////

void CClientPlazaDlg::OnBnClickedDrawItemButton()
{
}

void CClientPlazaDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonDblClk(nFlags, point);
	SetWndAcrossDesk(this->GetSafeHwnd());
}
