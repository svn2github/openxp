//*************************************************************************
// HMenu.h
//
#ifndef __HSKINMENU__H__
#define __HSKINMENU__H__
#pragma once

#ifndef _MENUDATA_MAX
#define _MENUDATA_MAX 32
#endif

#define SEPARATOR_HMENU 65535
#define SEPARATOR_TEXT  _T("")

typedef struct tagHMENUDATA
{
public:
    tagHMENUDATA()
	{
		menuIconNormal = -1;
		menuIconSelected = -1;
	};
public:
    char menuText[_MENUDATA_MAX];
    UINT menuIconNormal;
    UINT menuIconSelected;
}HMenuData,*pHMenuData;

typedef enum {Normal,TextOnly} HIGHLIGHTSTYLE;

class UI_CONTROL_API HMenu : public CMenu
{
public:
    HMenu();
	virtual ~HMenu();

public:
    virtual void DrawItem(LPDRAWITEMSTRUCT);
    virtual void MeasureItem(LPMEASUREITEMSTRUCT);
    void SetTextColor (COLORREF);
    void SetBackColor (COLORREF);
    void SetHighlightColor (COLORREF);
    void SetIconSize (int, int);
    void SetHighlightStyle (HIGHLIGHTSTYLE);
    void SetHighlightTextColor (COLORREF);

protected:
    COLORREF m_crText;
    COLORREF m_clrBack;
    COLORREF m_clrText;
    COLORREF m_clrHilight;
    COLORREF m_clrHilightText;
    LOGFONT m_lf;
    CFont m_fontMenu;
    UINT m_nMenuHeight;
    BOOL m_bLBtnDown;
    CBrush m_brBackground,m_brSelect;
    CPen m_penBack;
    int m_nIconX,m_nIconY;
    HIGHLIGHTSTYLE m_hilightStyle;
};

#endif