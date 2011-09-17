#ifndef __HTREECTRL__H__
#define __HTREECTRL__H__
#pragma once
#include <map>
using namespace std;

//项所包含的相关信息
typedef struct tagItemInfo  
{	
	HTREEITEM hTreeItem;		//项的句柄
	CImage    ImgItemBitmap;	//项的位图
	COLORREF  crTransparent;	//透明颜色
	tagItemInfo()
	{
		hTreeItem   = NULL;
		crTransparent = RGB(255,255,255);
	}
	~tagItemInfo(){}

}ITEM_INFO;
typedef map<HTREEITEM,ITEM_INFO*>	ITEMINFO_MAP;

class UI_CONTROL_API HTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(HTreeCtrl)

public:
	HTreeCtrl();
	virtual ~HTreeCtrl();

protected:
	//单击到无子节点的Item
	virtual void OnHitTreeItem(HTREEITEM hItem)=0;
	//双击到无子节点的Item
	virtual void OnDHitTreeItem(HTREEITEM hItem)=0;

public:
	//设置背景图片
	BOOL SetTreeBkImage(LPCTSTR lpszImagePath);
	BOOL SetTreeBkImage(UINT nID,HINSTANCE hInstance=NULL);

	//设置某一项图片
	void SetItemBitmap(HTREEITEM hItem, UINT nIdBitmap,HINSTANCE hInstance=NULL);
	void SetItemBitmap(HTREEITEM hItem, LPCTSTR lpszBitmap);

	//设置展开或收缩的图表(比如 + 或 - )
	void SetExpandBitmap(UINT nIdExpand, UINT nIdCollapse, COLORREF crTransparent = RGB(255,255,255),HINSTANCE hInstance=NULL);
	void SetExpandBitmap(LPCTSTR lpszExpand, LPCTSTR lpszCollapse, COLORREF crTransparent = RGB(255,255,255));

	//设置Item高度
	void SetHeight(UINT nItemHeight);

	//得到Item高度
	UINT GetHeight(){return m_nItemHeight;}

	//设置分割线的颜色
	void SetRowLineColor(COLORREF crRowLine=RGB(238,238,238)){m_crRowLine=crRowLine;}

	//设置选中和热跟踪颜色
	void SetSelectedAndHotColor(COLORREF crSelected=RGB(65,164,231),COLORREF crHot=RGB(203,232,251)){m_crSelected=crSelected;m_crHot=crHot;}

	//设置选中和热跟踪文本颜色
	void SetTextColor(COLORREF crSelected=RGB(255,255,255),COLORREF crNormal=RGB(0,0,0)){m_crSelectedText=crSelected;m_crNoramlText=crNormal;}

	//获取Item信息
	ITEM_INFO* GetItemInfo(HTREEITEM hItem);

	//插入新的项目
	HTREEITEM InsertItemEx(HTREEITEM hParent, LPCTSTR lpszItem, UINT nIdBitmap = 0,COLORREF crTransparent = RGB(255,255,255),HINSTANCE hInstance=NULL);
	HTREEITEM InsertItemEx(HTREEITEM hParent, LPCTSTR lpszItem, LPCTSTR lpszBitmap,COLORREF crTransparent = RGB(255,255,255));

protected:
	void DrawTreeItem(CDC *pDC);
	void DrawRowLine(CPoint ptBegin, CPoint ptEnd, CDC *pDC);
	void DrawExpand(CRect rcExpand, int nState, CDC *pDC);
	void DrawItemBitmap(CRect rcItem, HTREEITEM hItem, CDC *pDC);
	void DrawItemText(CRect rcText, HTREEITEM hItem, bool bSelected,CDC *pDC);

protected:
	ITEM_INFO*      m_pCurDrawItemMsg;	//当前正在绘制的项信息
	ITEMINFO_MAP	m_mapItmeInfo;			//保存所有相的相关信息
	UINT			m_nItemHeight;			//Item高度				
	CFont			m_fontRoot;				//根节点字体
	CFont			m_fontChild;			//子节点字体
	CImage			m_ImgTreeBK;			//控件背景图片
	CImage			m_ImgTreeAdd;			//伸展图表图片(例如 +)
	CImage			m_ImgTreeSub;			//缩小图标图片(例如 -）	
	CImageList		m_imglistExpand;		//展开或缩小的图表( + 、- 的图表)
	HTREEITEM       m_hItemMouseMove;		//当前鼠标所在的句柄
	CPoint			m_ptOldMouse;			//保存鼠标位置
	COLORREF		m_crRowLine;			//分割线颜色
	COLORREF		m_crSelected;			//被选中颜色
	COLORREF		m_crHot;				//热跟踪颜色
	COLORREF		m_crSelectedText;		//被选中文本颜色
	COLORREF		m_crNoramlText;			//正常文本颜色

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void PreSubclassWindow();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

#endif