// HookTraceDlg.h : ͷ�ļ�
//

#pragma once
#include "HAPIHook.h"
#include "afxwin.h"
#pragma comment(lib,"HookGame.lib")

// CHookTraceDlg �Ի���
class CHookTraceDlg : public CDialog
{
// ����
public:
	CHookTraceDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_HOOKTRACE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	
	static int CALLBACK APICallBack(WPARAM wParam,LPARAM lParam);


// ʵ��
protected:
	HICON m_hIcon;
	HAPIHook    m_hookKB;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CListBox m_lbHook;
};

static CHookTraceDlg *g_pDlg = NULL;