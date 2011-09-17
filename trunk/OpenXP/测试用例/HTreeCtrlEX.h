#pragma once

class HTreeCtrlEX : public HTreeCtrl
{
public:
	HTreeCtrlEX(void);
	~HTreeCtrlEX(void);
protected:
	//单击到无子节点的Item
	virtual void OnHitTreeItem(HTREEITEM hItem);
	//双击到无子节点的Item
	virtual void OnDHitTreeItem(HTREEITEM hItem);
};
