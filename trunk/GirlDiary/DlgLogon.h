#pragma once
#include "resource.h"
#include "afxwin.h"

class CDlgLogon : public CDialog
{
	DECLARE_DYNAMIC(CDlgLogon)

public:
	CDlgLogon(CWnd* pParent = NULL);   
	virtual ~CDlgLogon();
	enum { IDD = IDD_DLD_LOGON };

protected:
	HEdit m_edPassWord;
	HButton m_btnOK;
	HButton m_btnCancel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
