// HButton.cpp : 实现文件
//

#include "stdafx.h"
#include "../resource.h"
#include "../include/HButton.h"

// HButton

IMPLEMENT_DYNAMIC(HButton, CButton)

//构造函数
HButton::HButton()
{
	m_bOver   = false;
	m_textClr = RGB(0,0,0);      
	m_textFont.CreateFont(12,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,ANSI_CHARSET,3,2,1,2,_T("宋体"));
}

//析构函数
HButton::~HButton()
{
	if (((HFONT)m_textFont.m_hObject) != NULL)
	{
		m_textFont.DeleteObject();
	}
}

BEGIN_MESSAGE_MAP(HButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
END_MESSAGE_MAP()

//设置按钮图标
void HButton::SetButtonIcon( HICON hIcon )
{
	m_hIcon = hIcon;
}

//设置按钮图片
void HButton::SetButtonImage( CString strPath )
{
	if (!m_btnImg.IsNull())
		m_btnImg.Destroy();
	m_btnImg.Load(strPath);
}

//设置按钮图片
void HButton::SetButtonImage( UINT uBitmapId,HINSTANCE hInstance )
{
	if (!m_btnImg.IsNull())
		m_btnImg.Destroy();
	m_btnImg.LoadFromResource(hInstance,uBitmapId);
}

//设置按钮图片
void HButton::SetButtonImage( CImage *pImage )
{
	if (!m_btnImg.IsNull())
		m_btnImg.Destroy();
	ASSERT(pImage!=NULL);
	m_btnImg.Attach(pImage->Detach());
}

//设置文字颜色
void HButton::SetTextColor( COLORREF color )
{
	m_textClr = color;
}

//创建控件
int HButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	//设置默认按钮图片
	SetButtonImage(IDB_BUTTON,GetModuleHandle(UI_CONTROL_DLL_NAME));
	return 0;
}

//控件子类化
void HButton::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW);
	//设置默认按钮图片
	SetButtonImage(IDB_BUTTON,GetModuleHandle(UI_CONTROL_DLL_NAME));
	CButton::PreSubclassWindow();
}

//鼠标移动
void HButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bOver)//注册消息
	{
		TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, 0};
		TrackMouseEvent(&tme);
		m_bOver = TRUE;
		Invalidate();
	}
	CButton::OnMouseMove(nFlags, point);
}

//鼠标移开
LRESULT HButton::OnMouseLeave( WPARAM wParam, LPARAM lParam )
{
	m_bOver = FALSE;
	Invalidate();
	return 0;
}

//绘制
void HButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	RECT rc;
	GetClientRect(&rc);

	//计算目标位置
	BYTE byDestX;//目标区域X
	BYTE byDestY;//目标区域Y
	if (lpDrawItemStruct->itemState & ODS_DISABLED)
	{
		byDestX = (m_btnImg.GetWidth()/4 * 3);
		byDestY = 0; 
	}
	else if (lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		byDestX = (m_btnImg.GetWidth()/4 * 2);
		byDestY = 0; 
	}
	else
	{
		if (m_bOver)
		{
			byDestX = m_btnImg.GetWidth()/4;
			byDestY = 0; 
		}
		else
		{
			byDestX = 0;
			byDestY = 0; 
		}
	}

	//绘制图像
	if (!m_btnImg.IsNull())
	{
		m_btnImg.Draw(pDC->GetSafeHdc(),rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, 
			byDestX, byDestY,m_btnImg.GetWidth()/4, m_btnImg.GetHeight());
	}

	//绘制文字
	CString str;
	GetWindowText(str);
	if (!str.IsEmpty())
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_textClr);
		if (byDestX == (m_btnImg.GetWidth()/4 * 3))
		{
			pDC->SetTextColor(RGB(124,124,124));
		}
		CFont *pOldFont = pDC->SelectObject(&m_textFont);
		DrawText(pDC->GetSafeHdc(),str,-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		pDC->SelectObject(pOldFont);
	}
}

//擦出背景
BOOL HButton::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}





//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(HCheckBox, CButton)
HCheckBox::HCheckBox()
{
	m_textFont.CreateFont(12,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,ANSI_CHARSET,3,2,1,2,_T("宋体"));
	m_enCbState = CB_NORMAL;
	m_textClr = RGB(0,0,0);      
}

HCheckBox::~HCheckBox()
{
	if (((HFONT)m_textFont.m_hObject) != NULL)
	{
		m_textFont.DeleteObject();
	}
}	

BEGIN_MESSAGE_MAP(HCheckBox, CButton)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//设置按钮图片
void HCheckBox::SetButtonImage( UINT uBitmapId,HINSTANCE hInstance )
{
	if (!m_btnImg.IsNull())
		m_btnImg.Destroy();
	m_btnImg.LoadFromResource(hInstance,uBitmapId);
}

//设置文字颜色
void HCheckBox::SetTextColor( COLORREF clr )
{
	m_textClr = clr;
}

//鼠标单击
void HCheckBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	CButton::OnLButtonDown(nFlags, point);
	if (!IsWindowEnabled()) return;
	if (m_enCbState == CB_NORMAL) m_enCbState = CB_CHECK;
	else m_enCbState = CB_NORMAL;
	Invalidate();
}

//创建控件
int HCheckBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	//设置默认按钮图片
	SetButtonImage(IDB_CHECKBOX,GetModuleHandle(UI_CONTROL_DLL_NAME));
	return 0;
}

//控件子类化
void HCheckBox::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW);
	//设置默认按钮图片
	SetButtonImage(IDB_CHECKBOX,GetModuleHandle(UI_CONTROL_DLL_NAME));
	CButton::PreSubclassWindow();
}

//绘制
void HCheckBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	RECT rc;
	GetClientRect(&rc);

	COLORREF textColor = m_textClr;
	//计算文字颜色
	if (!IsWindowEnabled())
		textColor = RGB(124,124,124);

	//绘制图像
	if (!m_btnImg.IsNull())
		m_btnImg.TransparentBlt(pDC->GetSafeHdc(),rc.left,rc.top-2,m_btnImg.GetWidth()/2,rc.bottom-rc.top
		,0+m_enCbState*(m_btnImg.GetWidth()/2),0,m_btnImg.GetWidth()/2,m_btnImg.GetHeight(),RGB(255,0,255));

	//绘制文字
	CString str;
	GetWindowText(str);
	if (!str.IsEmpty())
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(textColor);
		CFont *pOldFont = pDC->SelectObject(&m_textFont);
		if (!m_btnImg.IsNull())
			rc.left += m_btnImg.GetWidth()/2;
		DrawText(pDC->GetSafeHdc(),str,-1,&rc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		pDC->SelectObject(pOldFont);
	}
}

//擦出背景
BOOL HCheckBox::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}