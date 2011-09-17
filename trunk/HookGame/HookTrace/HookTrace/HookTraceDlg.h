// HookTraceDlg.h : 头文件
//

#pragma once
#include "HAPIHook.h"
#include "afxwin.h"
#pragma comment(lib,"HookGame.lib")

// CHookTraceDlg 对话框
class CHookTraceDlg : public CDialog
{
// 构造
public:
	CHookTraceDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_HOOKTRACE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	
	static int CALLBACK APICallBack(WPARAM wParam,LPARAM lParam);


// 实现
protected:
	HICON m_hIcon;
	HAPIHook    m_hookKB;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CListBox m_lbHook;
};

static CHookTraceDlg *g_pDlg = NULL;