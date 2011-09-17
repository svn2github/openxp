// 
#include "stdafx.h"
#include "../resource.h"
#include "../include/HListCtrl.h"

// HListCtrl

IMPLEMENT_DYNAMIC(HListCtrl, CListCtrl)

//构造函数
HListCtrl::HListCtrl()
{
	 m_nRowHeight = 0;
	 m_color = RGB(0,0,0);
	 m_nCurHighlighted = -5;
	 m_bCheckBox = false;
}

//析构函数
HListCtrl::~HListCtrl()
{
	m_mapCheckMgr.clear();
}


BEGIN_MESSAGE_MAP(HListCtrl, CListCtrl)
	ON_WM_MEASUREITEM()
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_CREATE()
END_MESSAGE_MAP()


int HListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	ModifyStyle(0,LVS_OWNERDRAWFIXED);
	CHeaderCtrl *pHeader = GetHeaderCtrl();
	m_Header.SubclassWindow(pHeader->GetSafeHwnd());
	//设置默认图片
	LoadResource(GetModuleHandle(UI_CONTROL_DLL_NAME),IDB_HEADERCTRL,IDB_LISTCTRL);
	return 0;
}

//控件子类化
void HListCtrl::PreSubclassWindow()
{
	ModifyStyle(0,LVS_OWNERDRAWFIXED);
	CListCtrl::PreSubclassWindow();
	CHeaderCtrl *pHeader = GetHeaderCtrl();
	m_Header.SubclassWindow(pHeader->GetSafeHwnd());
	//设置默认图片
	LoadResource(GetModuleHandle(UI_CONTROL_DLL_NAME),IDB_HEADERCTRL,IDB_LISTCTRL);
}

//载入资源
void HListCtrl::LoadResource( LPCTSTR lpstHeader,LPCTSTR lpstBk,LPCTSTR lpstCheckBox,LPCTSTR lpstSelected )
{
	m_Header.LoadBKImage(lpstHeader);
	if (!m_imgBk.IsNull())
		m_imgBk.Destroy();
	m_imgBk.Load(lpstBk);
	if (lpstCheckBox != NULL)
	{
		if (!m_imgSeleted.IsNull())
			m_imgSeleted.Destroy();
		m_imgSeleted.Load(lpstSelected);
	}
	if (lpstCheckBox != NULL)
	{
		if (!m_imgCheckBox.IsNull())
			m_imgCheckBox.Destroy();
		m_imgCheckBox.Load(lpstCheckBox);
		m_bCheckBox = true;
	}
}

//设置图像资源
void HListCtrl::LoadResource( HINSTANCE hInstance,UINT nHeader,UINT nBk,UINT nCheckBox,UINT nSelected )
{
	m_Header.LoadBKImage(nHeader,hInstance);
	if (!m_imgBk.IsNull())
		m_imgBk.Destroy();
	m_imgBk.LoadFromResource(hInstance,nBk);
	if (nSelected != 0)
	{
		if (!m_imgSeleted.IsNull())
			m_imgSeleted.Destroy();
		m_imgSeleted.LoadFromResource(hInstance,nSelected);
	}
	if (nCheckBox != 0)
	{
		if (!m_imgCheckBox.IsNull())
			m_imgCheckBox.Destroy();
		m_imgCheckBox.LoadFromResource(hInstance,nCheckBox);
		m_bCheckBox = true;
	}
}

//设置头部字体颜色
void HListCtrl::SetHeaderTextColor( COLORREF color )
{
	m_Header.SetTextColor(color);
}

//设置列表字体颜色
void HListCtrl::SetTextColor( COLORREF color )
{
	m_color = color;
}

//绘制
void HListCtrl::OnPaint()
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

	CRect headerRect;
	GetDlgItem(0)->GetWindowRect(&headerRect);
	ScreenToClient(&headerRect);
	dc.ExcludeClipRect(&headerRect);
	CRect clip;
	dcBuffer.GetClipBox(&clip);
	dcBuffer.FillSolidRect(clip, RGB(241,244,248));
	SetTextBkColor(RGB(241,244,248));
	for (BYTE i = 0; i < 100; ++i)
	{
		BYTE j = 0;
		if (i % 2 == 0 && i > 0)
			j = 1;
		if (rect.Width() > 0  && !m_imgBk.IsNull())
		{
			m_imgBk.Draw(dcBuffer.GetSafeHdc(),0,-7+m_nRowHeight*i,rect.Width(),m_nRowHeight,
				0+j*m_imgBk.GetWidth()/2,0,m_imgBk.GetWidth()/2,m_imgBk.GetHeight());
		}
	}
	DefWindowProc(WM_PAINT, (WPARAM)dcBuffer.m_hDC, (LPARAM)0);

	//清除缓冲
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &dcBuffer, 0, 0, SRCCOPY);
	dcBuffer.Detach();
	imgBuffer.ReleaseDC();
}

//擦出背景
BOOL HListCtrl::OnEraseBkgnd(CDC* pDC)
{
	return CListCtrl::OnEraseBkgnd(pDC);
}

//绘制单元格
void HListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{ 
	TCHAR lpBuffer[256];
	LV_ITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_PARAM ;
	lvi.iItem = lpDrawItemStruct->itemID ; 
	lvi.iSubItem = 0;
	lvi.pszText = lpBuffer ;
	lvi.cchTextMax = sizeof(lpBuffer);
	VERIFY(GetItem(&lvi));

	LV_COLUMN lvc, lvcprev ;
	::ZeroMemory(&lvc, sizeof(lvc));
	::ZeroMemory(&lvcprev, sizeof(lvcprev));
	lvc.mask = LVCF_WIDTH | LVCF_FMT;
	lvcprev.mask = LVCF_WIDTH | LVCF_FMT;

	CDC* pDC;
	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rtClient;
	GetClientRect(&rtClient);
	for (int nCol = 0; GetColumn(nCol, &lvc); nCol++)
	{
		if (nCol > 0) 
		{
			GetColumn(nCol-1, &lvcprev) ;
			lpDrawItemStruct->rcItem.left += lvcprev.cx ;
			lpDrawItemStruct->rcItem.right += lpDrawItemStruct->rcItem.left; 
		}

		CRect rcItem;   
		if (!GetSubItemRect(lpDrawItemStruct->itemID,nCol,LVIR_LABEL,rcItem))   
			continue;   

		::ZeroMemory(&lvi, sizeof(lvi));
		lvi.iItem = lpDrawItemStruct->itemID;
		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		lvi.iSubItem = nCol;
		lvi.pszText = lpBuffer;
		lvi.cchTextMax = sizeof(lpBuffer);
		VERIFY(GetItem(&lvi));
		CRect rcTemp;
		rcTemp = rcItem;

		BYTE byBkSpr = 0;
		if (lpDrawItemStruct->itemID %2 != 0)
		{
			byBkSpr = 1;
		}
		if (rcItem.Width() > 0  && !m_imgBk.IsNull())
		{
			m_imgBk.Draw(pDC->GetSafeHdc(),
				rcItem.left,rcItem.top,rcItem.Width(),rcItem.Height(),
				0+byBkSpr*m_imgBk.GetWidth()/2,0,m_imgBk.GetWidth()/2,m_imgBk.GetHeight());
		}

		if ((lpDrawItemStruct->itemID == m_nCurHighlighted) && (m_nCurHighlighted != -5))
		{
			if (!m_imgSeleted.IsNull())
			{
				m_imgSeleted.Draw(pDC->GetSafeHdc(),rcItem.left,rcItem.top,rcItem.Width(),rcItem.Height(),
					0,0,m_imgBk.GetWidth(),m_imgBk.GetHeight());
			}
			else 
			{
				pDC->FillSolidRect(rcItem,RGB(30,158,201));
			}
		}

		TEXTMETRIC metric;
		pDC->GetTextMetrics(&metric);
		int ofst;
		ofst = rcItem.Height() - metric.tmHeight;
		rcItem.OffsetRect(0,ofst/2);
		pDC->SetTextColor(m_color);
		DWORD dwStyle = DT_LEFT;
		if (nCol == 0)//第一列
		{
			if (m_bCheckBox)
			{
				BYTE bySpr = 0;
				if (!m_mapCheckMgr[lpDrawItemStruct->itemID])
				{
					bySpr = 1;
				}
				//绘制checkbox
				CRect rect = rcItem;
				rect.left += 3;
				rect.top -= 3;
				if (!m_imgCheckBox.IsNull())
				{
					m_imgCheckBox.Draw(pDC->GetSafeHdc(),rect.left,rect.top,m_imgCheckBox.GetWidth()/4,m_imgCheckBox.GetHeight(),
						0+bySpr*m_imgCheckBox.GetWidth()/4,0,m_imgCheckBox.GetWidth()/4,m_imgCheckBox.GetHeight());
				}
				//计算数据
				rcItem.left += m_imgCheckBox.GetWidth()/4+rect.left;
			}
		}
		if ((lpDrawItemStruct->itemID == m_nCurHighlighted) && (m_nCurHighlighted != -5))
		{
			pDC->SetTextColor(RGB(255,255,255));
		}
		DrawText(pDC->GetSafeHdc(), lpBuffer, (int)strlen(lpBuffer), 
			&rcItem, dwStyle|DT_END_ELLIPSIS);
	}
}

//貌似没用的
void HListCtrl::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	CListCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);//什么事情都不干
}

//设置字体
void HListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if ( m_nRowHeight > 0 )//重载该函数，设置字体高度
	{
		lpMeasureItemStruct->itemHeight = m_nRowHeight;
	}
}

//插入列
int HListCtrl::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat, int nWidth, int nSubItem)
{
	m_Header.AddString(lpszColumnHeading);
	return CListCtrl::InsertColumn(nCol,lpszColumnHeading,nFormat,nWidth,nSubItem);
}

//插入子
int HListCtrl::InsertItem( int nItem, LPCTSTR lpszItem )
{
	m_mapCheckMgr[nItem] = 1;
	return CListCtrl::InsertItem(nItem,lpszItem);
}

// 设置表头高度
void HListCtrl::SetHeaderHeight(float Height) //设置表头高度
{
	m_Header.SetHeight(Height);
}

//设置行高(该处是最重要的)
void HListCtrl::SetRowHeigt(int nHeight) 
{
	m_nRowHeight = nHeight;
	CRect rcWin;
	GetWindowRect(&rcWin);
	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rcWin.Width();
	wp.cy = rcWin.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
}

//得到列总数
int HListCtrl::GetColNum()
{
	LVCOLUMN pColumn;   
	ZeroMemory(&pColumn,sizeof(LVCOLUMN));   
	pColumn.mask = LVCF_WIDTH;   
	int nCol=0,nColNum=0;   
	while(TRUE == GetColumn(nCol,&pColumn))   
	{   
		nCol += 1;   
	}   
	nColNum = nCol;
	return nColNum-1;
}

//遍历所选
int HListCtrl::SearchSelectIndex()
{
	POSITION pItemPos = GetFirstSelectedItemPosition();
	int nKeyIndex = 255;
	while (pItemPos != NULL)
	{
		nKeyIndex = GetNextSelectedItem(pItemPos);
	}
	//nKeyIndex是基于0的索引
	return nKeyIndex;
}

//遍历所选文本
CString HListCtrl::SearchSelectText()
{
	POSITION pItemPos = GetFirstSelectedItemPosition();
	CString keyString;
	while (pItemPos != NULL)
	{
		const int nKeyIndex = GetNextSelectedItem(pItemPos);  
		keyString = GetItemText(nKeyIndex,0);
	}
	return keyString;
}

//删除所选
void HListCtrl::ReMoveSelect()
{
	POSITION pItemPos = GetFirstSelectedItemPosition();
	int nPos = 0;
	while (pItemPos != NULL) 
	{
		const int nItemIndex = GetNextSelectedItem( pItemPos );
		DeleteItem(nItemIndex-nPos);
		++nPos;
	} 
}	

//从鼠标位置得到选择的索引
int HListCtrl::GetIndexFromPoint()
{
	DWORD dwPos = ::GetMessagePos();   
	CPoint point((int)LOWORD(dwPos),(int)HIWORD(dwPos));   
	ScreenToClient(&point);   
	return HitTest(point);
}

//设置某行被选中
void HListCtrl::SetRowSelected( int nIndex )
{
	if (nIndex > (GetItemCount()-1)) return;
	m_nCurHighlighted = nIndex;
	SetItemState(m_nCurHighlighted,0,LVIS_SELECTED); 
	UpdateSelected();
	EnsureVisible(nIndex,FALSE);
	Invalidate();
}

//左键按下
void HListCtrl::OnLButtonDown( UINT nFlags, CPoint point )
{
	m_nCurHighlighted = GetIndexFromPoint();   
	if (m_nCurHighlighted < 0 || m_nCurHighlighted > GetItemCount())
	{
		return CListCtrl::OnLButtonDown(nFlags, point);
	}
	//InvalidateRect(GetShowRect());
	UpdataCheckBox(point,m_nCurHighlighted);
	UpdateSelected();
	CListCtrl::OnLButtonDown(nFlags, point);
}	

//右键按下
void HListCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	CListCtrl::OnRButtonDown(nFlags, point);
}

//得到显示的矩形
CRect HListCtrl::GetShowRect()
{
	CRect rect;
	GetClientRect(&rect);
	CRect headerRect;
	GetDlgItem(0)->GetWindowRect(&headerRect);
	headerRect.bottom -= 2;
	rect -= headerRect;
	return rect;
}

//鼠标移动
void HListCtrl::OnMouseMove( UINT nFlags, CPoint point )
{
	CListCtrl::OnMouseMove(nFlags, point);
}

//更新选择
void HListCtrl::UpdateSelected()
{
	::PostMessage(GetParent()->GetSafeHwnd(),WM_HITCARLISTCTRL,(WPARAM)m_nCurHighlighted,0);
}

//更新CheckBox
void HListCtrl::UpdataCheckBox( CPoint point,int nHighLight )
{
	if (!m_bCheckBox) return;
	CRect rect;
	if (!GetSubItemRect(nHighLight,0,LVIR_LABEL,rect)) return;
	CRect rcItem = rect;
	rcItem.right = rcItem.left + m_imgCheckBox.GetWidth()/4+5;
	rcItem.bottom = rcItem.top + m_imgCheckBox.GetHeight()+5;
	if (rcItem.PtInRect(point))
	{
		if (m_mapCheckMgr[nHighLight])
		{
			m_mapCheckMgr[nHighLight] = 0;
		}
		else 
		{
			m_mapCheckMgr[nHighLight] = 1;
		}
	}
	InvalidateRect(rcItem);
}

//得到选中的项目(复选框选中的)
void HListCtrl::GetSelected( int *nCount )
{
#pragma warning(disable:4267) 
#pragma warning(push)
	for (size_t i = 0; i < m_mapCheckMgr.size(); ++i)
	{
		if (m_mapCheckMgr[i])
		{
			nCount[i] = 1;
		}
		else 
		{
			nCount[i] = 0;
		}
	}
#pragma warning(pop)
}

//消息解析
LRESULT HListCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message==WM_VSCROLL||message==WM_HSCROLL)
	{
		WORD sbCode=LOWORD(wParam);
		if(sbCode==SB_THUMBTRACK
			||sbCode==SB_THUMBPOSITION)
		{
			SCROLLINFO siv={0};
			siv.cbSize=sizeof(SCROLLINFO);
			siv.fMask=SIF_ALL;
			SCROLLINFO sih=siv;
			int nPos=HIWORD(wParam);
			CRect rcClient;
			GetClientRect(&rcClient);
			GetScrollInfo(SB_VERT,&siv);
			GetScrollInfo(SB_HORZ,&sih);
			SIZE sizeAll;
			if(sih.nPage==0) 
				sizeAll.cx=rcClient.right;
			else
				sizeAll.cx=rcClient.right*(sih.nMax+1)/sih.nPage ;
			if(siv.nPage==0)
				sizeAll.cy=rcClient.bottom;
			else
				sizeAll.cy=rcClient.bottom*(siv.nMax+1)/siv.nPage ;

			SIZE size={0,0};
			if(WM_VSCROLL==message)
			{
				size.cx=sizeAll.cx*sih.nPos/(sih.nMax+1);
				size.cy=sizeAll.cy*(nPos-siv.nPos)/(siv.nMax+1);
			}else
			{
				size.cx=sizeAll.cx*(nPos-sih.nPos)/(sih.nMax+1);
				size.cy=sizeAll.cy*siv.nPos/(siv.nMax+1);
			}
			Scroll(size);
			Invalidate(FALSE);
			return 1;
		}
	}
	return CListCtrl::WindowProc(message, wParam, lParam);
}

//销毁
void HListCtrl::OnDestroy()
{
	CListCtrl::OnDestroy();
	CHeaderCtrl *pHeader = GetHeaderCtrl();
	m_Header.UnsubclassWindow();
}

//控制头控件不能被拉动到最后
BOOL HListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	HD_NOTIFY *pHDN = (HD_NOTIFY*)lParam; 
	if((pHDN->hdr.code == HDN_ITEMCHANGINGW || pHDN-> hdr.code == HDN_ITEMCHANGINGA)   
		&& pHDN->pitem->cxy < 25) 
	{ 
		*pResult = TRUE;
		return TRUE;                                         
	} 
	return CListCtrl::OnNotify(wParam, lParam, pResult);
}