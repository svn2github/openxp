#pragma once

class HRadioTree : public HTreeCtrl
{
public:
	HRadioTree(void);
	~HRadioTree(void);
protected:
	//单击到无子节点的Item
	virtual void OnHitTreeItem(HTREEITEM hItem);
	//双击到无子节点的Item
	virtual void OnDHitTreeItem(HTREEITEM hItem);
};
