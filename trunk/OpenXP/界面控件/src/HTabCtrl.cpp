#include "stdafx.h"
#include "../resource.h"
#include "../include/HTabCtrl.h"

#define ID_MOVE_TIMER_CHECK		100
#define GAP_DIS					1 
#define SHADE_LINE				3

IMPLEMENT_DYNAMIC(CHTabCtrl, CWnd)

CHTabCtrl::CHTabCtrl(): m_pParentWnd(NULL) ,m_CurTabHwnd(NULL)
{
	m_font.CreateFont(12,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,ANSI_CHARSET,3,2,1,2,TEXT("����"));
	m_nHeight = 10;
	m_nGap = 5;
	m_nCurPush = 0;
	m_nCurMove = -5;
	m_bkColor = RGB(164,229,241);
	m_bInital = false;
}

CHTabCtrl::~CHTabCtrl()
{
	if (((HFONT)m_font.m_hObject) != NULL)
		m_font.DeleteObject();
	if (!m_vecItem.empty()) m_vecItem.clear();
	if (!m_vecItemWnd.empty()) m_vecItemWnd.clear();	
}


BEGIN_MESSAGE_MAP(CHTabCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


int CHTabCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	//����Ĭ��TabͼƬ
	SetSkinImage(GetModuleHandle(UI_CONTROL_DLL_NAME),IDB_TAB);
	return 0;
}

//nGap �������
//nHeight  �߶�
void CHTabCtrl::CreateSkinTab( const RECT& rect, CWnd* pParentWnd,UINT nGap/*=5*/,UINT nHeight/*=10*/ )
{
	this->m_pParentWnd = pParentWnd;
	Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rect,pParentWnd,200);
	m_nHeight = nHeight;
	m_nGap = nGap;
}

//����Ƥ��ͼƬ
void CHTabCtrl::SetSkinImage( LPCTSTR lpstRes )
{
	if (!m_imgRes.IsNull())
		m_imgRes.Destroy();	
	m_imgRes.Load(lpstRes);
}

void CHTabCtrl::SetSkinImage( HINSTANCE hInstance,UINT nID )
{
	if (!m_imgRes.IsNull())
		m_imgRes.Destroy();
	m_imgRes.LoadFromResource(hInstance,nID);
}

//nWidth Tab��ȣ�ÿһ����Բ�һ����
void CHTabCtrl::InsertItem( LPCTSTR lpstTitle,UINT nWidth )
{
	ITEM item;
	item.lpstTitle = lpstTitle;
	item.nWidth = nWidth;
	m_vecItem.push_back(item);
}

//���ñ�����ɫ
void CHTabCtrl::SetBkColor( COLORREF color /*= RGB(129,197,240)*/ )
{
	m_bkColor = color;
}

//����ÿһ������Ĵ���
void CHTabCtrl::SetLinkWnd( UINT nIndex,CWnd *pWnd )
{
	ITEM_WND item_wnd;
	item_wnd.nIndex = nIndex;
	item_wnd.pWnd = pWnd;
	m_vecItemWnd.push_back(item_wnd);
}

//�õ���ߵĵ�
int CHTabCtrl::GetLeftPoint( int nCurIndex )
{
	int nLeft = 0;
	nLeft = 0;
	vector<ITEM>::iterator vecpItem = m_vecItem.begin();
	for (int j = 0; j < nCurIndex; ++j)
	{
		nLeft += vecpItem->nWidth;
		vecpItem++;
	}
	return nLeft;
}

//����
void CHTabCtrl::OnPaint()
{
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(&rect);
	
	//��˫����
	CImage		imgBuffer;
	CDC			memDC;
	imgBuffer.Create(rect.Width(), rect.Height(), 24);
	memDC.Attach(imgBuffer.GetDC());
	memDC.FillSolidRect(rect,m_bkColor);
	
	CFont *OldFont = memDC.SelectObject(&m_font);
	int i = 0;
	CRect rectPush;
	for (m_vecpItem = m_vecItem.begin(); m_vecpItem != m_vecItem.end(); ++m_vecpItem)
	{
		int nLeft = GetLeftPoint(i);
		CRect rect(GAP_DIS+nLeft+m_nGap,5,GAP_DIS+nLeft+m_vecpItem->nWidth,m_nHeight+5);
		int nSpr = 0; 
		if (m_nCurPush != -5)
		{
			if (m_nCurPush == i)
			{
				nSpr = m_imgRes.GetWidth()/3;
				rect.top -= 3;
				rectPush = rect;
			}
		}
		if (m_nCurMove != -5)
		{
			if (m_nCurMove == i && m_nCurPush != i)
			{
				nSpr = m_imgRes.GetWidth()/3*2;
			}
		}
		if (!m_imgRes.IsNull())
		{
			if (rect.Width() > 0  && !m_imgRes.IsNull())//Ԥ����
			{
				m_imgRes.Draw(memDC.GetSafeHdc(),rect.left,rect.top,rect.Width(),rect.Height(),
					0+nSpr,0,m_imgRes.GetWidth()/3,m_imgRes.GetHeight());
			}
		}
		memDC.SetBkMode(TRANSPARENT);
		memDC.SetTextColor(RGB(0,78,116));
		memDC.DrawText(m_vecpItem->lpstTitle,rect,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
		i++;
	}
	CPen pPen;
	pPen.CreatePen(PS_SOLID,1,RGB(0,140,66));
	CPen *OldPen = memDC.SelectObject(&pPen);

	//ѡ�еĿ��(ͷ���е�Բ��)
	BYTE nShade = SHADE_LINE;
	for (BYTE i = 0; i < SHADE_LINE; ++i)
	{
		memDC.MoveTo(rectPush.left+nShade,rectPush.top+i);
		memDC.LineTo(rectPush.right+1-nShade,rectPush.top+i);
		nShade--;
	}
	memDC.MoveTo(rectPush.left,rectPush.top+SHADE_LINE);
	memDC.LineTo(rectPush.left,rectPush.bottom);
	memDC.MoveTo(rectPush.right,rectPush.top+SHADE_LINE);
	memDC.LineTo(rectPush.right,rectPush.bottom);

	//��߽߱�
	memDC.MoveTo(0,m_nHeight+5);
	memDC.LineTo(0,rect.bottom);
	//��ߵ�ѡ��
	memDC.MoveTo(0,m_nHeight+5);
	memDC.LineTo(rectPush.left,m_nHeight+5);
	//ѡ�е��ұ�
	memDC.MoveTo(rectPush.right,m_nHeight+5);
	memDC.LineTo(rect.right-1,m_nHeight+5);
	//�ұߵ��ײ�
	memDC.MoveTo(rect.right-1,m_nHeight+5);
	memDC.LineTo(rect.right-1,rect.bottom-1);
	//�ײ���ߵ��ұ�
	memDC.MoveTo(0,rect.bottom-1);
	memDC.LineTo(rect.right-1,rect.bottom-1);

	//������Դ
	memDC.SelectObject(OldPen);
	pPen.DeleteObject();
	memDC.SelectObject(OldFont);

	//Ԥ����û�г�ʼ���õ�ǰѡ�е�״̬ʱ
	if (!m_bInital)
	{
		SetCurHot(m_nCurPush);
		m_bInital = true;
	}

	//�������
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.Detach();
	imgBuffer.ReleaseDC();
}

//�������
void CHTabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	int i = 0;
	for (m_vecpItem = m_vecItem.begin(); m_vecpItem != m_vecItem.end(); ++m_vecpItem)
	{
		int nLeft = GetLeftPoint(i);
		CRect rect(GAP_DIS+nLeft+m_nGap,5,GAP_DIS+nLeft+m_vecpItem->nWidth,m_nHeight+5);
		if (rect.PtInRect(point))
		{
			m_nCurPush = i;
			Invalidate();
			ShowLinkCWnd(m_nCurPush);
			OnBnChangeTab();
			return;
		}
		i++;
	}
	CWnd::OnLButtonDown(nFlags, point);
}

//����ƶ�
void CHTabCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	m_nCurMove = -5;
	int i = 0;
	for (m_vecpItem = m_vecItem.begin(); m_vecpItem != m_vecItem.end(); ++m_vecpItem)
	{
		int nLeft = GetLeftPoint(i);
		CRect rect(GAP_DIS+nLeft+m_nGap,5,GAP_DIS+nLeft+m_vecpItem->nWidth,m_nHeight+5);
		if (rect.PtInRect(point))
		{
			m_nCurMove = i;
			Invalidate();
			::SetTimer(this->GetSafeHwnd(),ID_MOVE_TIMER_CHECK,100,NULL);
			return;
		}
		i++;
	}
	CWnd::OnMouseMove(nFlags, point);
}

//��ʱ��
void CHTabCtrl::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == ID_MOVE_TIMER_CHECK)
	{
		int i = 0;
		POINT point;
		::GetCursorPos(&point);
		for (m_vecpItem = m_vecItem.begin(); m_vecpItem != m_vecItem.end(); ++m_vecpItem)
		{
			int nLeft = GetLeftPoint(i);
			CRect rect(GAP_DIS+nLeft+m_nGap,5,GAP_DIS+nLeft+m_vecpItem->nWidth,m_nHeight+5);
			ClientToScreen(&rect);
			if (rect.PtInRect(point))
			{
				return;
			}
			i++;
		}
		m_nCurMove = -5;
		Invalidate();
		KillTimer(ID_MOVE_TIMER_CHECK);
	}
	CWnd::OnTimer(nIDEvent);
}

//��������
BOOL CHTabCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

//���õ�ǰѡ�еĹ�������
void CHTabCtrl::SetCurHot( UINT nIndex /*= 0*/ )
{
	m_nCurPush = nIndex;
	ShowLinkCWnd(nIndex);
	m_bInital = true;
}

//��������Ĵ����Ƿ���Ҫ��ʾ
void CHTabCtrl::ShowLinkCWnd( UINT nCurIndex )
{
	if (m_vecItemWnd.empty()) return;
	for (m_vecpItemWnd = m_vecItemWnd.begin(); m_vecpItemWnd != m_vecItemWnd.end(); ++m_vecpItemWnd)
	{
		CWnd *pWnd = m_vecpItemWnd->pWnd;
		if (pWnd != NULL && ::IsWindow(pWnd->GetSafeHwnd()))
		{
			if (m_vecpItemWnd->nIndex == nCurIndex)
			{
				if (!pWnd->IsWindowVisible())
				{
					pWnd->ShowWindow(SW_SHOW);
					m_CurTabHwnd=pWnd->GetSafeHwnd();
				}
			}
			else 
			{
				pWnd->ShowWindow(SW_HIDE);
			}
		}
	}
}

//�õ�������ʾ�Ӵ��ڵľ���(��MS��,�Ǻ�)
bool CHTabCtrl::GetFrameRect( CRect &rect )
{
	CRect rectClient;
	GetClientRect(&rectClient);
	rect.left = 1;
	rect.top = m_nHeight + 5 + 1;
	rect.right = rectClient.right - 1;
	rect.bottom = rectClient.bottom - 1;
	if (rect.IsRectEmpty() || rect.IsRectNull()) return true;
	return false;
}