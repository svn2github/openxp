#ifndef _HBTN_H_
#define _HBTN_H_
#pragma once
#include <atlimage.h>

//皮肤按钮类
class UI_CONTROL_API HButton : public CButton
{
	DECLARE_DYNAMIC(HButton)

public:
	HButton();
	virtual ~HButton();
	void SetButtonIcon(HICON hIcon);//设置按钮上的图片
	void SetButtonImage(CString strPath);//设置按钮上的图片
	void SetButtonImage(UINT uBitmapId,HINSTANCE hInstance);//设置按钮上的图片
	void SetButtonImage(CImage *pImage);//从其他CImage拷贝
	void SetTextColor(COLORREF clr);//设置按钮上字体颜色
	
private:
	HICON       m_hIcon;//按钮图标
	CImage      m_btnImg;//按钮上图片
	BOOL        m_bOver;//标记是否鼠标移动
	COLORREF    m_textClr;//字体颜色
	CFont       m_textFont;//文字字体 

protected:
	DECLARE_MESSAGE_MAP()
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

//皮肤CheckBox按钮类
class UI_CONTROL_API HCheckBox : public CButton
{
	DECLARE_DYNAMIC(HCheckBox)

private:
	enum enCheckBox{CB_NORMAL=0,CB_CHECK};	
	enCheckBox  m_enCbState;

public:
	HCheckBox();
	virtual ~HCheckBox();
	void SetButtonImage(UINT uBitmapId,HINSTANCE hInstance);//设置按钮上的图片
	void SetTextColor(COLORREF clr);//设置按钮上字体颜色
	enCheckBox GetCheckState(){return m_enCbState;}

private:
	CImage      m_btnImg;//按钮上图片
	CFont       m_textFont;//文字字体 
	COLORREF    m_textClr;//字体颜色
	

protected:
	DECLARE_MESSAGE_MAP()
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#endif