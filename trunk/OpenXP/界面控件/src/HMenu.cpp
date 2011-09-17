//*************************************************************************
// HMenu.cpp
//
#include "stdafx.h"
#include "../include/HMenu.h"


//构造函数
HMenu::HMenu()
{
    m_clrText = GetSysColor (COLOR_MENUTEXT);
    m_clrBack = GetSysColor (COLOR_MENU);
    m_brBackground.CreateSolidBrush (m_clrBack);
    m_penBack.CreatePen (PS_SOLID,0,m_clrBack);
    m_crText = m_clrText;
    m_bLBtnDown = FALSE;
    m_nIconX = GetSystemMetrics ( SM_CXMENUCHECK);
    m_nIconY = GetSystemMetrics (SM_CYMENUCHECK );

    m_clrHilight = GetSysColor (COLOR_HIGHLIGHT);
    m_brSelect.CreateSolidBrush (m_clrHilight);
    m_clrHilightText = GetSysColor (COLOR_HIGHLIGHTTEXT);

    ZeroMemory ((PVOID) &m_lf,sizeof (LOGFONT));
    NONCLIENTMETRICS nm;
    nm.cbSize = sizeof (NONCLIENTMETRICS);

    //得到系统的标题属性
    VERIFY (SystemParametersInfo (SPI_GETNONCLIENTMETRICS,0,&nm,0));

    m_lf = nm.lfMenuFont;
    m_nMenuHeight = nm.iMenuHeight;
    m_fontMenu.CreateFontIndirect (&m_lf);
}

//析构函数
HMenu::~HMenu()
{
    if ((HBRUSH) m_brBackground != NULL)
        m_brBackground.DeleteObject ();
    if ((HFONT)m_fontMenu !=NULL)
        m_fontMenu.DeleteObject ();
    if ((HBRUSH)m_brSelect != NULL)
         m_brSelect.DeleteObject ();
}

//绘制
void HMenu::DrawItem (LPDRAWITEMSTRUCT lpDIS)
{
    ASSERT(lpDIS != NULL);

    CDC *pDC = CDC::FromHandle(lpDIS->hDC);
    CRect rect;
    HICON hIcon;
    COLORREF crText = m_crText;
    rect.CopyRect(&lpDIS->rcItem);

	if (lpDIS->itemID == SEPARATOR_HMENU)
	{
		CRect rectTemp = rect;
		rectTemp.left = rectTemp.left +((m_nIconX)?m_nIconX:32);
		pDC->FillSolidRect(rect.left,rect.top,(m_nIconX)?m_nIconX:32,rect.Height(),RGB(232,237,238));
		CPen pen;
		pen.CreatePen(PS_SOLID,1,RGB(181,194,203));
		CPen *pOldPen = pDC->SelectObject(&pen);
		pDC->MoveTo(rectTemp.left,rectTemp.top);
		pDC->LineTo(rect.right,rect.bottom-2);
		pDC->SelectObject(pOldPen);
		pen.DeleteObject();
		return;
	}

    UINT action = lpDIS->itemAction;
    UINT state = lpDIS->itemState;
    CString strText;
    LOGFONT lf;
    lf = m_lf;

    CFont dispFont;
    CFont *pFont;
    if (lpDIS->itemData != NULL)
	{//载入所有资源
        strText = (((HMenuData*)(lpDIS->itemData))->menuText);
		if ((((HMenuData*)(lpDIS->itemData))->menuIconNormal) == -1)
		{
            hIcon = NULL;
		}
		else if (state & ODS_SELECTED)
		{
			if ((((HMenuData *)(lpDIS->itemData))->menuIconSelected) != -1)
			{
				hIcon = AfxGetApp ()->LoadIcon (((HMenuData *)(lpDIS->itemData))->menuIconSelected);
			}
			else
			{
				hIcon = AfxGetApp()->LoadIcon (((HMenuData*)(lpDIS->itemData))->menuIconNormal);
			}               
		}
		else
		{
			hIcon = AfxGetApp()->LoadIcon (((HMenuData*)(lpDIS->itemData))->menuIconNormal); 
		}
            
    }
	else
	{
        strText.Empty();
        hIcon = NULL;
    }

    if ((state & ODS_SELECTED))
	{
        CPen *pOldPen = pDC->SelectObject(&m_penBack);
		if (m_hilightStyle != Normal)
		{
            pDC->FillRect(rect,&m_brBackground);
        }
		else
		{
			if (!(state & ODS_GRAYED))//如果当前目标为灰掉的，这不需要绘制选择框
			{
				pDC->FillRect(rect,&m_brSelect);
			}
        }
        pDC->SelectObject(pOldPen);
        //lf.lfWeight = FW_BOLD;  //加入粗体，这样会看起来有动感效果
		if ((HFONT)dispFont != NULL)
		{
			dispFont.DeleteObject();
		}    
        dispFont.CreateFontIndirect(&lf);
        crText = m_clrHilightText;
    }
	else if ((state & ODS_GRAYED))//如果是灰掉的，这只需要绘制背景框就行
	{
		 pDC->FillRect(rect,&m_brBackground);
	}
	else
	{
        CPen *pOldPen = pDC->SelectObject(&m_penBack);
        pDC->FillRect(rect,&m_brBackground);
        pDC->SelectObject(pOldPen);
		if ((HFONT)dispFont != NULL)
		{
			dispFont.DeleteObject();
		}   
        dispFont.CreateFontIndirect(&lf);
    }

	//绘制图标下面部分的
	pDC->FillSolidRect(rect.left,rect.top,(m_nIconX)?m_nIconX:32,rect.Height(),RGB(232,237,238));
	
	if (hIcon != NULL)
	{
		DrawIconEx(pDC->GetSafeHdc(),rect.left,rect.top,hIcon,(m_nIconX)?m_nIconX:32,(m_nIconY)?m_nIconY:32,0,NULL,DI_NORMAL);
    }
    rect.left = rect.left +((m_nIconX)?m_nIconX:32);

    if (!strText.IsEmpty())
	{
        int iOldMode = pDC->GetBkMode();
        pDC->SetBkMode(TRANSPARENT);
        pDC->SetTextColor(crText);
		if ((state & ODS_GRAYED))
		{
			pDC->SetTextColor(RGB(192,192,192));
		}
        pFont = pDC->SelectObject(&dispFont);
        pDC->DrawText(strText,rect,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
        pDC->SetBkMode(iOldMode );
        pDC->SelectObject (pFont);
    }
    dispFont.DeleteObject();
}

//测量Item
void HMenu::MeasureItem( LPMEASUREITEMSTRUCT lpMIS )
{
    CDC *pDC = AfxGetApp()->m_pMainWnd->GetDC();
    CFont* pFont = pDC->SelectObject (&m_fontMenu);
    int nIconX = 0,nIconY= 0;
    TEXTMETRIC tm;
    pDC->GetTextMetrics (&tm);
    pDC->SelectObject (pFont);
    AfxGetApp()->m_pMainWnd->ReleaseDC (pDC);

	if (m_nIconX)
	{
		nIconX = m_nIconX;
	}
        
	if (m_nIconY)
	{
		nIconY = m_nIconY;
	}
    
	if (lpMIS->itemID == SEPARATOR_HMENU)
	{
		lpMIS->itemHeight = 2;
	}
	else
	{
		lpMIS->itemWidth = nIconX + tm.tmAveCharWidth * lstrlen(((HMenuData*)(lpMIS->itemData))->menuText) +10;
		lpMIS->itemHeight = (nIconY > (m_nMenuHeight+1))?nIconY:m_nMenuHeight + 1;
	}
}

//设置图标大小
void HMenu::SetIconSize (int width, int height)
{
    m_nIconX = width;
    m_nIconY = height;
}

//设置文字颜色
void HMenu::SetTextColor (COLORREF clrText)
{
    m_crText = clrText;
}

//设置背景颜色
void HMenu::SetBackColor (COLORREF clrBack)
{
    m_clrBack = clrBack;
    if ((HBRUSH)m_brBackground != NULL)
        m_brBackground.DeleteObject ();
    m_brBackground.CreateSolidBrush (clrBack);
}

//设置高亮时颜色
void HMenu::SetHighlightColor (COLORREF clrHilight)
{
    m_clrHilight = clrHilight;
    if ((HBRUSH)m_brSelect != NULL)
            m_brSelect.DeleteObject ();
    m_brSelect.CreateSolidBrush (clrHilight);
}

//设置高亮时文字颜色
void HMenu::SetHighlightTextColor (COLORREF clrHilightText)
{
    m_clrHilightText = clrHilightText;
}

//设置高亮方式
void HMenu::SetHighlightStyle (HIGHLIGHTSTYLE hilightStyle)
{
    m_hilightStyle = hilightStyle;
}