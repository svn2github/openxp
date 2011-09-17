#ifndef __HTREECTRL__H__
#define __HTREECTRL__H__
#pragma once
#include <map>
using namespace std;

//���������������Ϣ
typedef struct tagItemInfo  
{	
	HTREEITEM hTreeItem;		//��ľ��
	CImage    ImgItemBitmap;	//���λͼ
	COLORREF  crTransparent;	//͸����ɫ
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
	//���������ӽڵ��Item
	virtual void OnHitTreeItem(HTREEITEM hItem)=0;
	//˫�������ӽڵ��Item
	virtual void OnDHitTreeItem(HTREEITEM hItem)=0;

public:
	//���ñ���ͼƬ
	BOOL SetTreeBkImage(LPCTSTR lpszImagePath);
	BOOL SetTreeBkImage(UINT nID,HINSTANCE hInstance=NULL);

	//����ĳһ��ͼƬ
	void SetItemBitmap(HTREEITEM hItem, UINT nIdBitmap,HINSTANCE hInstance=NULL);
	void SetItemBitmap(HTREEITEM hItem, LPCTSTR lpszBitmap);

	//����չ����������ͼ��(���� + �� - )
	void SetExpandBitmap(UINT nIdExpand, UINT nIdCollapse, COLORREF crTransparent = RGB(255,255,255),HINSTANCE hInstance=NULL);
	void SetExpandBitmap(LPCTSTR lpszExpand, LPCTSTR lpszCollapse, COLORREF crTransparent = RGB(255,255,255));

	//����Item�߶�
	void SetHeight(UINT nItemHeight);

	//�õ�Item�߶�
	UINT GetHeight(){return m_nItemHeight;}

	//���÷ָ��ߵ���ɫ
	void SetRowLineColor(COLORREF crRowLine=RGB(238,238,238)){m_crRowLine=crRowLine;}

	//����ѡ�к��ȸ�����ɫ
	void SetSelectedAndHotColor(COLORREF crSelected=RGB(65,164,231),COLORREF crHot=RGB(203,232,251)){m_crSelected=crSelected;m_crHot=crHot;}

	//����ѡ�к��ȸ����ı���ɫ
	void SetTextColor(COLORREF crSelected=RGB(255,255,255),COLORREF crNormal=RGB(0,0,0)){m_crSelectedText=crSelected;m_crNoramlText=crNormal;}

	//��ȡItem��Ϣ
	ITEM_INFO* GetItemInfo(HTREEITEM hItem);

	//�����µ���Ŀ
	HTREEITEM InsertItemEx(HTREEITEM hParent, LPCTSTR lpszItem, UINT nIdBitmap = 0,COLORREF crTransparent = RGB(255,255,255),HINSTANCE hInstance=NULL);
	HTREEITEM InsertItemEx(HTREEITEM hParent, LPCTSTR lpszItem, LPCTSTR lpszBitmap,COLORREF crTransparent = RGB(255,255,255));

protected:
	void DrawTreeItem(CDC *pDC);
	void DrawRowLine(CPoint ptBegin, CPoint ptEnd, CDC *pDC);
	void DrawExpand(CRect rcExpand, int nState, CDC *pDC);
	void DrawItemBitmap(CRect rcItem, HTREEITEM hItem, CDC *pDC);
	void DrawItemText(CRect rcText, HTREEITEM hItem, bool bSelected,CDC *pDC);

protected:
	ITEM_INFO*      m_pCurDrawItemMsg;	//��ǰ���ڻ��Ƶ�����Ϣ
	ITEMINFO_MAP	m_mapItmeInfo;			//����������������Ϣ
	UINT			m_nItemHeight;			//Item�߶�				
	CFont			m_fontRoot;				//���ڵ�����
	CFont			m_fontChild;			//�ӽڵ�����
	CImage			m_ImgTreeBK;			//�ؼ�����ͼƬ
	CImage			m_ImgTreeAdd;			//��չͼ��ͼƬ(���� +)
	CImage			m_ImgTreeSub;			//��Сͼ��ͼƬ(���� -��	
	CImageList		m_imglistExpand;		//չ������С��ͼ��( + ��- ��ͼ��)
	HTREEITEM       m_hItemMouseMove;		//��ǰ������ڵľ��
	CPoint			m_ptOldMouse;			//�������λ��
	COLORREF		m_crRowLine;			//�ָ�����ɫ
	COLORREF		m_crSelected;			//��ѡ����ɫ
	COLORREF		m_crHot;				//�ȸ�����ɫ
	COLORREF		m_crSelectedText;		//��ѡ���ı���ɫ
	COLORREF		m_crNoramlText;			//�����ı���ɫ

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