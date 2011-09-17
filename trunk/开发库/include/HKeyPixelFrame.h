#ifndef __KEYPIXELFRAME__H__
#define __KEYPIXELFRAME__H__
#pragma once
#include <atlimage.h>

class TEMPLATE_CONTROL_API HKeyPixelFrame : public CFrameWnd
{
public:
	HKeyPixelFrame();
	~HKeyPixelFrame();
	void SetPixelData(LPCTSTR lpszImage,COLORREF keyColor=RGB(255,0,255));
	void SetPixelData(HINSTANCE hInstance,UINT nID,COLORREF keyColor=RGB(255,0,255));
	virtual void OnRender(CDC *pDC);

protected:
	CImage   m_Image;//≤Œ’’Õº∆¨
	COLORREF m_colorKey; //πÿº¸—’…´

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#endif