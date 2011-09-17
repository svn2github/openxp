#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <map>
using namespace std;

typedef struct tagXML 
{
	TCHAR szName[260];
	TCHAR szFunc[1024];
	TCHAR szIntro[1024];
}Xml,*pXml;

class CMMXCheckDlg : public CDialog
{
public:
	CMMXCheckDlg(CWnd* pParent = NULL);
	~CMMXCheckDlg();
	enum { IDD = IDD_MMXCHECK_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButton1();
	HRESULT OnHitListCtrl(WPARAM wParam,LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

protected:
	void InitDlgItem();//初始化Dlg控件
	void ReadOSInfo();//读取OS信息
	void LoadMMX();//导入mmx指令说明 

public:
	HICON				m_hIcon;
	HListCtrl			m_lcInfo;
	HEdit				m_ecIn;
	CEdit				m_ecFunc;
	HButton				m_btCheck;
	CEdit				m_ecIntro;
	HXMLConfig			m_xmlLoad;
	map<int,pXml>		m_mapXML;
	CImage				m_ImgBK;
	HAnimationGif		m_gifLogo;
	CString				m_strAppName;	
	CString				m_strAppName1;	
};
