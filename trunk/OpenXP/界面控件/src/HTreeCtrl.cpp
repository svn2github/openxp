#include "stdafx.h"
#include "../resource.h"
#include "../include/HTreeCtrl.h"


//�궨�庯���������ͷ�GDI����
#define RELEASE_GDI_RESOURCE(gdiResource) if(gdiResource.GetSafeHandle()){gdiResource.DeleteObject();}

IMPLEMENT_DYNAMIC(HTreeCtrl, CTreeCtrl)

HTreeCtrl::HTreeCtrl():m_nItemHeight(0)
{
	m_hItemMouseMove  = NULL;
	m_pCurDrawItemMsg = NULL;

	m_crRowLine = RGB(238,238,238);
	m_crSelected = RGB(65,164,231);
	m_crHot = RGB(203,232,251);
	m_crSelectedText = RGB(255,255,255);
	m_crNoramlText = RGB(0,0,0);

	m_fontRoot.CreateFont(12,0,0,0,FW_BOLD,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH|FF_SWISS,_T("����") );             
	m_fontChild.CreateFont(12,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH|FF_SWISS,_T("����") );            
}

HTreeCtrl::~HTreeCtrl()
{
	RELEASE_GDI_RESOURCE(m_fontRoot);
	RELEASE_GDI_RESOURCE(m_fontChild);

	if (m_imglistExpand.GetSafeHandle())
		m_imglistExpand.DeleteImageList();

	ITEMINFO_MAP::iterator item = m_mapItmeInfo.begin();
	for (;item != m_mapItmeInfo.end(); item++)
		delete item->second;
}

BEGIN_MESSAGE_MAP(HTreeCtrl, CTreeCtrl)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

//�������οؼ�����ͼƬ
BOOL HTreeCtrl::SetTreeBkImage( LPCTSTR lpszImagePath )
{
	if (!m_ImgTreeBK.IsNull()) m_ImgTreeBK.Destroy();
		m_ImgTreeBK.Load(lpszImagePath);
	return (!m_ImgTreeBK.IsNull());
}

//�������οؼ�����ͼƬ
BOOL HTreeCtrl::SetTreeBkImage( UINT nID,HINSTANCE hInstance )
{
	if (!m_ImgTreeBK.IsNull()) m_ImgTreeBK.Destroy();
		m_ImgTreeBK.LoadFromResource(hInstance,nID);
	return (!m_ImgTreeBK.IsNull());
}

//��������ͼ��
void HTreeCtrl::SetExpandBitmap( UINT nIdExpand, UINT nIdCollapse, COLORREF crTransparent,HINSTANCE hInstance )
{
	if (!m_ImgTreeAdd.IsNull()) m_ImgTreeAdd.Destroy();
	if (!m_ImgTreeSub.IsNull()) m_ImgTreeSub.Destroy();
	m_ImgTreeAdd.LoadFromResource(hInstance,nIdExpand);
	m_ImgTreeSub.LoadFromResource(hInstance,nIdCollapse);

	if (m_ImgTreeAdd.IsNull() || m_ImgTreeSub.IsNull()) return;

	CBitmap bmpExpand,bmpCollapse;

	HBITMAP hExpand = m_ImgTreeAdd.operator HBITMAP();
	bmpExpand.Attach(hExpand);

	HBITMAP hCollapse = m_ImgTreeSub.operator HBITMAP();
	bmpCollapse.Attach(hCollapse);

	//����CImageList
	m_imglistExpand.Create(m_ImgTreeAdd.GetWidth(),m_ImgTreeAdd.GetHeight(),ILC_COLOR24|ILC_MASK,0,2);
	m_imglistExpand.Add(&bmpExpand,crTransparent);
	m_imglistExpand.Add(&bmpCollapse,crTransparent);
}

//��������ͼ��
void HTreeCtrl::SetExpandBitmap( LPCTSTR lpszExpand, LPCTSTR lpszCollapse, COLORREF crTransparent )
{
	if (!m_ImgTreeAdd.IsNull()) m_ImgTreeAdd.Destroy();
	if (!m_ImgTreeSub.IsNull()) m_ImgTreeSub.Destroy();
	m_ImgTreeAdd.Load(lpszExpand);
	m_ImgTreeSub.Load(lpszCollapse);

	if (m_ImgTreeAdd.IsNull() || m_ImgTreeSub.IsNull()) return;

	CBitmap bmpExpand,bmpCollapse;

	HBITMAP hExpand = m_ImgTreeAdd.operator HBITMAP();
	bmpExpand.Attach(hExpand);

	HBITMAP hCollapse = m_ImgTreeSub.operator HBITMAP();
	bmpCollapse.Attach(hCollapse);

	//����CImageList
	m_imglistExpand.Create(m_ImgTreeAdd.GetWidth(),m_ImgTreeAdd.GetHeight(),ILC_COLOR24|ILC_MASK,0,2);
	m_imglistExpand.Add(&bmpExpand,crTransparent);
	m_imglistExpand.Add(&bmpCollapse,crTransparent);
}

//����ĳһ���ͼƬ
void HTreeCtrl::SetItemBitmap( HTREEITEM hItem, UINT nIdBitmap,HINSTANCE hInstance )
{
	if (!hItem) return;

	ITEM_INFO* pItemInfo = GetItemInfo(hItem);	
	if (!pItemInfo) return;

	if (!pItemInfo->ImgItemBitmap.IsNull())
		pItemInfo->ImgItemBitmap.Destroy();

	if (0 != nIdBitmap)
		pItemInfo->ImgItemBitmap.LoadFromResource(hInstance,nIdBitmap);
}

//����ĳһ���ͼƬ
void HTreeCtrl::SetItemBitmap( HTREEITEM hItem, LPCTSTR lpszBitmap )
{
	if (!hItem) return;

	ITEM_INFO* pItemInfo = GetItemInfo(hItem);	
	if (!pItemInfo) return;

	if (!pItemInfo->ImgItemBitmap.IsNull())
		pItemInfo->ImgItemBitmap.Destroy();

	pItemInfo->ImgItemBitmap.Load(lpszBitmap);
}

//���Ʒָ���
void HTreeCtrl::DrawRowLine( CPoint ptBegin, CPoint ptEnd, CDC *pDC )
{
	ASSERT(NULL!=pDC);
	CPen pen(PS_SOLID,1,m_crRowLine);
	CPen *pOldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(ptBegin);
	pDC->LineTo(ptEnd);
	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}

//��������ͼ��
void HTreeCtrl::DrawExpand( CRect rcExpand, int nState, CDC *pDC )
{
	if (!m_imglistExpand.GetImageCount())
		return;
	
	ASSERT(NULL!=pDC);

	int nImage = 0;
	if (nState & TVIS_EXPANDED)	//չ��
		nImage = 1;
	else	//����
		nImage = 0;

	CPoint point;
	point.x = rcExpand.left - m_ImgTreeAdd.GetWidth()-4;
	point.y = rcExpand.top+(rcExpand.Height()-m_ImgTreeAdd.GetHeight())/2;

	m_imglistExpand.Draw(pDC,nImage,point,ILD_TRANSPARENT);
}

//������ͼ��
void HTreeCtrl::DrawItemBitmap( CRect rcItem, HTREEITEM hItem, CDC *pDC )
{
	ASSERT(NULL!=pDC);
	if (m_pCurDrawItemMsg && !m_pCurDrawItemMsg->ImgItemBitmap.IsNull())
	{
		int nXOriginDest = rcItem.left;									// Ŀ��Xƫ��
		int nYOriginDest = rcItem.top+(rcItem.Height()-m_pCurDrawItemMsg->ImgItemBitmap.GetHeight())/2;	// Ŀ��Yƫ��
		int nWidthDest	 = m_pCurDrawItemMsg->ImgItemBitmap.GetWidth();								// Ŀ����
		int hHeightDest  = m_pCurDrawItemMsg->ImgItemBitmap.GetHeight();								// Ŀ��߶�

		m_pCurDrawItemMsg->ImgItemBitmap.TransparentBlt(pDC->GetSafeHdc(),
			nXOriginDest,nYOriginDest,nWidthDest,hHeightDest,
			0,0,m_pCurDrawItemMsg->ImgItemBitmap.GetWidth(),m_pCurDrawItemMsg->ImgItemBitmap.GetHeight(),
			m_pCurDrawItemMsg->crTransparent);
	}
}

//�������ı�
void HTreeCtrl::DrawItemText( CRect rcText, HTREEITEM hItem, bool bSelected,CDC *pDC )
{
	ASSERT(NULL!=pDC);

	if (m_pCurDrawItemMsg&& !m_pCurDrawItemMsg->ImgItemBitmap.IsNull())
	{
		rcText.left += m_pCurDrawItemMsg->ImgItemBitmap.GetWidth();
		rcText.right+= m_pCurDrawItemMsg->ImgItemBitmap.GetWidth();
	}

	CString strText = GetItemText(hItem);
	CSize sz = pDC->GetTextExtent(strText);
	rcText.DeflateRect(0,1,0,1);
	rcText.top += (rcText.Height()-sz.cy)/2;
	CRect rcClient;
	GetClientRect(rcClient);
	rcText.right= rcClient.right;

	//��������
	CFont* pOldFont = NULL;
	HTREEITEM hParentItem = GetParentItem(hItem);
	if (hParentItem)//���ڵ�
		pOldFont = pDC->SelectObject(&m_fontChild);
	else//�ӽڵ�
		pOldFont = pDC->SelectObject(&m_fontRoot);

	pDC->SetBkMode(TRANSPARENT);
	if (bSelected)
		pDC->SetTextColor(m_crSelectedText);
	else
		pDC->SetTextColor(m_crNoramlText);

	pDC->DrawText(strText,rcText,DT_LEFT|DT_TOP);

	pDC->SelectObject(pOldFont);
	pOldFont = NULL;
}

//�������е����νṹ
void HTreeCtrl::DrawTreeItem( CDC *pDC )
{
	HTREEITEM hTreeShowItem,hTreeParent; //��ǰ�������Լ����ĸ����ھ��
	CRect rcItem;						//�������
	DWORD dwStyle;						//��������
	int nItemState;						//ĳһ���״̬
	bool bSelected;						//True:��ʾ����Ҫ����	
	bool bHaveChilden;					//True:��ʾ�ǲ�����չ����������

	hTreeShowItem = GetFirstVisibleItem();//��ȡ��һ���ɼ���ľ��
	if (hTreeShowItem == NULL) return;

	dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE); //��ȡTREE������

	do
	{
		nItemState = GetItemState(hTreeShowItem, TVIF_STATE);//��ȡĳһ���״̬
		hTreeParent = GetParentItem(hTreeShowItem);		  //��ȡĳһ��ĸ����ھ��

		bHaveChilden = (ItemHasChildren(hTreeShowItem) || hTreeParent==NULL);

		bSelected = (nItemState & TVIS_SELECTED) && ((this == GetFocus()) || 
			(dwStyle & TVS_SHOWSELALWAYS));

		if (GetItemRect(hTreeShowItem,rcItem,TRUE))//��ȡĳһ��ľ�������
		{
			CRect rcClient;
			GetClientRect(rcClient);
			if (rcItem.top >= rcClient.bottom) break;	//���ɼ���ʱ��,�Ͳ���Ҫ�ٻ���

			m_pCurDrawItemMsg = GetItemInfo(hTreeShowItem);

			CRect rectItemToClient;
			rectItemToClient.left = rcClient.left;
			rectItemToClient.right = rcClient.right;
			rectItemToClient.top = rcItem.top;
			rectItemToClient.bottom = rcItem.bottom;
			
			//���ƺ���
			DrawRowLine(CPoint(rectItemToClient.left,rectItemToClient.bottom),CPoint(rectItemToClient.right,rectItemToClient.bottom),pDC);

			if (bHaveChilden)
			{	
				if (bSelected)	//ѡ��ʱ��������
					pDC->FillSolidRect(rectItemToClient,m_crSelected);
				else if (m_hItemMouseMove == hTreeShowItem) //�ȸ���
					pDC->FillSolidRect(rectItemToClient,m_crHot);

				//����չ������
				DrawExpand(rcItem,nItemState,pDC);

				//������λͼ
				DrawItemBitmap(rcItem,hTreeShowItem,pDC);
			}
			else
			{
				if (bSelected)	//ѡ��ʱ��������
					pDC->FillSolidRect(rectItemToClient,m_crSelected);
				else if (m_hItemMouseMove == hTreeShowItem)	//�ȸ���
					pDC->FillSolidRect(rectItemToClient,m_crHot);

				//������λͼ
				DrawItemBitmap(rcItem,hTreeShowItem,pDC);	
			}

			//�����ı�
			DrawItemText(rcItem,hTreeShowItem,bSelected,pDC);
		}
	}while ((hTreeShowItem = GetNextVisibleItem(hTreeShowItem)) != NULL);
}

//�滭����
void HTreeCtrl::OnPaint()
{
	CPaintDC dc(this); 
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������DC
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	//��������BITMAP
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());
	CBitmap *pOldBitmap = memDC.SelectObject(&memBitmap);

	//COPY����ͼ
	if (!m_ImgTreeBK.IsNull())
		m_ImgTreeBK.StretchBlt(memDC.GetSafeHdc(),0,0,rcClient.Width(),rcClient.Height(),0,0,m_ImgTreeBK.GetWidth(),m_ImgTreeBK.GetHeight(),SRCCOPY);
	else 
		dc.FillSolidRect(rcClient,0x000000);

	//COPY���νṹ
	DrawTreeItem(&memDC);

	dc.BitBlt(rcClient.left,rcClient.top,rcClient.Width(),rcClient.Height(),&memDC,0,0,SRCCOPY);
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
}

//�����ؼ�����
int HTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetTreeBkImage(IDB_TREECTRL,GetModuleHandle(UI_CONTROL_DLL_NAME));
	return 0;
}

//�ؼ����໰
void HTreeCtrl::PreSubclassWindow()
{
	SetItemHeight(25);
	m_nItemHeight = 25;
	SetTreeBkImage(IDB_TREECTRL,GetModuleHandle(UI_CONTROL_DLL_NAME));
	SetExpandBitmap(IDB_TREECTRL_ADD,IDB_TREECTRL_SUB,RGB(255,255,255),GetModuleHandle(UI_CONTROL_DLL_NAME));
	ModifyStyle(0,TVS_INFOTIP,0);
	CTreeCtrl::PreSubclassWindow();
}

//����Item�߶�
void HTreeCtrl::SetHeight( UINT nItemHeight )
{
	SetItemHeight(nItemHeight);
	m_nItemHeight = nItemHeight;
}

//��������
HTREEITEM HTreeCtrl::InsertItemEx( HTREEITEM hParent, LPCTSTR lpszItem, UINT nIdBitmap, COLORREF crTransparent, HINSTANCE hInstance )
{
	HTREEITEM hItem = InsertItem(lpszItem,0,1,hParent);
	ITEM_INFO *pItemMsg = new ITEM_INFO;
	pItemMsg->hTreeItem = hItem;

	if (0 != nIdBitmap)
	{
		if (!pItemMsg->ImgItemBitmap.IsNull())
			pItemMsg->ImgItemBitmap.Destroy();
		pItemMsg->ImgItemBitmap.LoadFromResource(hInstance,nIdBitmap);
	}

	pItemMsg->crTransparent = crTransparent;

	ITEMINFO_MAP::iterator item = m_mapItmeInfo.find(hItem);
	if (item == m_mapItmeInfo.end())
	{
		m_mapItmeInfo[hItem] = pItemMsg;
	}
	else
	{
		item->second = pItemMsg;
	}
	return hItem;
}

//��������
HTREEITEM HTreeCtrl::InsertItemEx( HTREEITEM hParent, LPCTSTR lpszItem, LPCTSTR lpszBitmap, COLORREF crTransparent )
{
	HTREEITEM hItem = InsertItem(lpszItem,0,1,hParent);

	ITEM_INFO *pItemMsg = new ITEM_INFO;
	pItemMsg->hTreeItem = hItem;

	if (!pItemMsg->ImgItemBitmap.IsNull())
		pItemMsg->ImgItemBitmap.Destroy();
	pItemMsg->ImgItemBitmap.Load(lpszBitmap);

	pItemMsg->crTransparent = crTransparent;

	ITEMINFO_MAP::iterator item = m_mapItmeInfo.find(hItem);
	if (item == m_mapItmeInfo.end())
	{
		m_mapItmeInfo[hItem] = pItemMsg;
	}
	else
	{
		item->second = pItemMsg;
	}
	return hItem;	
}

//����ƶ�
void HTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	CTreeCtrl::OnMouseMove(nFlags, point);

	m_ptOldMouse = point;
	HTREEITEM hItem = HitTest(point);
	if (hItem != NULL && hItem != m_hItemMouseMove)
	{
		m_hItemMouseMove = hItem;
		Invalidate(FALSE);
	}
}

//�������
void HTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CTreeCtrl::OnLButtonDown(nFlags, point);
	HTREEITEM hItem = HitTest(point);
	if (hItem != NULL)
	{
		SelectItem(hItem);//ѡ�и���
		if (ItemHasChildren(hItem))//������ӽڵ㣬չ��
			Expand(hItem,TVE_EXPAND);
		else 
			OnHitTreeItem(hItem);	
		Invalidate(FALSE);
	}
}

//���˫��
void HTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CTreeCtrl::OnLButtonDblClk(nFlags, point);
	HTREEITEM hItem = HitTest(point);
	if (hItem != NULL)
	{
		SelectItem(hItem);//ѡ�и���
		if (ItemHasChildren(hItem))//������ӽڵ㣬չ��
			Expand(hItem,TVE_COLLAPSE);
		else 
			OnDHitTreeItem(hItem);	
		Invalidate(FALSE);
	}
}

//��ȡĳ��Item����Ϣ
ITEM_INFO* HTreeCtrl::GetItemInfo( HTREEITEM hItem )
{
	if (!hItem) return NULL;

	ITEM_INFO* pItemMsg = NULL;
	ITEMINFO_MAP::iterator item = m_mapItmeInfo.find(hItem);
	if (item != m_mapItmeInfo.end())
		pItemMsg = item->second;

	return pItemMsg;
}

//��������
BOOL HTreeCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

////���������ӽڵ��Item
//void HTreeCtrl::OnHitTreeItem( HTREEITEM hItem )
//{
//}
//
////˫�������ӽڵ��Item
//void HTreeCtrl::OnDHitTreeItem( HTREEITEM hItem )
//{
//}