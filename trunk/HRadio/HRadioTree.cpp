#include "StdAfx.h"
#include "HRadioTree.h"

HRadioTree::HRadioTree(void)
{
}

HRadioTree::~HRadioTree(void)
{
}

void HRadioTree::OnHitTreeItem( HTREEITEM hItem )
{
}

void HRadioTree::OnDHitTreeItem( HTREEITEM hItem )
{
	AfxGetMainWnd()->PostMessage(WM_HITTREE,(WPARAM)hItem,0);
}