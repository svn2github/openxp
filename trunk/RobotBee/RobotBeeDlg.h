#pragma once
#include "afxwin.h"
#include "afxdtctl.h"

class CRobotBeeDlg : public CDialog
{
public:
	CRobotBeeDlg(CWnd* pParent = NULL);	
	enum { IDD = IDD_ROBOTBEE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	
	void RobotBee();	

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedCheckDown();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonPause();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg LRESULT OnHotKey(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	HICON					m_hIcon;
	CComboBox				m_cbAction;
	CButton					m_ckForce;
	CDateTimeCtrl			m_dtRule;
	CDateTimeCtrl			m_dtDown;
	CButton					m_btnDown;
	CButton					m_btnStart;
	CButton					m_btnPause;
	CTime					m_ctCur;
	CStatusBarCtrl			m_wndStatusBar;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
