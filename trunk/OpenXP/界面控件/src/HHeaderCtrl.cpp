//..
#include "stdafx.h"
#include "../include/HHeaderCtrl.h"

IMPLEMENT_DYNAMIC(HHeaderCtrl, CHeaderCtrl)

//���캯��
HHeaderCtrl::HHeaderCtrl()
{
	m_fHeight = 1;
	m_bLocked = FALSE;
	m_clrText = RGB(0,0,0);
	m_Font.CreateFont(12,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,ANSI_CHARSET,3,2,1,2,TEXT("����"));
	m_mapHotState.clear();
	m_bOver = FALSE;
}

//��������
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

//���뱳��ͼƬ
void HHeaderCtrl::LoadBKImage( LPCTSTR lpszPath )
{
	if (!m_imgBk.IsNull()) 
		m_imgBk.Destroy();
	m_imgBk.Load(lpszPath);
}

//���뱳��ͼƬ
void HHeaderCtrl::LoadBKImage( UINT nId,HINSTANCE hInstance )
{
	if (!m_imgBk.IsNull()) 
		m_imgBk.Destroy();
	m_imgBk.LoadFromResource(hInstance,nId);
}

//�����ı�
void HHeaderCtrl::AddString( LPCTSTR lpszColumnHeading )
{
	m_arrayText.Add(lpszColumnHeading);
}

//���ø߶�
void HHeaderCtrl::SetHeight( float nHeight )
{
	m_fHeight = nHeight;
}

//����yanse 
void HHeaderCtrl::SetTextColor( COLORREF color )
{
	m_clrText = color;
}

//����ʹ������
void HHeaderCtrl::SetUseFont( LOGFONT *pLogFont )
{
	m_Font.GetLogFont(pLogFont);
}

//���ù��
BOOL HHeaderCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg)
{
	return m_bLocked?TRUE:CHeaderCtrl::OnSetCursor(pWnd, nHitTest, msg);
}

//���ı�֪ͨ
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

//����
void HHeaderCtrl::OnPaint()
{
	CPaintDC dc(this);
	//��ȡλ��
	CRect rect;
	GetClientRect(&rect);
	//��˫����
	CImage		imgBuffer;
	CDC			dcBuffer;
	imgBuffer.Create(rect.Width(), rect.Height(), 24);
	dcBuffer.Attach(imgBuffer.GetDC());
	dcBuffer.FillSolidRect(0,0,rect.Width(),rect.Height(),RGB(241,244,248));
	
	
	int nItem = 0; 
	nItem = GetItemCount();//�õ��м�����Ԫ 
	for(int i = 0; i < nItem; i++) 
	{ 
		CRect tRect;
		GetItemRect(i,&tRect);//�õ�Item�ĳߴ�
		CRect nRect(tRect);//�����ߴ絽�µ������� 
		nRect.left++;//�����ָ��ߵĵط� 
		
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
		
		//������Դ
		dcBuffer.SelectObject(pOldFont);
	} 
	//��ͷ��ʣ�ಿ��
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

	//�������
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &dcBuffer, 0, 0, SRCCOPY);
	dcBuffer.Detach();
	imgBuffer.ReleaseDC();
}

//���ñ�ͷ�߶�
LRESULT HHeaderCtrl::OnLayout( WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam); 
	HD_LAYOUT &hdl = *( HD_LAYOUT * ) lParam; 
	RECT *prc = hdl.prc; 
	WINDOWPOS *pwpos = hdl.pwpos; 

	//��ͷ�߶�Ϊԭ��1.5�������Ҫ��̬�޸ı�ͷ�߶ȵĻ�����1.5���һ��ȫ�ֱ��� 
	int nHeight = (int)(pwpos->cy*m_fHeight);
	pwpos->cy = nHeight; 
	prc->top = nHeight; 
	return lResult; 
}

//����������
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

//����ƶ�
void HHeaderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bOver)//ע����Ϣ
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

//�������� 
BOOL HHeaderCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

//����ƿ�
LRESULT HHeaderCtrl::OnMouseLeave( WPARAM wParam, LPARAM lParam )
{
	m_bOver = FALSE;
	for(int i = 0; i < GetItemCount(); i++) 
		m_mapHotState[i] = FALSE;
	Invalidate();
	return 0;
}