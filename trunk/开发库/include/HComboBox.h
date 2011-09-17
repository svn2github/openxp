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
	CFont	  m_Font;//����
	CImage    m_ImgDown;//������ͷ
	CImage    m_ImgBk;//����
	COLORREF  m_clrTrans;//͸��ɫ 
	BYTE      m_bySpr;//����֡
	bool      m_bIsClick;//�Ƿ�����������ͷ

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


