#include "StdAfx.h"
#include "HTreeCtrlDiary.h"

HTreeCtrlDiary::HTreeCtrlDiary()
{
}

HTreeCtrlDiary::~HTreeCtrlDiary()
{
}

void HTreeCtrlDiary::OnHitTreeItem( HTREEITEM hItem )
{

}

void HTreeCtrlDiary::OnDHitTreeItem( HTREEITEM hItem )
{
	CString strText = GetItemText(hItem);
	TCHAR szText[2048] = {0};
	sprintf(szText,"%s",strText);
	if (strText.Find('-'))
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(),WM_DHITTRLL,(WPARAM)szText,0);
}

BEGIN_MESSAGE_MAP(HTreeCtrlDiary, HTreeCtrl)
ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

void HTreeCtrlDiary::OnRButtonDown(UINT nFlags, CPoint point)
{
	HTreeCtrl::OnRButtonDown(nFlags, point);
}
