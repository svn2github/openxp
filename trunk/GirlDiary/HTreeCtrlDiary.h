#ifndef __HTREECTRLDIARY__H__
#define __HTREECTRLDIARY__H__
#pragma once

class HTreeCtrlDiary : public HTreeCtrl
{
public:
	HTreeCtrlDiary();
	~HTreeCtrlDiary();
protected:
	//单击到无子节点的Item
	virtual void OnHitTreeItem(HTREEITEM hItem);
	//双击到无子节点的Item
	virtual void OnDHitTreeItem(HTREEITEM hItem);
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

#endif