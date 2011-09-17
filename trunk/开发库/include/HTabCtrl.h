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
	CWnd* GetActualOwn() const{return m_pParentWnd;}//�õ�ʵ�ʵ�ӵ����
	void SetSkinImage(LPCTSTR lpstRes);
	void SetSkinImage(HINSTANCE hInstance,UINT nID);
	void InsertItem(LPCTSTR lpstTitle,UINT nWidth=20);
	void SetBkColor(COLORREF color = RGB(164,229,241));
	void SetCurHot(UINT nIndex = 0);//���õ�ǰ����������
	UINT GetCurHot() const{return m_nCurPush;}//�õ���ǰѡ�е�Tab����
	void SetLinkWnd(UINT nIndex,CWnd *pWnd);//����Item�����Ĵ���
	HWND GetCurHotHWnd() const{return m_CurTabHwnd;}//�õ���ǰѡ�еĴ��ھ��
	bool GetFrameRect(CRect &rect);//�õ�������ʾ���ڵ�����

protected:
	int GetLeftPoint(int nCurIndex);
	void ShowLinkCWnd(UINT nCurIndex);
	virtual void OnBnChangeTab()=0;//������������ʱ

protected:
	CImage m_imgRes;//������Դ
	CFont       m_font;//����
	COLORREF m_bkColor;//������ɫ

private:
	//Item������ 
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
	vector<ITEM> m_vecItem;//������
	vector<ITEM>::iterator m_vecpItem;//������ָ��

	//����ÿ��Item��Ӧ��CWnd
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
	UINT m_nHeight;//Tab�߶�
	UINT m_nGap;//�������
	UINT  m_nCurPush;//��ǰѡ�е�����
	int  m_nCurMove;//��ǰ�ƶ��ϵ�����
	bool m_bInital;//�Ƿ��ǵ�һ�ν������
	//������ָ��
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
