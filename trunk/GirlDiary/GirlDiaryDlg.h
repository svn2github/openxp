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
	HICON				m_hIcon;//图标
	HMDFile				m_mfEPK;//epk加密文件操作
	CString				m_strEPK;//epk加密文件路径
	CImage				m_ImgBacK;//背景图片
	CImage				m_ImgTip;//提示图片
	HTreeCtrlDiary		m_tcDiary;//日记列表控件
	HScrollTip			m_stWnd;//滚动文字窗口
	CEdit				m_etDiary;//编辑框
	HButton				m_btnSave;//保存按钮
	HButton				m_btnAlter;//修改按钮
	CString				m_strCurTip;//当前的提示信息
	HComboBox			m_cbWether;//天气选择
	HButton				m_btnNew;//添加日记
	HButton				m_btnAlterPW;//修改密码	
	CStatusBarCtrl		m_wndStatusBar;//状态栏
	HTREEITEM			m_hRootItem;//树形控件主Item
	CString				m_strCurEditDate;//当前编辑日期
	HButton				m_btnChangeBK;
	int					m_nCurBackIndex;//当前背景图片索引

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