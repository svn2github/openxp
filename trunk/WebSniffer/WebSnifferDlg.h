// WebSnifferDlg.h : ͷ�ļ�
//

#pragma once
#include "resource.h"
#include "HSnifferMgr.h"

// CWebSnifferDlg �Ի���
class CWebSnifferDlg : public CDialog
{
// ����
public:
	void AddData(CString s0,CString s1,CString s2,CString s3,CString s4,CString s5,CString s6);
	CWebSnifferDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_WEBSNIFFER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��



	CListCtrl	m_ctrList;
	HSnifferMgr m_SniffMgr;


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSniffer();
};
