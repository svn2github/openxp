#ifndef __HLISTCTRL__H__
#define __HLISTCTRL__H__
#pragma once
#include "HHeaderCtrl.h"
#include <atlimage.h>
#include <map>

#define WM_HITCARLISTCTRL WM_USER+1500 //点击某一行时发送消息

class UI_CONTROL_API HListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(HListCtrl)

public:
	HListCtrl();
	virtual ~HListCtrl();

	//外部接口
public:
	void LoadResource(LPCTSTR lpstHeader,LPCTSTR lpstBk,LPCTSTR lpstCheckBox=NULL,LPCTSTR lpstSelected=NULL);//设置图片资源
	void LoadResource(HINSTANCE hInstance,UINT nHeader,UINT nBk,UINT nCheckBox=0,UINT nSelected=0);//设置图片资源
	void SetRowHeigt(int nHeight); //设置行高
	void SetHeaderHeight(float Height);// 设置表头高度
	void SetHeaderTextColor(COLORREF color);//设置表头颜色
	void SetTextColor(COLORREF color);//设置列表文本颜色
	int InsertColumn(int nCol, LPCTSTR lpszColumnHeading,
		int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);
	int InsertItem(int nItem, LPCTSTR lpszItem);
	void SetHeaderLock(BOOL bLock=TRUE){m_Header.Lock(bLock);}
	void GetSelected(int *nCount);//得到复选框被选中的项目
	void SetRowSelected(int nIndex);//设置某行被选中

	//成员
private:
	HHeaderCtrl				m_Header;
	int						m_nRowHeight;// 行高
	CImage					m_imgBk;
	CImage					m_imgSeleted;
	CImage					m_imgCheckBox;
	COLORREF				m_color;
	int						m_nCurHighlighted;
	bool					m_bCheckBox;
	std::map<int,int>		m_mapCheckMgr;//复选框的勾选管理

	//实现复查
protected:
	int     GetColNum();
	int     SearchSelectIndex();
	CString SearchSelectText();
	void    ReMoveSelect(); 
	int     GetIndexFromPoint();
	CRect   GetShowRect();
	void    UpdataCheckBox(CPoint point,int nHighLight);
	void    UpdateSelected();

	//消息映射
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#endif