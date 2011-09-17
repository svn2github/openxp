#ifndef __HSELECTDLG__H__
#define __HSELECTDLG__H__
#pragma once
#include "resource.h"

class HSelectDlg : public CDialog
{
public:
	HSelectDlg(CWnd* pParent = NULL);
	~HSelectDlg();
	enum { IDD = IDD_DIALOG_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	

public:
	BOOL			m_bAddNew;
	BOOL			m_bDel;
	BOOL			m_bRename;
	BOOL			m_bModify;
	BOOL			m_bOther;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonYesall();
	afx_msg void OnBnClickedButtonNoall();
	afx_msg void OnBnClickedOk();
};

#endif