
#include "stdafx.h"
#include "../resource.h"
#include "../include/HComboBox.h"


// HComboBox

IMPLEMENT_DYNAMIC(HComboBox, CComboBox)

HComboBox::HComboBox()
{
	m_Font.CreateFont(12,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,ANSI_CHARSET,3,2,1,2,_T("宋体"));
	m_clrTrans = RGB(255,0,255);
	m_bySpr   = 0;
	m_bIsClick = false;
}

HComboBox::~HComboBox()
{
	if (((HFONT)m_Font.m_hObject) != NULL)
	{
		m_Font.DeleteObject();
	}
}

BEGIN_MESSAGE_MAP(HComboBox, CComboBox)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_CONTROL_REFLECT(CBN_DROPDOWN, &HComboBox::OnCbnDropdown)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, &HComboBox::OnCbnSelchange)
	ON_CONTROL_REFLECT(CBN_SELENDCANCEL, &HComboBox::OnCbnSelendcancel)
	ON_WM_CREATE()
END_MESSAGE_MAP()

void HComboBox::SetComboBoxImage( CString strBk,CString strDown,COLORREF clrTrans )
{
	if (!m_ImgDown.IsNull()) m_ImgDown.Destroy();
	m_ImgDown.Load(strDown);
	
	if (!m_ImgBk.IsNull()) m_ImgBk.Destroy();
	m_ImgBk.Load(strBk);

	m_clrTrans = clrTrans;
}

void HComboBox::SetComboBoxImage( HINSTANCE hInstance,UINT nArrowID,UINT nBgID,COLORREF clrTrans)
{
	if (!m_ImgDown.IsNull())
		m_ImgDown.Destroy();
	m_ImgDown.LoadFromResource(hInstance,nArrowID);

	if (!m_ImgBk.IsNull())
		m_ImgBk.Destroy();
	m_ImgBk.LoadFromResource(hInstance,nBgID);

	m_clrTrans = clrTrans;
}

// HComboBox 消息处理程序
void HComboBox::OnPaint()
{
	CPaintDC dc(this); 
	CRect rect;
	GetClientRect(&rect);

	//绘制背景
	if (!m_ImgBk.IsNull())
	{
		m_ImgBk.StretchBlt(dc.m_hDC,rect.left,rect.top,rect.Width(),rect.Height()-1,
			0,0,m_ImgBk.GetWidth(),m_ImgBk.GetHeight(),SRCCOPY);
	}

	//绘制文字
	CString strText;
	GetWindowText(strText);
	if (!strText.IsEmpty())
	{
		dc.SetBkMode(TRANSPARENT);
		CFont *pOldFont = dc.SelectObject(&m_Font);
		dc.DrawText(strText,CRect(rect.left+2,rect.top+4,rect.Width()-2,rect.Height()-2),DT_LEFT);
		dc.SelectObject(pOldFont);
	}
	
	//判断箭头状态、绘制它
	if (!IsWindowEnabled())
	{
		m_bySpr = 3;
	}
	if (!m_ImgDown.IsNull())
	{
		m_ImgDown.TransparentBlt(dc.m_hDC,rect.right - m_ImgDown.GetWidth()/4+2,rect.top,m_ImgDown.GetWidth()/4-2,rect.Height()-2,
			0+m_bySpr*m_ImgDown.GetWidth()/4,0,m_ImgDown.GetWidth()/4,m_ImgDown.GetHeight(),m_clrTrans);
	}
}

BOOL HComboBox::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void HComboBox::OnCbnDropdown()
{
	m_bySpr = 1;
	m_bIsClick = true;
	Invalidate();
}

void HComboBox::OnCbnSelchange()
{
	if (m_bIsClick)
	{
		m_bIsClick = false;
	}
	m_bySpr = 0;
	Invalidate();
}

void HComboBox::OnCbnSelendcancel()
{
	if (m_bIsClick)
	{
		m_bIsClick = false;
	}
	m_bySpr = 0;
	Invalidate();
}

int HComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	//设置默认按钮图片
	SetComboBoxImage(GetModuleHandle(UI_CONTROL_DLL_NAME),IDB_COMBO_ARROW,IDB_COMBO_BG);
	this->SetFont(&m_Font);   
	return 0;
}

void HComboBox::PreSubclassWindow()
{
	SetComboBoxImage(GetModuleHandle(UI_CONTROL_DLL_NAME),IDB_COMBO_ARROW,IDB_COMBO_BG);
	CComboBox::PreSubclassWindow();
}
