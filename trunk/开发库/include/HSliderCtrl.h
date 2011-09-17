#ifndef _QINSLIDE_H_
#define _QINSLIDE_H_
#pragma once
#include <atlimage.h>

class UI_CONTROL_API HSliderCtrl : public CSliderCtrl
{
	DECLARE_DYNAMIC(HSliderCtrl)

public:
	HSliderCtrl();
	BOOL CreateSkinSlide(BOOL bHor,CRect rect,CWnd *pParent);
	void SetSliderImage(LPCTSTR lpstBk);
	void SetSliderImage(HINSTANCE hInstance,UINT nID);
	virtual ~HSliderCtrl();

private:
	CImage m_imgBk;
	BYTE m_byThuState;
	enum
	{
		THUME_NORMAL=0,
		THUME_MOVE,
		THUME_DOWN,
	};

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#endif