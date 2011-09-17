#ifndef __DLGALTERPW__H__
#define __DLGALTERPW__H__
#pragma once
#include "resource.h"
#include "afxwin.h"

class CDlgAlterPW : public CDialog
{
	DECLARE_DYNAMIC(CDlgAlterPW)

public:
	CDlgAlterPW(CWnd* pParent = NULL);   
	virtual ~CDlgAlterPW();
	enum { IDD = IDD_DLG_ALTER };

protected:
	HEdit m_edOldPW;
	HEdit m_edNewPW;
	HEdit m_edNewPWSure;
	HButton m_btnOK;
	HButton m_btnCancel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};

#endif