#pragma once

class HTreeCtrlEX : public HTreeCtrl
{
public:
	HTreeCtrlEX(void);
	~HTreeCtrlEX(void);
protected:
	//���������ӽڵ��Item
	virtual void OnHitTreeItem(HTREEITEM hItem);
	//˫�������ӽڵ��Item
	virtual void OnDHitTreeItem(HTREEITEM hItem);
};
