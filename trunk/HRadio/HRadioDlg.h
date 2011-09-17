#pragma once
#include "CWmpOCX.h"
#include "CWMPSettings.h"
#include "afxcmn.h"
#include <atlimage.h>
#include "afxwin.h"
#include "HRadioTree.h"
#include <map>
using namespace std;

#define WM_NOTIFYICON	WM_USER+5  //自定义消息的ID
#define IDI_ICON		0x0005     //图标ID

class CHRadioDlg : public CDialog
{
public:
	CHRadioDlg(CWnd* pParent = NULL);	
	enum { IDD = IDD_HRADIO_DIALOG };
	HRESULT OnHitTreeList(WPARAM wParam,LPARAM lParam);
	//自定义WM_NOTIFYICON消息的处理函数
	HRESULT OnNotifyIcon(WPARAM wParam, LPARAM lParam);  

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON					m_hIcon;
	CImage					m_ImgBK;
	CWmpOCX					m_wmpRadio;
	CWMPSettings			m_wmpSetting;
	HButton					m_btnClose;
	HButton					m_btnMin;
	HRadioTree				m_tcRadio;
	CFont					m_fontText;
	map<CString,CString>	m_mapRadio;
	CString					m_strCurPlay;
	HXMLConfig				m_xmlRadio;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnMinsize();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnCancel();
};
