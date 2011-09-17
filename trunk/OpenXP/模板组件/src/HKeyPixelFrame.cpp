#include "StdAfx.h"
#include "../resource.h"
#include "..\include\HKeyPixelFrame.h"

void ShapeWindowFromBitmap(CImage &image,CWnd *pWnd,COLORREF keyColor)
{
	pWnd->MoveWindow(0,0,image.GetWidth(),image.GetHeight());
	if (!image.IsNull())
	{
		CRgn crRgn, crRgnTmp;
		crRgn.CreateRectRgn(0,0,0,0);
		for (int nY = 0; nY <= image.GetHeight(); nY++)
		{
			int nX = 0;
			do
			{
				while (nX <= image.GetWidth() && image.GetPixel(nX, nY) == keyColor)
					nX++;
				int iLeftX = nX;
				while (nX <= image.GetWidth() && image.GetPixel(nX, nY) != keyColor)
					++nX;
				crRgnTmp.CreateRectRgn(iLeftX, nY, nX, nY+1);
				crRgn.CombineRgn(&crRgn, &crRgnTmp, RGN_OR);
				crRgnTmp.DeleteObject();
			}while(nX < image.GetWidth());
			nX = 0;
		}
		pWnd->SetWindowRgn(crRgn,TRUE);
		crRgn.DeleteObject();
	}
}

HKeyPixelFrame::HKeyPixelFrame()
{
	m_colorKey = RGB(255,0,255);
}

HKeyPixelFrame::~HKeyPixelFrame()
{
}

BEGIN_MESSAGE_MAP(HKeyPixelFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

int HKeyPixelFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	ModifyStyle(WS_CAPTION|WS_BORDER|WS_DLGFRAME|WS_THICKFRAME,0,0);
	//…Ë÷√ƒ¨»œ∞¥≈•Õº∆¨
	SetPixelData(GetModuleHandle(TEMPLATE_CONTROL_DLL_NAME),IDB_PIXCEL_FRAME,RGB(0,0,0));	
	return 0;
}

void HKeyPixelFrame::SetPixelData( LPCTSTR lpszImage,COLORREF keyColor )
{
	if (!m_Image.IsNull())
		m_Image.Destroy();
	m_Image.Load(lpszImage);
	m_colorKey = keyColor;
	ShapeWindowFromBitmap(m_Image,this,m_colorKey);
}

void HKeyPixelFrame::SetPixelData( HINSTANCE hInstance,UINT nID,COLORREF keyColor)
{
	if (!m_Image.IsNull())
		m_Image.Destroy();
	m_Image.LoadFromResource(hInstance,nID);
	m_colorKey = keyColor;
	ShapeWindowFromBitmap(m_Image,this,m_colorKey);
}

void HKeyPixelFrame::OnPaint()
{
	CPaintDC dc(this); 
	CRect rect;
	GetClientRect(&rect);
	HMemDC memDC(&dc,rect);
	m_Image.Draw(memDC.GetSafeHdc(),0,0);
	OnRender(memDC);
}

void HKeyPixelFrame::OnLButtonDown(UINT nFlags, CPoint point)
{
	CFrameWnd::OnLButtonDown(nFlags, point);
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
}

void HKeyPixelFrame::OnRender( CDC *pDC )
{

}