#include "stdafx.h"
#include "DiskMonitor.h"
#include "DiskMonitorDlg.h"
#include "HBrowseFolder.h"
#include "HSelectDlg.h"
#include "TimeTrace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDM_TIMER_CHECKTIME  1121

CDiskMonitorDlg::CDiskMonitorDlg(CWnd* pParent) : CDialog(CDiskMonitorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	m_hWatchThread = NULL;	
	m_hDirectory = INVALID_HANDLE_VALUE;

	m_vecCI.clear();
}

CDiskMonitorDlg::~CDiskMonitorDlg()
{
	pCheckInfo PI;
	for (size_t i = 0; i < m_vecCI.size(); i++)
	{
		PI = NULL;
		PI = m_vecCI[i];
		if (NULL != PI)
			delete PI;
	}
	m_vecCI.clear();

	if (m_pszListIndex != NULL)
	{
		delete m_pszListIndex;
		m_pszListIndex = NULL;
	}

	if(m_hWatchThread !=NULL)
	{
		::TerminateThread(m_hWatchThread, 0 );
		m_hWatchThread = NULL;
	}

	if(m_hDirectory !=INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hDirectory);
		m_hDirectory = INVALID_HANDLE_VALUE;
	}
}

void CDiskMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INFO, m_listAllInfo);
}

BEGIN_MESSAGE_MAP(CDiskMonitorDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_CHOICEPATH, &CDiskMonitorDlg::OnBnClickedButtonChoicepath)
	ON_BN_CLICKED(IDC_BUTTON_START, &CDiskMonitorDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_CLEARALL, &CDiskMonitorDlg::OnBnClickedButtonClearall)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CDiskMonitorDlg::OnBnClickedButtonStop)
	ON_WM_SYSCOMMAND()
	ON_MESSAGE(WM_HOTKEY,OnHotKey)
	ON_BN_CLICKED(IDC_BUTTON_OPTION, &CDiskMonitorDlg::OnBnClickedButtonOption)
	ON_BN_CLICKED(IDC_CHECK_AUTOSTART, &CDiskMonitorDlg::OnBnClickedCheckAutostart)
	ON_BN_CLICKED(IDC_CHECK_MINSIZE, &CDiskMonitorDlg::OnBnClickedCheckMinsize)
	ON_BN_CLICKED(IDC_CHECK_AUTORUN, &CDiskMonitorDlg::OnBnClickedCheckAutorun)
	ON_BN_CLICKED(IDC_CHECK_ALWAYFROUND, &CDiskMonitorDlg::OnBnClickedCheckAlwayfround)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CDiskMonitorDlg::OnBnClickedButtonExport)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_INFO, OnDblclkList1)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CDiskMonitorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);	
	InitDlgItem();
	return TRUE;  
}

void CDiskMonitorDlg::OnPaint()
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

HCURSOR CDiskMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//�����߳�
DWORD WINAPI CDiskMonitorDlg::ThreadWatchProc( LPVOID lParam )
{
	CDiskMonitorDlg *obj = (CDiskMonitorDlg*)lParam;  

	obj->m_hDirectory = CreateFile(            //��Ŀ¼���õ�Ŀ¼�ľ��
		obj->m_strWatchedDir,
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL
		); 
	if(obj->m_hDirectory ==INVALID_HANDLE_VALUE) 
		return false;

	char buf[(sizeof(FILE_NOTIFY_INFORMATION)+MAX_PATH)*2];
	FILE_NOTIFY_INFORMATION* pNotify=(FILE_NOTIFY_INFORMATION*)buf;
	DWORD dwBytesReturned;        
	while(true)
	{
		if(::ReadDirectoryChangesW(obj->m_hDirectory,
			pNotify,
			sizeof(buf),
			true,
			FILE_NOTIFY_CHANGE_FILE_NAME|
			FILE_NOTIFY_CHANGE_DIR_NAME|
			FILE_NOTIFY_CHANGE_ATTRIBUTES|
			FILE_NOTIFY_CHANGE_SIZE|
			FILE_NOTIFY_CHANGE_LAST_WRITE|
			FILE_NOTIFY_CHANGE_LAST_ACCESS|
			FILE_NOTIFY_CHANGE_CREATION|
			FILE_NOTIFY_CHANGE_SECURITY,
			&dwBytesReturned,
			NULL,
			NULL))
		{
			char tmp[MAX_PATH], str1[MAX_PATH], str2[MAX_PATH];
			memset(tmp, 0, sizeof(tmp)); 
			WideCharToMultiByte(CP_ACP,0,pNotify->FileName,pNotify->FileNameLength/2,tmp,99,NULL,NULL);
			strcpy(str1, tmp);

			if(pNotify->NextEntryOffset != 0)
			{
				PFILE_NOTIFY_INFORMATION p = (PFILE_NOTIFY_INFORMATION)((char*)pNotify+pNotify->NextEntryOffset);
				memset(tmp, 0, sizeof(tmp));
				WideCharToMultiByte(CP_ACP,0,p->FileName,p->FileNameLength/2,tmp,99,NULL,NULL);
				strcpy(str2, tmp);
			}
			switch(pNotify->Action)
			{				
			case FILE_ACTION_ADDED:
				if(obj->m_bAddNew)
				{
					CTime tt = CTime::GetCurrentTime();	
					CString strTT;
					strTT.Format(_T("%d:%d:%d"),tt.GetHour(),tt.GetMinute(),tt.GetSecond());
					obj->m_listAllInfo.InsertItem(0,obj->m_pszListIndex);
					obj->m_listAllInfo.SetItemText(0,2,_T("��������ļ�"));
					obj->m_listAllInfo.SetItemText(0,3,str1);
					obj->m_listAllInfo.SetItemText(0,1,strTT);
					
					pCheckInfo PI = NULL;
					PI = new tagCheckInfo();
					strcpy(PI->szIndex,obj->m_pszListIndex);
					strcpy(PI->szTime,strTT);
					strcpy(PI->szType,_T("��������ļ�"));
					strcpy(PI->szDetail,str1);
					obj->m_vecCI.push_back(PI);
				}
				break;

			case FILE_ACTION_REMOVED:
				if(obj->m_bDelete)
				{
					CTime tt=CTime::GetCurrentTime();	
					CString strTT;
					strTT.Format(_T("%d:%d:%d"),tt.GetHour(),tt.GetMinute(),tt.GetSecond());
					obj->m_listAllInfo.InsertItem(0,obj->m_pszListIndex);
					obj->m_listAllInfo.SetItemText(0,2,_T("ɾ�����ļ�"));
					obj->m_listAllInfo.SetItemText(0,3,str1);
					obj->m_listAllInfo.SetItemText(0,1,strTT);
					
					pCheckInfo PI = NULL;
					PI = new tagCheckInfo();
					strcpy(PI->szIndex,obj->m_pszListIndex);
					strcpy(PI->szTime,strTT);
					strcpy(PI->szType,_T("ɾ�����ļ�"));
					strcpy(PI->szDetail,str1);
					obj->m_vecCI.push_back(PI);
				}
				break;

			case FILE_ACTION_RENAMED_NEW_NAME:
				if(obj->m_bRename)
				{
					CTime tt = CTime::GetCurrentTime();	
					CString strTT;
					strTT.Format(_T("%d:%d:%d"),tt.GetHour(),tt.GetMinute(),tt.GetSecond());
					obj->m_listAllInfo.InsertItem(0,obj->m_pszListIndex);
					obj->m_listAllInfo.SetItemText(0,2,_T("���������ļ�"));
					strcat(str1,_T("->"));
					obj->m_listAllInfo.SetItemText(0,3,strcat(str1,str2));
					obj->m_listAllInfo.SetItemText(0,1,strTT);
					
					pCheckInfo PI = NULL;
					PI = new tagCheckInfo();
					strcpy(PI->szIndex,obj->m_pszListIndex);
					strcpy(PI->szTime,strTT);
					strcpy(PI->szType,_T("���������ļ�"));
					strcpy(PI->szDetail,strcat(str1,str2));
					obj->m_vecCI.push_back(PI);
				}
				break;

			case FILE_ACTION_RENAMED_OLD_NAME:
				if(obj->m_bRename)
				{
					CTime tt=CTime::GetCurrentTime();	
					CString strTT;
					strTT.Format(_T("%d:%d:%d"),tt.GetHour(),tt.GetMinute(),tt.GetSecond());
					obj->m_listAllInfo.InsertItem(0,obj->m_pszListIndex);
					obj->m_listAllInfo.SetItemText(0,2,_T("���������ļ�"));
					strcat(str1,_T(" ����Ϊ "));
					obj->m_listAllInfo.SetItemText(0,3,strcat(str1,str2));
					obj->m_listAllInfo.SetItemText(0,1,strTT);
					
					pCheckInfo PI = NULL;
					PI = new tagCheckInfo();
					strcpy(PI->szIndex,obj->m_pszListIndex);
					strcpy(PI->szTime,strTT);
					strcpy(PI->szType,_T("���������ļ�"));
					strcpy(PI->szDetail,strcat(str1,str2));
					obj->m_vecCI.push_back(PI);
				}
				break;

			case FILE_ACTION_MODIFIED:
				if(obj->m_bModify)
				{
					CTime tt = CTime::GetCurrentTime();	
					CString strTT;
					strTT.Format(_T("%d:%d:%d"),tt.GetHour(),tt.GetMinute(),tt.GetSecond());
					obj->m_listAllInfo.InsertItem(0,obj->m_pszListIndex);
					obj->m_listAllInfo.SetItemText(0,2,_T("�޸����ļ�"));
					obj->m_listAllInfo.SetItemText(0,3,str1);
					obj->m_listAllInfo.SetItemText(0,1,strTT);
					
					pCheckInfo PI = NULL;
					PI = new tagCheckInfo();
					strcpy(PI->szIndex,obj->m_pszListIndex);
					strcpy(PI->szTime,strTT);
					strcpy(PI->szType,_T("�޸����ļ�"));
					strcpy(PI->szDetail,str1);
					obj->m_vecCI.push_back(PI);
				}
				break;  

			default:
				if(obj->m_bOther)
				{
					CTime tt = CTime::GetCurrentTime();	
					CString strTT;
					strTT.Format(_T("%d:%d:%d"),tt.GetHour(),tt.GetMinute(),tt.GetSecond());
					obj->m_listAllInfo.InsertItem(0,obj->m_pszListIndex);
					obj->m_listAllInfo.SetItemText(0,2,_T("δ֪�仯"));
					obj->m_listAllInfo.SetItemText(0,3,_T(""));
					obj->m_listAllInfo.SetItemText(0,1,strTT);

					pCheckInfo PI = NULL;
					PI = new tagCheckInfo();
					strcpy(PI->szIndex,obj->m_pszListIndex);
					strcpy(PI->szTime,strTT);
					strcpy(PI->szType,_T("δ֪�仯"));
					strcpy(PI->szDetail,_T(""));
					obj->m_vecCI.push_back(PI);
				}
				break;  
			}	
			obj->m_nListCount++;		
			itoa(obj->m_nListCount,obj->m_pszListIndex,10);		
		}
		else           
			break;           
	}
	obj->m_nListCount = 1;
	return 0;
}

//��ʼ���Ի���Ԫ��
void CDiskMonitorDlg::InitDlgItem()
{
	//���״̬��
	m_wndStatusBar.Create(WS_CHILD|WS_VISIBLE|CCS_BOTTOM,CRect(0,0,0,0),this,10000); 
	int strPartDim[3]={220,350,-1};
	m_wndStatusBar.SetParts(3,strPartDim);//����״̬��Ϊ4����
	m_wndStatusBar.SetText("ESC�����ش��� ALT+W������/��ʾ����",0,0);
	m_wndStatusBar.SetText("��ȡʱ����...",1,0);
	m_wndStatusBar.SetText("ѡ���п����õ�ǰ������Ŀ (С��)",2,0);  
	SetTimer(IDM_TIMER_CHECKTIME,1000,NULL);

	m_listAllInfo.ModifyStyle(LVS_TYPEMASK,LVS_REPORT);
	m_listAllInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_FLATSB|LVS_EX_TWOCLICKACTIVATE |LVS_EX_GRIDLINES);
	m_listAllInfo.InsertColumn(0,_T("����"),LVCFMT_LEFT,40);
	m_listAllInfo.InsertColumn(1,_T("ʱ��"),LVCFMT_LEFT,80);
	m_listAllInfo.InsertColumn(2,_T("����"),LVCFMT_LEFT,100);
	m_listAllInfo.InsertColumn(3,_T("�仯������"),LVCFMT_LEFT,500);

	LOGFONT lf;
	::ZeroMemory(&lf,sizeof(lf)); 
	lf.lfHeight = 90;         
	lf.lfWeight = FW_NORMAL;   
	lf.lfItalic = false;	
	::lstrcpy(lf.lfFaceName,_T("Verdana")); 
	m_hFont.CreatePointFontIndirect(&lf);
	GetDlgItem(IDC_STATIC_PATH)->SetFont(&m_hFont);

	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);


	//����ini�����ļ�
	m_bAddNew = (::GetPrivateProfileInt("Selections","AddNew",0,".\\Settings.ini")==0 ?false:true);
	m_bDelete = (::GetPrivateProfileInt("Selections","Delete",0,".\\Settings.ini")==0 ?false:true);
	m_bRename = (::GetPrivateProfileInt("Selections","Rename",0,".\\Settings.ini")==0 ?false:true);
	m_bModify = (::GetPrivateProfileInt("Selections","Modify",0,".\\Settings.ini")==0 ?false:true);
	m_bOther  = (::GetPrivateProfileInt("Selections","Other",0,".\\Settings.ini")==0 ?false:true);

	char buf[MAX_PATH];
	::GetPrivateProfileString("Settings","WatchDir","",buf,MAX_PATH,".\\Settings.ini");
	m_strWatchedDir.Format("%s",buf);
	CString strBuffer = _T("��ѡ���˼��� ");
	m_strWatchedDir.FreeExtra();
	strBuffer += m_strWatchedDir;
	GetDlgItem(IDC_STATIC_PATH)->SetWindowText(strBuffer);

	CButton *p = NULL;
	m_bAlwaysOnTop = (::GetPrivateProfileInt("Settings","AlwaysOnTop",0,".\\Settings.ini")==0 ?false:true);
	p = (CButton*)GetDlgItem(IDC_CHECK_ALWAYFROUND);
	p->SetCheck(m_bAlwaysOnTop);
	if (m_bAlwaysOnTop)
		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | WS_EX_TOPMOST);
		

	m_bAutoRun = (::GetPrivateProfileInt("Settings","AutoRun",0,".\\Settings.ini")==0 ?false:true);
	p = (CButton*)GetDlgItem(IDC_CHECK_AUTORUN);
	p->SetCheck(m_bAutoRun);
	if (m_bAutoRun)
	{
		m_bAutoRun = false;
		OnBnClickedCheckAutorun();
	}

	m_bMinimized = (::GetPrivateProfileInt("Settings","Minimized",0,".\\Settings.ini")==0 ?false:true);
	p = (CButton*)GetDlgItem(IDC_CHECK_MINSIZE);
	p->SetCheck(m_bMinimized);
	if (m_bMinimized)
		PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);

	m_bAutoStart = (::GetPrivateProfileInt("Settings","AutoStart",0,".\\Settings.ini")==0 ?false:true);
	p = (CButton*)GetDlgItem(IDC_CHECK_AUTOSTART);
	p->SetCheck(m_bAutoStart);
	if (m_bAutoStart && !m_strWatchedDir.IsEmpty())
		OnBnClickedButtonStart();

	m_nListCount = 1;
	m_pszListIndex = new TCHAR[MAX_PATH];
	itoa(m_nListCount,m_pszListIndex,10);


	::RegisterHotKey(m_hWnd,1121,MOD_ALT,'W');
}

//ѡ�����Ŀ¼
void CDiskMonitorDlg::OnBnClickedButtonChoicepath()
{
	HBrowseFolder Dlg;
	if(Dlg.DoModal(this,NULL) == IDOK)
	{
		m_strWatchedDir.Format("%s",Dlg.GetDirPath());
		CString strBuffer = _T("��ѡ���˼��� ");
		GetDlgItem(IDC_STATIC_PATH)->SetWindowText(strBuffer+m_strWatchedDir);

		::WritePrivateProfileString("Settings","WatchDir",m_strWatchedDir,".\\Settings.ini");
	}
}

//��ʼ���Ӱ�ť
void CDiskMonitorDlg::OnBnClickedButtonStart()
{
	if (m_strWatchedDir.IsEmpty())
	{
		AfxMessageBox("��ѡ�����Ŀ¼��");
		return;
	}
	this->StartWatch(m_strWatchedDir);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_CHOICEPATH)->EnableWindow(false);
	CString strBuffer = _T("���ڼ���...    ");
	GetDlgItem(IDC_STATIC_PATH)->SetWindowText(strBuffer+m_strWatchedDir);
}

//��ʼ�����߳�
BOOL CDiskMonitorDlg::StartWatch( CString strPath )
{
	m_strWatchedDir = strPath;  
	DWORD ThreadId; 
	m_hWatchThread = ::CreateThread(NULL,0,ThreadWatchProc,this,0,&ThreadId);
	return (NULL!=m_hWatchThread);
}

//ֹͣ����
void CDiskMonitorDlg::OnBnClickedButtonStop()
{
	CString strBuffer = _T("ֹͣ�˼��� ");
	GetDlgItem(IDC_STATIC_PATH)->SetWindowText(strBuffer+m_strWatchedDir);
	if(m_hWatchThread != NULL)
	{
		::TerminateThread(m_hWatchThread,0);
		m_hWatchThread = NULL;
	}
	if(m_hDirectory != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hDirectory);
		m_hDirectory = INVALID_HANDLE_VALUE;
	}
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CHOICEPATH)->EnableWindow(TRUE);
}

//����б�ؼ�����
void CDiskMonitorDlg::OnBnClickedButtonClearall()
{
	m_listAllInfo.DeleteAllItems();
	m_nListCount = 1;

	pCheckInfo PI;
	for (size_t i = 0; i < m_vecCI.size(); i++)
	{
		PI = NULL;
		PI = m_vecCI[i];
		if (NULL != PI)
			delete PI;
	}
	m_vecCI.clear();
}

//�ؼ���ɫ
HBRUSH CDiskMonitorDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if(pWnd->GetDlgCtrlID() == IDC_STATIC_PATH)
		pDC->SetTextColor(RGB(0,0,255));
	return hbr;
}

//��Ϣ����
BOOL CDiskMonitorDlg::PreTranslateMessage(MSG* pMsg)
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

//ϵͳ��Ϣ����
void CDiskMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

//�ȼ�����
LRESULT CDiskMonitorDlg::OnHotKey( WPARAM wParam,LPARAM lParam )
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

//ѡ������
void CDiskMonitorDlg::OnBnClickedButtonOption()
{
	HSelectDlg sel;
	if(sel.DoModal() == IDOK)
	{
		this->m_bAddNew = sel.m_bAddNew;
		this->m_bDelete = sel.m_bDel;
		this->m_bModify = sel.m_bModify;
		this->m_bRename = sel.m_bRename;
		this->m_bOther  = sel.m_bOther;

		::WritePrivateProfileString("Selections","Add",sel.m_bAddNew? "1":"0",".\\Settings.ini");
		::WritePrivateProfileString("Selections","Delete",sel.m_bDel? "1":"0",".\\Settings.ini");
		::WritePrivateProfileString("Selections","Modify",sel.m_bModify? "1":"0",".\\Settings.ini");
		::WritePrivateProfileString("Selections","Rename",sel.m_bRename? "1":"0",".\\Settings.ini");
		::WritePrivateProfileString("Selections","Other",sel.m_bOther? "1":"0",".\\Settings.ini");
	}
}

//�������ļ�
void CDiskMonitorDlg::OnBnClickedButtonExport()
{
	pCheckInfo PI;
	for (size_t i = 0; i < m_vecCI.size(); i++)
	{
		PI = NULL;
		PI = m_vecCI[i];
		if (NULL != PI)
		{
			TCHAR szInfo[2048] = {0};
			sprintf(szInfo,"%s		-->		%s\r\n",PI->szType,PI->szDetail);

			if (strcmp(PI->szType,"��������ļ�") == 0)
			{
				HTraceHtml(HTML_COLOR_NORMAL,szInfo);
			}
			else if (strcmp(PI->szType,"ɾ�����ļ�") == 0)
			{
				HTraceHtml(HTML_COLOR_ERROR,szInfo);
			}
			else if (strcmp(PI->szType,"���������ļ�") == 0)
			{
				HTraceHtml(HTML_COLOR_WARNING,szInfo);
			}
			else if (strcmp(PI->szType,"�޸����ļ�") == 0)
			{
				HTraceHtml(HTML_COLOR_WARNING,szInfo);
			}
			else if (strcmp(PI->szType,"δ֪�仯") == 0)
			{
				HTraceHtml(HTML_COLOR_WARNING,szInfo);
			}
			delete PI;
		}
	}

	m_vecCI.clear();

	int nErrorCode = ::MessageBox(this->GetSafeHwnd(),_T("�����ɹ����Ƿ����ȫ����Ϣ��"),_T("ϵͳ��ʾ"),MB_YESNO);
	if (nErrorCode == 6)
		OnBnClickedButtonClearall();

	nErrorCode = ::MessageBox(this->GetSafeHwnd(),_T("����ɹ����Ƿ�鿴�����ļ���"),_T("ϵͳ��ʾ"),MB_YESNO);
	if (nErrorCode == 6)
	{
		TCHAR exeFullPath[MAX_PATH+1]; 
		GetModuleFileName(NULL,exeFullPath,MAX_PATH);
		(_tcsrchr(exeFullPath, _T('\\')))[1] = 0;//ɾ���ļ�����ֻ���·��
		::ShellExecute(NULL,"open",exeFullPath,NULL,NULL,SW_SHOW);
	}
}

//���ó�������ʱ���Զ���ʼ���
void CDiskMonitorDlg::OnBnClickedCheckAutostart()
{
	m_bAutoStart = !m_bAutoStart;
	::WritePrivateProfileString("Settings","AutoStart",m_bAutoStart? "1":"0",".\\Settings.ini");
}

//���ó�������ʱ���Զ�����
void CDiskMonitorDlg::OnBnClickedCheckMinsize()
{
	m_bMinimized = !m_bMinimized;
	::WritePrivateProfileString("Settings","Minimized",m_bMinimized? "1":"0",".\\Settings.ini");
}

//���ó��򿪻�������
void CDiskMonitorDlg::OnBnClickedCheckAutorun()
{
	m_bAutoRun = !m_bAutoRun;
	const TCHAR gcszAutoRunKey[]= _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	const TCHAR	gcszWindowClass[] = _T("LJSoft");

	HKEY hKey;
	LONG lRet, lRet2;
	DWORD dwLength, dwDataType;
	TCHAR szItemValue[MAX_PATH], szPrevValue[MAX_PATH];
	TCHAR szBuffer[MAX_PATH];

	GetModuleFileName(NULL, szItemValue, MAX_PATH);	

	lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,gcszAutoRunKey,0,KEY_READ|KEY_WRITE,&hKey);
	if(lRet != ERROR_SUCCESS) return;        

	dwLength = sizeof(szBuffer);
	lRet = RegQueryValueEx(hKey,gcszWindowClass,NULL,&dwDataType,(LPBYTE)szBuffer,&dwLength);


	if(m_bAutoRun)//���
	{
		::WritePrivateProfileString("Settings","AutoRun",m_bAutoRun? "1":"0",".\\Settings.ini");

		if(lRet != ERROR_SUCCESS)// AutoRun��Ŀ������
		{
			lRet2 = RegSetValueEx(hKey,gcszWindowClass,0,REG_SZ,(LPBYTE)szItemValue,strlen(szItemValue));
		}
		else//����, �Ƚ϶����Ƿ���ͬ
		{

			dwLength = sizeof(szPrevValue);
			lRet2 = RegQueryValueEx(hKey, gcszWindowClass,0, &dwDataType,(LPBYTE)szPrevValue, &dwLength);

			if(lstrcmp(szItemValue, szPrevValue))// ����ͬ���滻
			{
				lRet2 = RegDeleteValue( hKey, gcszWindowClass );
				lRet2 = RegSetValueEx( hKey, gcszWindowClass,0, REG_SZ,(LPBYTE)szItemValue, strlen( szItemValue ) );
			}
		}
	}
	else 	// ɾ��
	{
		if(lRet == ERROR_SUCCESS)
			lRet2 = RegDeleteValue(hKey,gcszWindowClass);	// AutoRun��Ŀ�Ѵ�����ɾ��
		::WritePrivateProfileString("Settings","AutoRun",m_bAutoRun? "1":"0",".\\Settings.ini");
	}
	RegCloseKey(hKey);	
}

//���ô���Zϵλ��
void CDiskMonitorDlg::OnBnClickedCheckAlwayfround()
{
	m_bAlwaysOnTop = !m_bAlwaysOnTop;
	if (m_bAlwaysOnTop)
	{
		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | WS_EX_TOPMOST);
		::WritePrivateProfileString("Settings","AlwaysOnTop",m_bAlwaysOnTop? "1":"0",".\\Settings.ini");
	}
	else
	{
		::SetWindowPos(GetSafeHwnd(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		::WritePrivateProfileString("Settings","AlwaysOnTop",m_bAlwaysOnTop? "1":"0",".\\Settings.ini");
	}
}

//���ٴ���
BOOL CDiskMonitorDlg::DestroyWindow()
{
	::UnregisterHotKey(m_hWnd,1121);
	return CDialog::DestroyWindow();
}

//ϵͳ�رհ�ť
void CDiskMonitorDlg::OnCancel()
{
	if (!::IsWindowEnabled(GetDlgItem(IDC_BUTTON_START)->GetSafeHwnd()))
	{
		int nErrorCode = ::MessageBox(this->GetSafeHwnd(),_T("������������У��Ƿ�ǿ���˳���"),_T("ϵͳ��ʾ"),MB_YESNO);
		if (nErrorCode == 7)
			return;
	}
	CDialog::OnCancel();
}

//˫���б�ؼ�
void CDiskMonitorDlg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nIndex = m_listAllInfo.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
	if(nIndex == -1)
		return;

	TCHAR buf[MAX_PATH];
	m_listAllInfo.GetItemText(nIndex,3,buf,MAX_PATH);
	CString str;	
	str = this->m_strWatchedDir+"\\"+buf;

	::ShellExecute(NULL,"open",str,NULL,NULL,SW_SHOW);

	*pResult = 0;
}

//��ʱ��
void CDiskMonitorDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == IDM_TIMER_CHECKTIME)
	{
		CTime time = CTime::GetCurrentTime(); 
		CString date = time.Format("%Y-%m-%d %H:%M:%S"); 
		m_wndStatusBar.SetText(date,1,0);
	}
	CDialog::OnTimer(nIDEvent);
}
