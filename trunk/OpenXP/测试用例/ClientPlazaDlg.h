// ClientPlazaDlg.h
//

#pragma once
#include "afxcmn.h"
#include "HTreeCtrlEX.h"

// CClientPlazaDlg 
class CClientPlazaDlg : public CDialog
{
public:
	CClientPlazaDlg(CWnd* pParent = NULL);	
	enum { IDD = IDD_CLIENTPLAZA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	

protected:
	HButton			m_btnDefault,m_btnDL,m_btnUnDL;
	HCheckBox		m_cbDefault;
	HSuperLink		m_staticDefault;
	HListCtrl		m_listDefault;
	HComboBox       m_comboxDefault;  
	HEdit           m_editDefault;
	HEditWnd		m_editWndDefault;
	HSliderCtrl     m_sliderDefault;
	HTreeCtrlEX		m_tcDefault;
	
	HZipFile        m_ZipFile;    
	HUnzipFile      m_UnZipFile;
	HMDFile         m_mdFile;   
	HZlibComp		m_zlibFile;	

	HMenu			m_menuDefault;
	HMenu			m_menuDefaultSub;
	HMenuData		menuData [8];

	HAnimationGif   m_gifDefault;
	HHtmlCtrl       m_htmlDefault; 
	HFlashPlay		m_flashDefault;

	HAdoConnect		m_adoDB;

	HAPIHook		m_hookKB;
	SYSINFO			m_ssiLook;
	HNetCardInfo	m_nciLook;	

	HAudioPlay		m_adPlay;
	int				m_nLPos;

private:
	CImage m_imgZip;//测试epk文件中的png图像

protected:
	HRESULT OnUnBreakDownLoadNotify(WPARAM,LPARAM);
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonDUn();
	afx_msg void OnBnClickedButtonD();
	afx_msg void OnBnClickedDrawItemButton();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
