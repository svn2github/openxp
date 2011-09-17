#ifndef __HHEADERCTRL__H__
#define __HHEADERCTRL__H__
#pragma once
#include <atlimage.h>
#include <map>

class UI_CONTROL_API HHeaderCtrl : public CHeaderCtrl
{
	DECLARE_DYNAMIC(HHeaderCtrl)

public:
	HHeaderCtrl();
	virtual ~HHeaderCtrl();

	//控制函数
public:
	void LoadBKImage(LPCTSTR lpszPath);
	void LoadBKImage(UINT nId,HINSTANCE hInstance);
	inline void AddString(LPCTSTR lpszColumnHeading);
	inline void SetHeight(float fHeight);
	float GetHeight()const{return m_fHeight;}
	inline void SetTextColor(COLORREF clr);
	inline void SetUseFont(LOGFONT *pLogFont);
	inline void Lock(BOOL bLock){m_bLocked = bLock;}
	BOOL IsLocked()const{return m_bLocked;}	

	//成员变量
private:
	BOOL				m_bOver;//标记是否鼠标移动
	BOOL				m_bLocked;
	CImage				m_imgBk;
	CStringArray		m_arrayText;
	float				m_fHeight;
	COLORREF			m_clrText;
	CFont				m_Font;
	typedef std::map<int,BOOL> MapHotState;
	MapHotState			m_mapHotState;

	//消息映射
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	virtual BOOL OnChildNotify(UINT msg, WPARAM wp, LPARAM lp, LRESULT* pRes);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	LRESULT OnLayout( WPARAM wParam, LPARAM lParam );
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
};

#endif