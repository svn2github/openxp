#ifndef __HHTEMLCTRL__H__
#define __HHTEMLCTRL__H__
#pragma once
#include <afxhtml.h>
#include <afxadv.h>			 
#include <afxcview.h>

#define HTML_OPEN_VIEW		0x0001  //ʹ��HtmlView�ڶԻ����д�
#define HTML_OPEN_DEFAULT	0x0002  //ʹ��Ĭ���������
#define HTML_OPEN_DIALOG	0x0003  //ʹ��HtmlDialog��

class TEMPLATE_CONTROL_API HHtmlCtrl : public CHtmlView
{
public:
	HHtmlCtrl();
	virtual ~HHtmlCtrl();
	BOOL CreateFromStatic(UINT nID, CWnd* pParent);
	void OpenURL(LPCTSTR lpszURL,UINT nOpenType=HTML_OPEN_VIEW);

protected:
	virtual void PostNcDestroy();
	afx_msg void OnDestroy();
	afx_msg int  OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT msg);
	DECLARE_MESSAGE_MAP();
	DECLARE_DYNAMIC(HHtmlCtrl)
};

#endif