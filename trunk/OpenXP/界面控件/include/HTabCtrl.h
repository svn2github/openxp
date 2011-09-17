#ifndef __HTAB__H__
#define __HTAB__H__
#pragma once
#include <atlimage.h>
#include <vector>
using namespace std;

class UI_CONTROL_API CHTabCtrl : public CWnd
{
	DECLARE_DYNAMIC(CHTabCtrl)

public:
	CHTabCtrl();
	virtual ~CHTabCtrl();
	void CreateSkinTab(const RECT& rect, CWnd* pParentWnd,UINT nGap=5,UINT nHeight=10);
	CWnd* GetActualOwn() const{return m_pParentWnd;}//得到实际的拥有者
	void SetSkinImage(LPCTSTR lpstRes);
	void SetSkinImage(HINSTANCE hInstance,UINT nID);
	void InsertItem(LPCTSTR lpstTitle,UINT nWidth=20);
	void SetBkColor(COLORREF color = RGB(164,229,241));
	void SetCurHot(UINT nIndex = 0);//设置当前高亮的索引
	UINT GetCurHot() const{return m_nCurPush;}//得到当前选中的Tab索引
	void SetLinkWnd(UINT nIndex,CWnd *pWnd);//设置Item关联的窗口
	HWND GetCurHotHWnd() const{return m_CurTabHwnd;}//得到当前选中的窗口句柄
	bool GetFrameRect(CRect &rect);//得到用于显示窗口的区域

protected:
	int GetLeftPoint(int nCurIndex);
	void ShowLinkCWnd(UINT nCurIndex);
	virtual void OnBnChangeTab()=0;//当鼠标左键单击时

protected:
	CImage m_imgRes;//背景资源
	CFont       m_font;//字体
	COLORREF m_bkColor;//背景颜色

private:
	//Item的内容 
	struct ITEM
	{
		LPCTSTR lpstTitle;
		UINT    nWidth;
		ITEM ()
		{
			lpstTitle = _T("");
			nWidth = 67;
		}
	};
	vector<ITEM> m_vecItem;//适配器
	vector<ITEM>::iterator m_vecpItem;//迭代器指针

	//保存每个Item对应的CWnd
	struct ITEM_WND 
	{
		UINT nIndex;
		CWnd *pWnd;
		ITEM_WND ()
		{
			nIndex = 0;
			pWnd = NULL;
		}
	};
	vector<ITEM_WND> m_vecItemWnd;
	vector<ITEM_WND>::iterator m_vecpItemWnd;

private:
	UINT m_nHeight;//Tab高度
	UINT m_nGap;//间隔距离
	UINT  m_nCurPush;//当前选中的索引
	int  m_nCurMove;//当前移动上的索引
	bool m_bInital;//是否是第一次进入绘制
	//父窗口指针
	CWnd * m_pParentWnd;
	HWND m_CurTabHwnd;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#endif
