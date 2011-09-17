#ifndef _HEDIT_H_
#define _HEDIT_H_
#pragma once

class UI_CONTROL_API HEdit : public CEdit
{
	DECLARE_DYNAMIC(HEdit)

public:
	HEdit();
	virtual ~HEdit();
	BOOL CreateSkinEdit(int nX,int nY,int nCX,CWnd *parent,UINT id);
	inline void SetTextColor(COLORREF colorFocus=RGB(0,0,0),COLORREF colorKillFocus=RGB(0,0,0)){m_colorFocus=colorFocus;m_colorKillFocus=colorKillFocus;}
	inline void SetBkColor(COLORREF color){m_colorBk=color;}
	inline void SetBordColor(COLORREF color){m_colorBord=color;}

private:
	CFont m_Font;//����
	bool m_bForcus;//����
	COLORREF m_colorFocus;//�н���ʱ������ɫ
	COLORREF m_colorKillFocus;//ʧȥ����ʱ������ɫ
	COLORREF m_colorBk;//������ɫ
	COLORREF m_colorBord;//�߿������ɫ

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};

class UI_CONTROL_API HEditWnd : public CWnd
{
	DECLARE_DYNCREATE(HEditWnd)
public:
	HEditWnd();
	virtual ~HEditWnd();

public:
	void GetString(CString &strWnd);
	void SetString(CString strText){m_strEdit=strText;}

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int PosFromChar(UINT nChar);		// ͨ��ָ�����ַ������x���λ��
	int CharFromPos(int nPosX);			// ͨ��������x���λ�û����Ӧ���ַ�
	void CutString();					// �����ַ���
	void PasteString();					// ճ���ַ���
	void CopyString();					// �����ַ���
	void MakeBKGround(CDC *pDC, COLORREF clrBegin, COLORREF clrEnd, CRect rect);	// ���ɴ��ڱ���
	CString m_strEdit;		// ��ǰ�������ı���
	CString m_strSelect;	// ��ǰѡ����ı���

	CPoint m_ptCursor;		// ��ǰ����λ�á�
	CPoint m_ptTxtBegin;	// �ı�����ʼλ�á�
	CPoint m_ptFirst;		// ����������ʱ�ĵ�һ��λ�á�
	CPoint m_ptSecond;		// ��굱ǰ��λ�á�

	int m_nCharPos;			// ��ǰ��괦�ڵڼ����ַ��ĺ��棬��0��ʼ������
	int m_nShowChar;		// �ڱ༭���е�һ����ʾ���ַ�����0��ʼ������
	int m_nMaxShowChars;	// �ڱ༭����һ�������ʾ���ַ�����	��
	int m_nSelectBegin;		// ѡ����ַ�����ʼλ�á�
	int m_nSelectEnd;		// ѡ����ַ�����ֹλ�á�

	CBitmap m_BmpBK;		// ����ͼƬ��

	CFont m_TxtFont;		// �ı������塣
	TEXTMETRIC m_tm;		// �ı��������Ϣ��

	CRect	m_rcWnd;		// ���ڵĴ�С��

	bool	m_bLBtDown;		// ���������¡�
	int		m_nShiftBegin;	// shift����һ��ʱ������ڵ��ַ���λ�á�
};

#endif