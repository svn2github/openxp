#ifndef _HBTN_H_
#define _HBTN_H_
#pragma once
#include <atlimage.h>

//Ƥ����ť��
class UI_CONTROL_API HButton : public CButton
{
	DECLARE_DYNAMIC(HButton)

public:
	HButton();
	virtual ~HButton();
	void SetButtonIcon(HICON hIcon);//���ð�ť�ϵ�ͼƬ
	void SetButtonImage(CString strPath);//���ð�ť�ϵ�ͼƬ
	void SetButtonImage(UINT uBitmapId,HINSTANCE hInstance);//���ð�ť�ϵ�ͼƬ
	void SetButtonImage(CImage *pImage);//������CImage����
	void SetTextColor(COLORREF clr);//���ð�ť��������ɫ
	
private:
	HICON       m_hIcon;//��ťͼ��
	CImage      m_btnImg;//��ť��ͼƬ
	BOOL        m_bOver;//����Ƿ�����ƶ�
	COLORREF    m_textClr;//������ɫ
	CFont       m_textFont;//�������� 

protected:
	DECLARE_MESSAGE_MAP()
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

//Ƥ��CheckBox��ť��
class UI_CONTROL_API HCheckBox : public CButton
{
	DECLARE_DYNAMIC(HCheckBox)

private:
	enum enCheckBox{CB_NORMAL=0,CB_CHECK};	
	enCheckBox  m_enCbState;

public:
	HCheckBox();
	virtual ~HCheckBox();
	void SetButtonImage(UINT uBitmapId,HINSTANCE hInstance);//���ð�ť�ϵ�ͼƬ
	void SetTextColor(COLORREF clr);//���ð�ť��������ɫ
	enCheckBox GetCheckState(){return m_enCbState;}

private:
	CImage      m_btnImg;//��ť��ͼƬ
	CFont       m_textFont;//�������� 
	COLORREF    m_textClr;//������ɫ
	

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