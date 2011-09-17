#ifndef __GIRLDIARYDLG__H__
#define __GIRLDIARYDLG__H__
#include "afxcmn.h"
#include "HTreeCtrlDiary.h"
#include "HScrollTip.h"
#include "afxwin.h"
#include "DlgAlterPW.h"
#pragma once


class CGirlDiaryDlg : public CDialog
{
public:
	CGirlDiaryDlg(CWnd* pParent = NULL);	
	~CGirlDiaryDlg();
	enum { IDD = IDD_GIRLDIARY_DIALOG };

protected:
	HICON				m_hIcon;//ͼ��
	HMDFile				m_mfEPK;//epk�����ļ�����
	CString				m_strEPK;//epk�����ļ�·��
	CImage				m_ImgBacK;//����ͼƬ
	CImage				m_ImgTip;//��ʾͼƬ
	HTreeCtrlDiary		m_tcDiary;//�ռ��б�ؼ�
	HScrollTip			m_stWnd;//�������ִ���
	CEdit				m_etDiary;//�༭��
	HButton				m_btnSave;//���水ť
	HButton				m_btnAlter;//�޸İ�ť
	CString				m_strCurTip;//��ǰ����ʾ��Ϣ
	HComboBox			m_cbWether;//����ѡ��
	HButton				m_btnNew;//����ռ�
	HButton				m_btnAlterPW;//�޸�����	
	CStatusBarCtrl		m_wndStatusBar;//״̬��
	HTREEITEM			m_hRootItem;//���οؼ���Item
	CString				m_strCurEditDate;//��ǰ�༭����
	HButton				m_btnChangeBK;
	int					m_nCurBackIndex;//��ǰ����ͼƬ����

protected:
	void ClearResourse();
	void InitTreeList();
	HRESULT OnDHitTreeCtrl(WPARAM wParam,LPARAM lParam);
	virtual void DoDataExchange(CDataExchange* pDX);	
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnAlter();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnNew();
	afx_msg void OnBnClickedBtnPw();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeEditIn();
public:
	afx_msg void OnBnClickedButton1();
};

#endif