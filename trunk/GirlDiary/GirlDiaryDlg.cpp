#include "stdafx.h"
#include "GirlDiary.h"
#include "GirlDiaryDlg.h"
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

typedef struct tagDiary
{
	CString strYear;
	CString strMonth;
	CString strDay;
	CString strDiary;
}Diary;
vector<Diary*> g_vecRecord;

typedef struct tagDiaryText
{
	CString strDate;
	CString strDiaryText;
}DiaryText;
vector<DiaryText*> g_vecRecordText;

map<CString,HTREEITEM> g_mapYear; 

typedef struct tagMonthData
{
	CString strYear;
	CString strMonth;
	HTREEITEM hItem;
}MonthData;
vector<MonthData*> g_vecRecordMonth; 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDM_TIMER_CHECKTIME					113
#define MAX_DIARY_TEXT_LENGHT				20480	


void CrevasseDataFromFile(CString strFile,CString &strSrc)
{
	TCHAR szDest[MAX_DIARY_TEXT_LENGHT] = {0};
	TCHAR szSrc[MAX_DIARY_TEXT_LENGHT] = {0};
	CFile file;
	BOOL bReturn = file.Open(strFile,CFile::modeRead);
	if (!bReturn)
	{
		file.Close();
		return;
	}
	file.Read(szDest,(UINT)file.GetLength());
	if (strlen(szDest) <= 0)
	{
		file.Close();
		return;
	}
	for (int i = 0; i < MAX_DIARY_TEXT_LENGHT; i++)
	{
		szDest[i] ^= 23;
	}
	sprintf(szSrc,"%s",szDest);
	strSrc.Format("%s",szSrc);
}

void EncryptDataToFile(CString strFile,CString strSrc)
{
	TCHAR szDest[MAX_DIARY_TEXT_LENGHT] = {0};
	sprintf(szDest,"%s",strSrc);
	for (int i = 0; i < MAX_DIARY_TEXT_LENGHT; i++)
	{
		szDest[i] ^= 23;
	}
	CFile file;
	BOOL bReturn = file.Open(strFile,CFile::modeCreate|CFile::modeWrite);
	if (!bReturn)
	{
		file.Close();
		return;
	}
	file.Write(szDest,strlen(szDest));
	file.Close();
}

//构造函数
CGirlDiaryDlg::CGirlDiaryDlg(CWnd* pParent) : CDialog(CGirlDiaryDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strCurTip = "心情记事";
}

//清理资源
void CGirlDiaryDlg::ClearResourse()
{
	g_mapYear.clear();

	for (size_t i = 0; i < g_vecRecord.size(); i++)
	{
		Diary *pD = g_vecRecord[i];
		if (pD != NULL)
			SafeDelete(pD);
	}
	g_vecRecord.clear();

	for (size_t i = 0; i < g_vecRecordText.size(); i++)
	{
		DiaryText *pD = g_vecRecordText[i];
		if (pD != NULL)
			SafeDelete(pD);
	}	
	g_vecRecordText.clear();

	for (size_t i = 0; i < g_vecRecordMonth.size(); i++)
	{
		MonthData *pD = g_vecRecordMonth[i];
		if (pD != NULL)
			SafeDelete(pD);
	}	
	g_vecRecordMonth.clear();
}

//析构函数
CGirlDiaryDlg::~CGirlDiaryDlg()
{
	ClearResourse();
}

//控件绑定
void CGirlDiaryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_DIARY, m_tcDiary);
	DDX_Control(pDX, IDC_EDIT_IN, m_etDiary);
	DDX_Control(pDX, IDC_BTN_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_BTN_ALTER, m_btnAlter);
	DDX_Control(pDX, IDC_COMBO_WETHER, m_cbWether);
	DDX_Control(pDX, IDC_BTN_NEW, m_btnNew);
	DDX_Control(pDX, IDC_BTN_PW, m_btnAlterPW);
	DDX_Control(pDX, IDC_BUTTON1, m_btnChangeBK);
}

BEGIN_MESSAGE_MAP(CGirlDiaryDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_SAVE, &CGirlDiaryDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_ALTER, &CGirlDiaryDlg::OnBnClickedBtnAlter)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_NEW, &CGirlDiaryDlg::OnBnClickedBtnNew)
	ON_BN_CLICKED(IDC_BTN_PW, &CGirlDiaryDlg::OnBnClickedBtnPw)
	ON_WM_TIMER()
	ON_MESSAGE(WM_DHITTRLL,&CGirlDiaryDlg::OnDHitTreeCtrl)
	ON_EN_CHANGE(IDC_EDIT_IN, &CGirlDiaryDlg::OnEnChangeEditIn)
	ON_BN_CLICKED(IDC_BUTTON1, &CGirlDiaryDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

//初始化树形控件
void CGirlDiaryDlg::InitTreeList()
{
	CString strTree;
	strTree = GetExeModulePath(AfxGetInstanceHandle());
	m_hRootItem = m_tcDiary.InsertItemEx(NULL,_T("日记本"),strTree+"\\bak\\tc.lji");
	m_tcDiary.SelectItem(m_hRootItem);
	HScrollBar(m_tcDiary.GetSafeHwnd(),NULL);

	//导入日记信息
	CFileFind ff;
	BOOL bFind = ff.FindFile(strTree+"\\data\\*.ljd");
	while (bFind)
	{
		bFind = ff.FindNextFile();

		CString strFileTile,strDiaryText,strYear,strMonth,strDay;
		strFileTile = ff.GetFileTitle();
		
		CrevasseDataFromFile(ff.GetFilePath(),strDiaryText);	

		strYear = strFileTile.Left(4);
		strMonth = strFileTile.Mid(5,2);
		strDay = strFileTile.Right(2);

		Diary *pD = NULL;
		pD = new Diary();
		pD->strYear = strYear;
		pD->strMonth = strMonth;
		pD->strDay = strDay;
		pD->strDiary.Format("%s",strDiaryText);
		g_vecRecord.push_back(pD); 
	}
	ff.Close();

	//设置树形控件日记列表
	CString strYear,strMonth,strDay;
	Diary *pD = NULL;

	for (size_t i = 0; i < g_vecRecord.size(); i++)
	{
		pD = g_vecRecord[i];
		if (pD != NULL)
		{
			if (!m_tcDiary.ItemHasChildren(m_hRootItem))
			{
				HTREEITEM hTreeItem = m_tcDiary.InsertItemEx(m_hRootItem,pD->strYear+"年",strTree+"\\bak\\tc.lji");
				g_mapYear[pD->strYear+"年"] = hTreeItem;
			}
			else
			{
				BOOL bHasItem = FALSE;
				for (map<CString,HTREEITEM>::iterator it = g_mapYear.begin(); it != g_mapYear.end(); it++)
				{
					if (it->first == pD->strYear+"年")
					{
						bHasItem = TRUE;
						break;
					}
				}
				if (bHasItem)
					continue;
				HTREEITEM hTreeItem = m_tcDiary.InsertItemEx(m_hRootItem,pD->strYear+"年",strTree+"\\bak\\tc.lji");
				g_mapYear[pD->strYear+"年"] = hTreeItem;
			}
		}
	}

	//搜寻月份
	for (size_t i = 0; i < g_vecRecord.size(); i++)
	{
		pD = g_vecRecord[i];
		if (pD != NULL)
		{
			for (map<CString,HTREEITEM>::iterator it = g_mapYear.begin(); it != g_mapYear.end(); it++)
			{
				CString strYear = it->first;
				if (strYear == pD->strYear+"年")
				{
					if (!m_tcDiary.ItemHasChildren(it->second))
					{
						HTREEITEM hItemMonth = m_tcDiary.InsertItemEx(it->second,pD->strMonth+"月",strTree+"\\bak\\tc.lji");
						MonthData *md = new tagMonthData();
						md->strYear = pD->strYear;
						md->strMonth = pD->strMonth;
						md->hItem = hItemMonth;
						g_vecRecordMonth.push_back(md);	
					}
					else
					{
						BOOL bHasItem = FALSE;
						for (size_t j = 0; j < g_vecRecordMonth.size(); j++)
						{
							CString strText = g_vecRecordMonth[j]->strMonth;
							if (strText == pD->strMonth)
							{
								bHasItem = TRUE;
								break;
							}
						}
						if (bHasItem)
						{
							break;
						}
						else
						{
							HTREEITEM hItemMonth = m_tcDiary.InsertItemEx(it->second,pD->strMonth+"月",strTree+"\\bak\\tc.lji");
							MonthData *md = new tagMonthData();
							md->strYear = pD->strYear;
							md->strMonth = pD->strMonth;
							md->hItem = hItemMonth;
							g_vecRecordMonth.push_back(md);	
						}
					}
				}
			}
		}
	}

	//搜寻日期
	for (size_t i = 0; i < g_vecRecord.size(); i++)
	{
		pD = g_vecRecord[i];
		if (pD != NULL)
		{
			for (size_t j = 0; j < g_vecRecordMonth.size(); j++)
			{
				CString strYear,strMonth;
				MonthData *pMD = NULL;
				pMD = g_vecRecordMonth[j];
				if (NULL != pMD)
				{
					strYear = pMD->strYear;
					strMonth = pMD->strMonth;
					if (strYear == pD->strYear && strMonth == pD->strMonth)
					{
						//插入具体日记内容
						CString strDiaryFlag;
						strDiaryFlag.Format("%s-%s-%s",pD->strYear,pD->strMonth,pD->strDay);
						m_tcDiary.InsertItemEx(pMD->hItem,strDiaryFlag,strTree+"\\bak\\tc.lji");

						//判断当天日记是否有，有则"添加日记"按钮无用
						CTime time = CTime::GetCurrentTime(); 
						CString date = time.Format("%Y-%m-%d"); 
						if (strDiaryFlag == date)
							m_btnNew.EnableWindow(FALSE);

						//放入容器
						DiaryText *pDT = NULL;
						pDT = new DiaryText();
						pDT->strDate.Format("%s",strDiaryFlag);
						pDT->strDiaryText.Format("%s",pD->strDiary);
						g_vecRecordText.push_back(pDT); 
					}
				}
			}
		}
	}

	m_tcDiary.Expand(m_hRootItem,TVE_EXPAND);
}

//初始化对话框
BOOL CGirlDiaryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);
	//载入加密文件
	m_strEPK = GetExeModulePath(AfxGetInstanceHandle());
	m_strEPK += EPK_NAME;
	m_mfEPK.Open(m_strEPK,EPK_PLUG,EPK_PW);


	//载入应用程序功能名称
	DWORD dwLenght = 0;
	LPBYTE lpData = m_mfEPK.GetBufferFromEpk(EPK_APPNAME_FILE,&dwLenght);
	char *t = new char[dwLenght+1];
	ZeroMemory(t,dwLenght+1);
	memcpy(t,lpData,dwLenght+1);
	SetWindowText(t);
	delete [] t;
	t = NULL;


	//载入背景图片
	m_strEPK = GetExeModulePath(AfxGetInstanceHandle());
	TCHAR szBackImage[260] = {0};
	::GetPrivateProfileString("default","bkimage",_T("255"),szBackImage,sizeof(szBackImage),m_strEPK+"config.ini"); 
	int nImageIndex = atoi(szBackImage);
	if (nImageIndex >= EPK_BACKIMAGE_MAX || nImageIndex < 0)
		nImageIndex = EPK_ERROR_IMAGE_CODE;
	m_nCurBackIndex = nImageIndex;
	CString strImage = EPK_BAKCIMAGE_PATH;
	strImage += g_szBackImage[nImageIndex];
	m_mfEPK.GetImageFromEpk(strImage,&m_ImgBacK);

	//载入提示图片
	strImage = EPK_TIPIMAGE_PATH;
	m_mfEPK.GetImageFromEpk(strImage+"tip.png",&m_ImgTip,TRUE);


	//设置树形控件
	InitTreeList();

	//设置按钮图片
	CImage ImageButton;
	strImage = "btnimage\\";
	m_mfEPK.GetImageFromEpk(strImage+"pifu.bmp",&ImageButton,TRUE);
	m_btnChangeBK.SetButtonImage(&ImageButton);

	//创建滚动提示框
	m_stWnd.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,402);

	//挂编辑框滚动条
	HScrollBar(GetSafeHwnd(),NULL);
	HScrollBar(m_etDiary.GetSafeHwnd(),NULL);


	//天气选择框
	for (int i = MAX_TIP_WETHER-1; i >= 0 ; i--)
		m_cbWether.InsertString(0,g_szWether[i]);
	m_cbWether.SetCurSel(0);

	//设置窗口
	MoveWindow(0,0,798,575);
	CenterWindow();

	//设置状态栏
	m_wndStatusBar.Create(WS_CHILD|WS_VISIBLE|CCS_BOTTOM,CRect(0,0,0,0),this,10000); 
	int strPartDim[2]={580,-1};
	m_wndStatusBar.SetParts(2,strPartDim);
	m_wndStatusBar.SetText("佳佳日记本V1.0  免费授权<版权及解释权归作者所有>		QQ：77967292",0,0);
	m_wndStatusBar.SetText("	读取时间中...",1,0);
	SetTimer(IDM_TIMER_CHECKTIME,1000,NULL);
	return TRUE; 
}

//绘制
void CGirlDiaryDlg::OnPaint()
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
		if (!m_ImgBacK.IsNull())
			m_ImgBacK.Draw(memDC->GetSafeHdc(),0,0);
		if (!m_ImgTip.IsNull())
		{
			m_ImgTip.Draw(memDC->GetSafeHdc(),260,40,32,35);
		}
		memDC->SetBkMode(TRANSPARENT);
		if (!m_strCurTip.IsEmpty())
		{
			memDC->SetTextColor(RGB(128,0,128));
			memDC->DrawText(m_strCurTip,CRect(300,58,400,78),DT_LEFT);
		}
		memDC->SetTextColor(RGB(0,0,0));
		memDC->DrawText("天气",CRect(480,58,rcClient.Width(),rcClient.Height()),DT_LEFT);
		CDialog::OnPaint();
	}
}

//获取图标
HCURSOR CGirlDiaryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//编辑框有改动
void CGirlDiaryDlg::OnEnChangeEditIn()
{
	m_btnSave.EnableWindow(TRUE);
}

//消息解析
BOOL CGirlDiaryDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) 
			return FALSE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

//关闭窗体
void CGirlDiaryDlg::OnCancel()
{
	m_mfEPK.Close();
	CDialog::OnCancel();
}

//控件颜色设置
HBRUSH CGirlDiaryDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetSafeHwnd() == m_etDiary.GetSafeHwnd() || pWnd->GetSafeHwnd() == m_wndStatusBar.GetSafeHwnd())
	{
		pDC->SetBkColor(RGB(255,255,255)); 
		pDC->SetTextColor(RGB(128,128,0)); 
		HBRUSH hbrSub = CreateSolidBrush(RGB(255,255,255));
		return hbrSub;
	}
	return hbr;
}

//改变大小
void CGirlDiaryDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (m_tcDiary.GetSafeHwnd())
		m_tcDiary.MoveWindow(61,103,160,350);
	if (m_stWnd.GetSafeHwnd())
		m_stWnd.MoveWindow(61,83,160,16);
	if (m_etDiary.GetSafeHwnd())
		m_etDiary.MoveWindow(240,83,463,380);
}

//定时器
void CGirlDiaryDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == IDM_TIMER_CHECKTIME)
	{
		CTime time = CTime::GetCurrentTime(); 
		CString date = time.Format("	%Y-%m-%d %H:%M:%S"); 
		m_wndStatusBar.SetText(date,1,0);
	}
	CDialog::OnTimer(nIDEvent);
}

//双击树形控件
HRESULT CGirlDiaryDlg::OnDHitTreeCtrl( WPARAM wParam,LPARAM lParam )
{
	TCHAR szTextDate[2048] = {0};
	strcpy(szTextDate,(TCHAR*)wParam);
	CString strTextDate;
	strTextDate.Format("%s",szTextDate);
	for (size_t i = 0; i < g_vecRecordText.size(); i++)
	{
		DiaryText *pD = g_vecRecordText[i];
		if (pD != NULL)
		{
			if (pD->strDate == szTextDate)
			{
				m_strCurEditDate = pD->strDate;
				
				CString strWether;
				strWether = pD->strDiaryText.Left(8);
				for (int i = 0; i < m_cbWether.GetCount(); i++)
				{
					CString strBuffer;
					m_cbWether.GetLBText(i,strBuffer);
					if (strBuffer == strWether)
					{
						m_cbWether.SetCurSel(i);
						break;
					}
				}

				CString strDiaryText;
				strDiaryText = pD->strDiaryText.Mid(10);

				m_etDiary.SetWindowText(strDiaryText);
				m_btnAlter.EnableWindow(TRUE);
				break;
			}
		}
	}

	m_strCurTip = m_strCurEditDate;
	InvalidateRect(CRect(300,58,400,78));

	m_cbWether.Invalidate();	
	return S_OK;
}

//保存按钮
void CGirlDiaryDlg::OnBnClickedBtnSave()
{
	m_etDiary.SetReadOnly(TRUE);
	m_etDiary.EnableWindow(FALSE);
	m_btnSave.EnableWindow(FALSE);

	CString strWether;
	m_cbWether.GetWindowText(strWether);

	CString strText;
	m_etDiary.GetWindowText(strText);

	strWether += "\r\n";
	strWether += strText;
	strText = strWether;

	CString strSaveAbsPath;
	strSaveAbsPath = GetExeModulePath(AfxGetInstanceHandle());
	strSaveAbsPath += "\\data\\";
	strSaveAbsPath += m_strCurEditDate;
	strSaveAbsPath += ".ljd";

	CString strSuffix;
	strSuffix == strSaveAbsPath.Right(8);
	if (strSuffix == ".ljd.ljd")
		strSaveAbsPath.Mid(0,strSaveAbsPath.GetLength()-8);

	EncryptDataToFile(strSaveAbsPath,strText);

	m_tcDiary.DeleteAllItems();
	ClearResourse();

	InitTreeList();
}

//修改按钮
void CGirlDiaryDlg::OnBnClickedBtnAlter()
{
	m_etDiary.SetReadOnly(FALSE);
	m_etDiary.EnableWindow(TRUE);
	m_btnSave.EnableWindow(TRUE);
}

//添加日记
void CGirlDiaryDlg::OnBnClickedBtnNew()
{
	CTime time = CTime::GetCurrentTime(); 
	CString date = time.Format("%Y-%m-%d"); 
	m_strCurTip = date;
	Invalidate();

	m_strCurEditDate = date;

	OnBnClickedBtnSave();

	m_etDiary.SetReadOnly(FALSE);
	m_etDiary.EnableWindow(TRUE);
	m_etDiary.SetWindowText("");

	m_btnSave.EnableWindow(TRUE);
}

//修改密码
void CGirlDiaryDlg::OnBnClickedBtnPw()
{
	CDlgAlterPW  dap;
	dap.DoModal();
}

//更换背景
void CGirlDiaryDlg::OnBnClickedButton1()
{
	m_nCurBackIndex++;
	if (m_nCurBackIndex >= EPK_BACKIMAGE_MAX)
		m_nCurBackIndex = 0;
	CString strImage = EPK_BAKCIMAGE_PATH;
	strImage += g_szBackImage[m_nCurBackIndex];
	m_ImgBacK.Destroy();
	m_mfEPK.GetImageFromEpk(strImage,&m_ImgBacK);
	Invalidate();

	//存储到配置文件
	m_strEPK = GetExeModulePath(AfxGetInstanceHandle());
	TCHAR szBuffer[MAX_PATH] = {0};
	::WritePrivateProfileString("default","bkimage",itoa(m_nCurBackIndex,szBuffer,10),m_strEPK+"config.ini"); 
}
