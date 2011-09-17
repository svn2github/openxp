#pragma once
#include <atlimage.h>

// HComboBox

class UI_CONTROL_API HComboBox : public CComboBox
{
	DECLARE_DYNAMIC(HComboBox)

public:
	HComboBox();
	virtual ~HComboBox();
	void SetComboBoxImage(CString strBk,CString strDown,COLORREF clrTrans=RGB(255,0,255));
	void SetComboBoxImage(HINSTANCE hInstance,UINT nArrowID,UINT nBgID,COLORREF clrTrans=RGB(255,0,255));

private:
	CFont	  m_Font;//字体
	CImage    m_ImgDown;//下拉箭头
	CImage    m_ImgBk;//背景
	COLORREF  m_clrTrans;//透明色 
	BYTE      m_bySpr;//精灵帧
	bool      m_bIsClick;//是否点击了下拉箭头

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnCbnDropdown();
	afx_msg void OnCbnSelchange();
	afx_msg void OnCbnSelendcancel();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void PreSubclassWindow();
};


