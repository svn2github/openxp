#pragma once
#include <vector>
using namespace std;

typedef struct tagCheckInfo
{
	TCHAR szIndex[MAX_PATH];
	TCHAR szTime[MAX_PATH];
	TCHAR szType[MAX_PATH];
	TCHAR szDetail[1024];
}CheckInfo,*pCheckInfo;

class CDiskMonitorDlg : public CDialog
{
public:
	CDiskMonitorDlg(CWnd* pParent = NULL);	
	~CDiskMonitorDlg();
	enum { IDD = IDD_DISKMONITOR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	

public:
	typedef vector<pCheckInfo>          VecCheckInfo;
	VecCheckInfo						m_vecCI;

	BOOL								m_bAddNew;
	BOOL								m_bDelete;
	BOOL								m_bRename;
	BOOL								m_bModify;
	BOOL								m_bOther;

	BOOL								m_bAlwaysOnTop;
	BOOL								m_bAutoRun;
	BOOL								m_bMinimized;
	BOOL								m_bAutoStart;

	int									m_nListCount;
	TCHAR								*m_pszListIndex;

	HANDLE								m_hWatchThread;
	HANDLE								m_hDirectory;

	HICON								m_hIcon;
	CFont								m_hFont;
	CListCtrl							m_listAllInfo;
	CStatusBarCtrl						m_wndStatusBar;
	CString                             m_strWatchedDir;  

protected:
	//初始化对话框元素
	void InitDlgItem();
	//开始监视
	BOOL StartWatch(CString strPath);
	//线程函数，用来监视
	static DWORD WINAPI ThreadWatchProc(LPVOID lParam) ;

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonChoicepath();//选择监视目录
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonClearall();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonStop();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg LRESULT OnHotKey(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedButtonOption();
	afx_msg void OnBnClickedCheckAutostart();
	afx_msg void OnBnClickedCheckMinsize();
	afx_msg void OnBnClickedCheckAutorun();
	afx_msg void OnBnClickedCheckAlwayfround();
	virtual BOOL DestroyWindow();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	afx_msg void OnBnClickedButtonExport();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
