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
	CFont m_Font;//字体
	bool m_bForcus;//焦点
	COLORREF m_colorFocus;//有焦点时字体颜色
	COLORREF m_colorKillFocus;//失去焦点时字体颜色
	COLORREF m_colorBk;//背景颜色
	COLORREF m_colorBord;//边框矩形颜色

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
	int PosFromChar(UINT nChar);		// 通过指定的字符获得其x轴的位置
	int CharFromPos(int nPosX);			// 通过给定的x轴的位置获得相应的字符
	void CutString();					// 剪切字符串
	void PasteString();					// 粘贴字符串
	void CopyString();					// 拷贝字符串
	void MakeBKGround(CDC *pDC, COLORREF clrBegin, COLORREF clrEnd, CRect rect);	// 生成窗口背景
	CString m_strEdit;		// 当前包含的文本。
	CString m_strSelect;	// 当前选择的文本。

	CPoint m_ptCursor;		// 当前光标的位置。
	CPoint m_ptTxtBegin;	// 文本的起始位置。
	CPoint m_ptFirst;		// 鼠标左键按下时的第一点位置。
	CPoint m_ptSecond;		// 鼠标当前的位置。

	int m_nCharPos;			// 当前光标处于第几个字符的后面，从0开始计数。
	int m_nShowChar;		// 在编辑框中第一个显示的字符，从0开始计数。
	int m_nMaxShowChars;	// 在编辑框中一次最多显示的字符数量	。
	int m_nSelectBegin;		// 选择的字符的起始位置。
	int m_nSelectEnd;		// 选择的字符的终止位置。

	CBitmap m_BmpBK;		// 背景图片。

	CFont m_TxtFont;		// 文本的字体。
	TEXTMETRIC m_tm;		// 文本字体的信息。

	CRect	m_rcWnd;		// 窗口的大小。

	bool	m_bLBtDown;		// 鼠标左键按下。
	int		m_nShiftBegin;	// shift键第一次时光标所在的字符的位置。
};

#endif