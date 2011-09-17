#ifndef __HLISTCTRL__H__
#define __HLISTCTRL__H__
#pragma once
#include "HHeaderCtrl.h"
#include <atlimage.h>
#include <map>

#define WM_HITCARLISTCTRL WM_USER+1500 //���ĳһ��ʱ������Ϣ

class UI_CONTROL_API HListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(HListCtrl)

public:
	HListCtrl();
	virtual ~HListCtrl();

	//�ⲿ�ӿ�
public:
	void LoadResource(LPCTSTR lpstHeader,LPCTSTR lpstBk,LPCTSTR lpstCheckBox=NULL,LPCTSTR lpstSelected=NULL);//����ͼƬ��Դ
	void LoadResource(HINSTANCE hInstance,UINT nHeader,UINT nBk,UINT nCheckBox=0,UINT nSelected=0);//����ͼƬ��Դ
	void SetRowHeigt(int nHeight); //�����и�
	void SetHeaderHeight(float Height);// ���ñ�ͷ�߶�
	void SetHeaderTextColor(COLORREF color);//���ñ�ͷ��ɫ
	void SetTextColor(COLORREF color);//�����б��ı���ɫ
	int InsertColumn(int nCol, LPCTSTR lpszColumnHeading,
		int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);
	int InsertItem(int nItem, LPCTSTR lpszItem);
	void SetHeaderLock(BOOL bLock=TRUE){m_Header.Lock(bLock);}
	void GetSelected(int *nCount);//�õ���ѡ��ѡ�е���Ŀ
	void SetRowSelected(int nIndex);//����ĳ�б�ѡ��

	//��Ա
private:
	HHeaderCtrl				m_Header;
	int						m_nRowHeight;// �и�
	CImage					m_imgBk;
	CImage					m_imgSeleted;
	CImage					m_imgCheckBox;
	COLORREF				m_color;
	int						m_nCurHighlighted;
	bool					m_bCheckBox;
	std::map<int,int>		m_mapCheckMgr;//��ѡ��Ĺ�ѡ����

	//ʵ�ָ���
protected:
	int     GetColNum();
	int     SearchSelectIndex();
	CString SearchSelectText();
	void    ReMoveSelect(); 
	int     GetIndexFromPoint();
	CRect   GetShowRect();
	void    UpdataCheckBox(CPoint point,int nHighLight);
	void    UpdateSelected();

	//��Ϣӳ��
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