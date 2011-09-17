#include "stdafx.h"
#include "../resource.h"
#include "../include/HSliderCtrl.h"

// HSliderCtrl

IMPLEMENT_DYNAMIC(HSliderCtrl, CSliderCtrl)

HSliderCtrl::HSliderCtrl()
{
	m_byThuState = THUME_NORMAL;
}

HSliderCtrl::~HSliderCtrl()
{
}


BEGIN_MESSAGE_MAP(HSliderCtrl, CSliderCtrl)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
END_MESSAGE_MAP()



// HSliderCtrl 消息处理程序
BOOL HSliderCtrl::CreateSkinSlide( BOOL bHor,CRect rect,CWnd *pParent )
{
	if (bHor)
	{
		if (!CSliderCtrl::Create(WS_CHILD|WS_VISIBLE|TBS_HORZ|TBS_NOTICKS|TBS_BOTH,rect,pParent,189))
		{
			return FALSE;
		}
	}
	else 
	{
		if (!CSliderCtrl::Create(WS_CHILD|WS_VISIBLE|TBS_VERT|TBS_NOTICKS|TBS_BOTH,rect,pParent,189))
		{
			return FALSE;
		}
	}
	return TRUE;
}

void HSliderCtrl::SetSliderImage( LPCTSTR lpstBk )
{	
	if (!m_imgBk.IsNull())
		m_imgBk.Destroy();
	m_imgBk.Load(lpstBk);
}

void HSliderCtrl::SetSliderImage( HINSTANCE hInstance,UINT nID )
{
	if (!m_imgBk.IsNull())
		m_imgBk.Destroy();
	m_imgBk.LoadFromResource(hInstance,nID);
}

void HSliderCtrl::OnPaint()
{
	CPaintDC dc(this);
	//获取位置
	CRect rect;
	GetClientRect(&rect);
	//建双缓冲
	CImage		imgBuffer;
	CDC			dcBuffer;
	imgBuffer.Create(rect.Width(), rect.Height(), 24);
	dcBuffer.Attach(imgBuffer.GetDC());

	if (!m_imgBk.IsNull())
	{
		m_imgBk.TransparentBlt(dcBuffer.GetSafeHdc(),0,0,rect.Width(),rect.Height(),
			0,0,m_imgBk.GetWidth(),m_imgBk.GetHeight()/5,RGB(255,0,255));
		CRect thuRect;
		GetThumbRect(&thuRect);
		m_imgBk.TransparentBlt(dcBuffer.GetSafeHdc(),thuRect.left,thuRect.top,thuRect.Width(),thuRect.Height(),
			0,0+m_imgBk.GetHeight()/5*(m_byThuState+2),11,m_imgBk.GetHeight()/5,RGB(255,0,255));
		m_imgBk.TransparentBlt(dcBuffer.GetSafeHdc(),0,0,thuRect.left,rect.Height(),
			0,0+m_imgBk.GetHeight()/5,thuRect.left,m_imgBk.GetHeight()/5,RGB(255,0,255));
	}

	//清除缓冲
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &dcBuffer, 0, 0, SRCCOPY);
	dcBuffer.Detach();
	imgBuffer.ReleaseDC();
}

void HSliderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	Invalidate();
	CSliderCtrl::OnMouseMove(nFlags, point);
}

void HSliderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect thuRect;
	GetThumbRect(&thuRect);
	if (thuRect.PtInRect(point))
	{
		m_byThuState = THUME_DOWN;
		Invalidate();
	}
	CSliderCtrl::OnLButtonDown(nFlags, point);
}

void HSliderCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_byThuState == THUME_DOWN)
	{
		m_byThuState = THUME_NORMAL;
		Invalidate();
	}
	CSliderCtrl::OnLButtonUp(nFlags, point);
}

BOOL HSliderCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

int HSliderCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CSliderCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	this->SetSliderImage(GetModuleHandle(UI_CONTROL_DLL_NAME),IDB_SLIDER);
	return 0;
}
