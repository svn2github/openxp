#pragma once

class HRadioTree : public HTreeCtrl
{
public:
	HRadioTree(void);
	~HRadioTree(void);
protected:
	//���������ӽڵ��Item
	virtual void OnHitTreeItem(HTREEITEM hItem);
	//˫�������ӽڵ��Item
	virtual void OnDHitTreeItem(HTREEITEM hItem);
};
