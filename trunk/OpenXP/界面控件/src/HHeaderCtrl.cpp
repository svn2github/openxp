//..
#include "stdafx.h"
#include "../include/HHeaderCtrl.h"

IMPLEMENT_DYNAMIC(HHeaderCtrl, CHeaderCtrl)

//构造函数
HHeaderCtrl::HHeaderCtrl()
{
	m_fHeight = 1;
	m_bLocked = FALSE;
	m_clrText = RGB(0,0,0);
	m_Font.CreateFont(12,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,ANSI_CHARSET,3,2,1,2,TEXT("宋体"));
	m_mapHotState.clear();
	m_bOver = FALSE;
}

//析构函数
HHeaderCtrl::~HHeaderCtrl()
{
	if (((HFONT)m_Font.m_hObject) != NULL)
		m_Font.DeleteObject();
	m_mapHotState.clear();
}

BEGIN_MESSAGE_MAP(HHeaderCtrl, CHeaderCtrl)
	ON_WM_PAINT()
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

//载入背景图片
void HHeaderCtrl::LoadBKImage( LPCTSTR lpszPath )
{
	if (!m_imgBk.IsNull()) 
		m_imgBk.Destroy();
	m_imgBk.Load(lpszPath);
}

//载入背景图片
void HHeaderCtrl::LoadBKImage( UINT nId,HINSTANCE hInstance )
{
	if (!m_imgBk.IsNull()) 
		m_imgBk.Destroy();
	m_imgBk.LoadFromResource(hInstance,nId);
}

//增加文本
void HHeaderCtrl::AddString( LPCTSTR lpszColumnHeading )
{
	m_arrayText.Add(lpszColumnHeading);
}

//设置高度
void HHeaderCtrl::SetHeight( float nHeight )
{
	m_fHeight = nHeight;
}

//设置yanse 
void HHeaderCtrl::SetTextColor( COLORREF color )
{
	m_clrText = color;
}

//设置使用字体
void HHeaderCtrl::SetUseFont( LOGFONT *pLogFont )
{
	m_Font.GetLogFont(pLogFont);
}

//设置光标
BOOL HHeaderCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg)
{
	return m_bLocked?TRUE:CHeaderCtrl::OnSetCursor(pWnd, nHitTest, msg);
}

//光标改变通知
BOOL HHeaderCtrl::OnChildNotify(UINT msg, WPARAM wp, LPARAM lp, LRESULT* pRes)
{
#pragma warning(disable:4244) 
#pragma warning(push)
	NMHDR& nmh = *(NMHDR*)lp;
	if (nmh.code == HDN_BEGINTRACKA || nmh.code == HDN_BEGINTRACKW) 
	{
		if (m_bLocked)
			return *pRes=TRUE; 
	}
#pragma warning(pop)
	return CHeaderCtrl::OnChildNotify(msg, wp, lp, pRes);
}

//绘制
void HHeaderCtrl::OnPaint()
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
	dcBuffer.FillSolidRect(0,0,rect.Width(),rect.Height(),RGB(241,244,248));
	
	
	int nItem = 0; 
	nItem = GetItemCount();//得到有几个单元 
	for(int i = 0; i < nItem; i++) 
	{ 
		CRect tRect;
		GetItemRect(i,&tRect);//得到Item的尺寸
		CRect nRect(tRect);//拷贝尺寸到新的容器中 
		nRect.left++;//留出分割线的地方 
		
		if (nRect.Width() > 0 && !m_imgBk.IsNull())
		{
			m_imgBk.Draw(dcBuffer.GetSafeHdc(),nRect.left,nRect.top,nRect.Width(),nRect.Height(),
				0+m_imgBk.GetWidth()/2*m_mapHotState[i],0,m_imgBk.GetWidth()/2,m_imgBk.GetHeight());
		}
		dcBuffer.SetBkMode(TRANSPARENT);
		CFont *pOldFont = dcBuffer.SelectObject(&m_Font);
		TEXTMETRIC metric;
		dcBuffer.GetTextMetrics(&metric);
		int ofst;
		ofst = nRect.Height() - metric.tmHeight;
		nRect.OffsetRect(0,ofst/2);
		dcBuffer.SetTextColor(m_clrText);
		dcBuffer.DrawText(m_arrayText[i],&nRect,DT_CENTER|DT_END_ELLIPSIS);
		
		//清理资源
		dcBuffer.SelectObject(pOldFont);
	} 
	//画头部剩余部分
	CRect rtRect;
	CRect clientRect;
	GetItemRect(nItem - 1,rtRect);
	GetClientRect(clientRect);
	rtRect.left = rtRect.right+1;
	rtRect.right = clientRect.right;
	CRect nRect(rtRect);
	for(int j = rtRect.top; j <= rtRect.bottom; j++) 
	{ 
		nRect.bottom = nRect.top+1; 
		dcBuffer.FillSolidRect(nRect,RGB(255,255,255));
		nRect.top = nRect.bottom; 
	} 

	//清除缓冲
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &dcBuffer, 0, 0, SRCCOPY);
	dcBuffer.Detach();
	imgBuffer.ReleaseDC();
}

//设置表头高度
LRESULT HHeaderCtrl::OnLayout( WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam); 
	HD_LAYOUT &hdl = *( HD_LAYOUT * ) lParam; 
	RECT *prc = hdl.prc; 
	WINDOWPOS *pwpos = hdl.pwpos; 

	//表头高度为原来1.5倍，如果要动态修改表头高度的话，将1.5设成一个全局变量 
	int nHeight = (int)(pwpos->cy*m_fHeight);
	pwpos->cy = nHeight; 
	prc->top = nHeight; 
	return lResult; 
}

//鼠标左键按下
void HHeaderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	/*for(int i = 0; i < GetItemCount(); i++) 
	{ 
		m_mapHotState[i] = FALSE;
		CRect rect;
		GetItemRect(i,&rect);
		if (rect.PtInRect(point))
		{
			m_mapHotState[i] = TRUE;
		}
	}
	Invalidate();*/
	CHeaderCtrl::OnLButtonDown(nFlags, point);
}

//鼠标移动
void HHeaderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bOver)//注册消息
	{
		TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, 0};
		TrackMouseEvent(&tme);
		m_bOver = TRUE;
	}
	for(int i = 0; i < GetItemCount(); i++) 
	{ 
		m_mapHotState[i] = FALSE;
		CRect rect;
		GetItemRect(i,&rect);
		if (rect.PtInRect(point))
		{
			m_mapHotState[i] = TRUE;
		}
	}
	Invalidate();
	CHeaderCtrl::OnMouseMove(nFlags, point);
}

//擦出背景 
BOOL HHeaderCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

//鼠标移开
LRESULT HHeaderCtrl::OnMouseLeave( WPARAM wParam, LPARAM lParam )
{
	m_bOver = FALSE;
	for(int i = 0; i < GetItemCount(); i++) 
		m_mapHotState[i] = FALSE;
	Invalidate();
	return 0;
}