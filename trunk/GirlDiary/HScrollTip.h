#ifndef __HSCROLLTIP__H__
#define __HSCROLLTIP__H__
#pragma once

class HScrollTip : public CWnd
{
	DECLARE_DYNAMIC(HScrollTip)

public:
	HScrollTip();
	virtual ~HScrollTip();
	inline void SetScrollText(CString strText){m_strScroll=strText;}

protected:
	CString		m_strScroll;
	CFont		m_fontText;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#endif