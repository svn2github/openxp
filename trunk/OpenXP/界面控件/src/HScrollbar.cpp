#include "stdafx.h"
#include "../resource.h"
#include "../include/internal.h"
#include "../include/HScrollbar.h"
#include "../include/detours.h"

#define WIN32_LEAN_AND_MEAN

static TCHAR g_szPropSB[] = TEXT("PROP_HScrollBar");
CBitmap g_bmpScrollBar;
static DWORD g_dwHookRefers = 0;
static POINT ptArray[6][6] =
{
	{ {0,  0}, {17,  0}, {34,  0}, {51,  0}, {68,  0}, {85,  0} },
	{ {0, 17}, {17, 17}, {34, 17}, {51, 17}, {68, 17}, {85, 17} },
	{ {0, 34}, {17, 34}, {34, 34}, {51, 34}, {68, 34}, {85, 34} },
	{ {0, 51}, {17, 51}, {34, 51}, {51, 51}, {68, 51}, {85, 51} },
	{ {0, 68}, {17, 68}, {34, 68}, {51, 68}, {68, 68}, {85, 68} },
	{ {0, 85}, {17, 85}, {34, 85}, {51, 85}, {68, 85}, {85, 85} }
};

DETOUR_TRAMPOLINE(int   WINAPI SetScrollInfoT(HWND, int, LPCSCROLLINFO, BOOL), SetScrollInfo)
DETOUR_TRAMPOLINE(BOOL  WINAPI GetScrollInfoT(HWND, int, LPSCROLLINFO), GetScrollInfo)
DETOUR_TRAMPOLINE(int   WINAPI SetScrollPosT(HWND, int, int, BOOL), SetScrollPos)
DETOUR_TRAMPOLINE(int   WINAPI GetScrollPosT(HWND, int), GetScrollPos)
DETOUR_TRAMPOLINE(BOOL  WINAPI GetScrollRangeT(HWND, int, LPINT, LPINT), GetScrollRange)
DETOUR_TRAMPOLINE(BOOL  WINAPI SetScrollRangeT(HWND, int, int, int, BOOL), SetScrollRange)
DETOUR_TRAMPOLINE(BOOL  WINAPI ShowScrollBarT(HWND, int, BOOL), ShowScrollBar)
DETOUR_TRAMPOLINE(BOOL  WINAPI EnableScrollBarT(HWND, UINT, UINT), EnableScrollBar)

int WINAPI SetScrollInfoD(HWND hwnd, int fnBar, LPCSCROLLINFO lpsi, BOOL bRedraw)
{
	if( hScrollBar_IsValid(hwnd) )
		return HScrollBar_SetScrollInfo(hwnd, fnBar, lpsi, bRedraw);
	else
		return SetScrollInfoT(hwnd, fnBar, lpsi, bRedraw);
}

BOOL WINAPI GetScrollInfoD(HWND hwnd, int fnBar, LPSCROLLINFO lpsi)
{
	if( hScrollBar_IsValid(hwnd) )
		return HScrollBar_GetScrollInfo(hwnd, fnBar, lpsi);
	else
		return GetScrollInfoT(hwnd, fnBar, lpsi);
}

int WINAPI SetScrollPosD(HWND hwnd, int nBar, int nPos, BOOL bRedraw)
{
	if( hScrollBar_IsValid(hwnd) )
		return HScrollBar_SetScrollPos(hwnd, nBar, nPos, bRedraw);
	else
		return SetScrollPosT(hwnd, nBar, nPos, bRedraw);
}

int WINAPI GetScrollPosD(HWND hwnd, int nBar)
{
	if( hScrollBar_IsValid(hwnd) )
		return HScrollBar_GetScrollPos(hwnd, nBar);
	else
		return GetScrollPosT(hwnd, nBar);
}

BOOL WINAPI SetScrollRangeD(HWND hwnd, int nBar, int nMinPos, int nMaxPos, BOOL bRedraw)
{
	if( hScrollBar_IsValid(hwnd) )
		return HScrollBar_SetScrollRange(hwnd, nBar, nMinPos, nMaxPos, bRedraw);
	else
		return SetScrollRangeT(hwnd, nBar, nMinPos, nMaxPos, bRedraw);
}

BOOL WINAPI GetScrollRangeD(HWND hwnd, int nBar, LPINT lpMinPos, LPINT lpMaxPos)
{
	if( hScrollBar_IsValid(hwnd) )
		return HScrollBar_GetScrollRange(hwnd, nBar, lpMinPos, lpMaxPos);
	else
		return GetScrollRangeT(hwnd, nBar, lpMinPos, lpMaxPos);
}

BOOL WINAPI ShowScrollBarD(HWND hwnd, int nBar, BOOL bShow)
{
	if( hScrollBar_IsValid(hwnd) )
		return HScrollBar_ShowScrollBar(hwnd, nBar, bShow);
	else
		return ShowScrollBarT(hwnd, nBar, bShow);
}

BOOL WINAPI EnableScrollBarD(HWND hwnd, UINT wSBflags, UINT wArrows)
{
	if( hScrollBar_IsValid(hwnd) )
		return HScrollBar_EnableScrollBar(hwnd, wSBflags, wArrows);
	else
		return EnableScrollBarT(hwnd, wSBflags, wArrows);
}

BOOL WINAPI HookSetup()
{
	DetourFunctionWithTrampoline((PBYTE)SetScrollInfoT, (PBYTE)SetScrollInfoD);
	DetourFunctionWithTrampoline((PBYTE)GetScrollInfoT, (PBYTE)GetScrollInfoD);
	DetourFunctionWithTrampoline((PBYTE)SetScrollPosT, (PBYTE)SetScrollPosD);
	DetourFunctionWithTrampoline((PBYTE)GetScrollPosT, (PBYTE)GetScrollPosD);
	DetourFunctionWithTrampoline((PBYTE)SetScrollRangeT, (PBYTE)SetScrollRangeD);
	DetourFunctionWithTrampoline((PBYTE)GetScrollRangeT, (PBYTE)GetScrollRangeD);
	DetourFunctionWithTrampoline((PBYTE)ShowScrollBarT, (PBYTE)ShowScrollBarD);
	DetourFunctionWithTrampoline((PBYTE)EnableScrollBarT, (PBYTE)EnableScrollBarD);

	return TRUE;
}

BOOL WINAPI HookDestroy()
{
	DetourRemove((PBYTE)SetScrollInfoT, (PBYTE)SetScrollInfoD);
	DetourRemove((PBYTE)SetScrollPosT, (PBYTE)SetScrollPosD);
	DetourRemove((PBYTE)GetScrollInfoT, (PBYTE)GetScrollInfoD);
	DetourRemove((PBYTE)GetScrollPosT, (PBYTE)GetScrollPosD);
	DetourRemove((PBYTE)SetScrollRangeT, (PBYTE)SetScrollRangeD);
	DetourRemove((PBYTE)GetScrollRangeT, (PBYTE)GetScrollRangeD);
	DetourRemove((PBYTE)ShowScrollBarT, (PBYTE)ShowScrollBarD);
	DetourRemove((PBYTE)EnableScrollBarT, (PBYTE)EnableScrollBarD);

	return TRUE;
}

BOOL WINAPI HScrollBar(HWND hwnd, HBITMAP hBmp)
{
	if ( !hBmp && !g_bmpScrollBar.Detach() )
	{
		g_bmpScrollBar.LoadBitmap(IDB_SCROLLBAR);
		hBmp = (HBITMAP)g_bmpScrollBar;
	}

	if ( !g_dwHookRefers ) HookSetup();
	g_dwHookRefers++;

	LPSB    psb;
	DWORD   dwStyle;

	if( !IsWindow(hwnd) ) {
		SetLastError(ERROR_INVALID_HANDLE);
		return FALSE;
	}

	if( hScrollBar_IsValid(hwnd) )
		return FALSE;

	psb = (LPSB)LocalAlloc(LPTR, sizeof(SB));
	if( psb == NULL )
		return FALSE;

	ZeroMemory(psb, sizeof(SB));

	dwStyle = GetWindowLong(hwnd, GWL_STYLE);

	psb->Horz.cbSize = psb->Vert.cbSize = sizeof(SCROLLINFO);
	psb->Horz.fMask  = psb->Vert.fMask  = SIF_ALL;

	if( dwStyle & WS_HSCROLL )
		GetScrollInfo(hwnd, SB_HORZ, &psb->Horz);

	if( dwStyle & WS_VSCROLL )
		GetScrollInfo(hwnd, SB_VERT, &psb->Vert);
	
	if( GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_LEFTSCROLLBAR )
		psb->fLeftScrollBar = TRUE;

	if( !hBmp ) {
		SetLastError(ERROR_INVALID_HANDLE);
		return FALSE;
	}

	psb->hBmp = hBmp;
	psb->hwnd = hwnd;
	psb->pfnOldProc = (WNDPROC)(LONG_PTR)SetWindowLong(hwnd, 
		GWL_WNDPROC, (LONG)(LONG_PTR)VSkinScrollBar_Proc);

	psb->fPreventStyleChange = FALSE;
	psb->fTracking = FALSE;
	TCHAR szClassName[255] = { 0 };
	GetClassName(hwnd, szClassName, sizeof(szClassName));
	if( _strcmpi(szClassName, TEXT("RichEdit20A")) == 0 ||
		_strcmpi(szClassName, TEXT("RichEdit20W")) == 0 ) {
		psb->fRichEdit = TRUE;
	}
	
	if( !SetProp(hwnd, g_szPropSB, (HANDLE)psb) )
		return FALSE;

	SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOZORDER |
            SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_DRAWFRAME);

	return TRUE;
}

BOOL WINAPI HScrollBar_Uninit(HWND hwnd)
{
	LPSB        psb;
	SCROLLINFO  vsi;
	SCROLLINFO  hsi;
	BOOL        vValid, hValid;
	UINT        vFlags, hFlags;

	if( (psb = HScrollBar_GetSB(hwnd)) == NULL )
		return FALSE;

	vsi.cbSize = hsi.cbSize = sizeof(SCROLLINFO);
	vsi.fMask  = hsi.fMask = SIF_ALL | SIF_DISABLENOSCROLL;

	vValid = HScrollBar_GetScrollInfo(hwnd, SB_VERT, &vsi);
	vFlags = VSkinScrollBar_GetDisableFlags(psb, TRUE);
	hValid = HScrollBar_GetScrollInfo(hwnd, SB_HORZ, &hsi);
	hFlags = VSkinScrollBar_GetDisableFlags(psb, FALSE);

	SetWindowLong(hwnd, GWL_WNDPROC, (LONG)(LONG_PTR)psb->pfnOldProc);
	RemoveProp(hwnd, g_szPropSB);
	LocalFree((HLOCAL)psb);

	if( vValid ) {
		SetScrollInfo(hwnd, SB_VERT, &vsi, TRUE);
		EnableScrollBar(hwnd, SB_VERT, vFlags);
	}

	if( hValid ) {
		SetScrollInfo(hwnd, SB_HORZ, &hsi, TRUE);
		EnableScrollBar(hwnd, SB_HORZ, hFlags);
	}

	if ( g_dwHookRefers > 1 ) g_dwHookRefers--;
	else HookDestroy();

	return TRUE;
}

BOOL WINAPI hScrollBar_IsValid(HWND hwnd)
{
	return (HScrollBar_GetSB(hwnd) != NULL);
}

BOOL WINAPI HScrollBar_GetScrollInfo(HWND hwnd, int fnBar, LPSCROLLINFO lpsi)
{
	LPSB psb;
	LPSCROLLINFO psi;
	BOOL fCopied = FALSE;

	if( !lpsi || lpsi->cbSize != sizeof(SCROLLINFO) )
		return FALSE;

	if( (psb = HScrollBar_GetSB(hwnd)) == NULL )
		return FALSE;

	if( fnBar == SB_HORZ )
		psi = &psb->Horz;
	else if( fnBar == SB_VERT )
		psi = &psb->Vert;
	else if( fnBar == SB_CTL ) 
		return FALSE;

	if( lpsi->fMask & SIF_PAGE ) {
		lpsi->nPage = psi->nPage;
		fCopied = TRUE;
	}

	if( lpsi->fMask & SIF_POS ) {
		lpsi->nPos  = psi->nPos;
		fCopied = TRUE;
	}

	if( lpsi->fMask & SIF_TRACKPOS ) {
		lpsi->nTrackPos = psi->nTrackPos;
		fCopied = TRUE;
	}

	if( lpsi->fMask & SIF_RANGE ) {
		lpsi->nMin = psi->nMin;
		lpsi->nMax = psi->nMax;
		fCopied = TRUE;
	}

	return fCopied;
}

int WINAPI HScrollBar_SetScrollInfo(HWND hwnd, int fnBar, LPCSCROLLINFO psi, BOOL fRedraw)
{
	LPSB            psb;
	LPSCROLLINFO    mysi;
	int             nRet;
	DWORD           dwStyle;
	BOOL            fVert;
	UINT            wScroll;
	BOOL            fScroll;
	BOOL            fOldScroll;
	BOOL            bReturnOldPos;

	if( fnBar == SB_CTL )
		return 0;

 	if( (psb = HScrollBar_GetSB(hwnd)) == NULL )
		return 0;

	if( fRedraw )
		fRedraw = IsWindowVisible(hwnd);

	fVert         = (fnBar == SB_VERT);
	bReturnOldPos = (psi->fMask & SIF_POS);
	dwStyle       = GetWindowLong(hwnd, GWL_STYLE);
	wScroll       = fVert ? WS_VSCROLL : WS_HSCROLL;

	fScroll = fOldScroll = (dwStyle & wScroll) ? TRUE : FALSE;
	
	if( !(psi->fMask & SIF_RANGE) && !fOldScroll )
		return 0;

	mysi = (fVert ? &psb->Vert : &psb->Horz);

	if( !HScrollBar_SetSBParms(mysi, *psi, &fScroll, &nRet, bReturnOldPos) &&
		!(psi->fMask & SIF_DISABLENOSCROLL) ) {
		if( fOldScroll && fRedraw )
			goto redrawAfterSet;
		return nRet;
	}

	if( fScroll )
		psb->style |= wScroll;
	else
		psb->style &= ~wScroll;

	SetWindowLong(hwnd, GWL_STYLE, dwStyle | wScroll);

	if( psi->fMask & SIF_DISABLENOSCROLL ) {
		if( fOldScroll ) {
			fScroll = ((int)mysi->nPage <= (mysi->nMax - mysi->nMin));
			psb->style |= wScroll;
			SetWindowLong(hwnd, GWL_STYLE, dwStyle | wScroll);
			VSkinScrollBar_EnableArrows(psb, fnBar, fScroll ? ESB_ENABLE_BOTH : ESB_DISABLE_BOTH);
		}
	}
	else if( fOldScroll ^ fScroll ) {
		SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOZORDER |
            SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_DRAWFRAME);
		return nRet;
	}

	if( fScroll && fRedraw ) {

redrawAfterSet:
		if(dwStyle & wScroll) {
			HDC hDC = GetWindowDC(hwnd);
			HScrollBar_DrawThumb(psb, hDC, fVert);
			ReleaseDC(hwnd, hDC);
		}
		else {
			if( HScrollBar_IsScrollInfoActive(psi) )
				SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOZORDER |
				SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_DRAWFRAME);
		}
	}

	return mysi->nPos;
}

int WINAPI HScrollBar_GetScrollPos(HWND hwnd, int nBar)
{
	LPSB psb;
	int nPos;
	if( !(psb = HScrollBar_GetSB(hwnd)) )
		return FALSE;

	if( nBar == SB_HORZ )
		nPos = psb->Horz.nPos;
	else if( nBar == SB_VERT )
		nPos = psb->Vert.nPos;

	return nPos;
}

int WINAPI HScrollBar_SetScrollPos(HWND hwnd, int nBar, int nPos, BOOL fRedraw)
{
	LPSB psb;
	LPSCROLLINFO psi;
	int nOldPos;

	if( (psb = HScrollBar_GetSB(hwnd)) == NULL)
		return FALSE;

	if( nBar == SB_HORZ )
		psi = &psb->Horz;
	else if( nBar == SB_VERT )
		psi = &psb->Vert;
	else
		return FALSE;

	nOldPos = psi->nPos;
	psi->nPos = nPos;

	if( fRedraw ) {
		HDC hDC = GetWindowDC(hwnd);
		HScrollBar_DrawScrollBar(psb, hDC, (nBar == SB_VERT) );
		ReleaseDC(hwnd, hDC);
	}

	return nOldPos;
}

BOOL WINAPI HScrollBar_GetScrollRange(HWND hwnd, int nBar, LPINT lpMinPos, LPINT lpMaxPos)
{
	LPSB psb;
	LPSCROLLINFO psi;
	if( !lpMinPos || !lpMaxPos )
		return FALSE;

	if( (psb = HScrollBar_GetSB(hwnd)) == NULL )
		return FALSE;

	if( nBar == SB_HORZ )
		psi = &psb->Horz;
	else if( nBar == SB_VERT )
		psi = &psb->Vert;

	*lpMinPos = psi->nMin;
	*lpMaxPos = psi->nMax;

	return TRUE;
}

BOOL WINAPI HScrollBar_SetScrollRange(HWND hwnd, int nBar, int nMinPos, int nMaxPos, BOOL fRedraw)
{
	LPSB psb;
	LPSCROLLINFO psi;

	if( !(psb = HScrollBar_GetSB(hwnd)) )
		return FALSE;

	if( nBar == SB_HORZ )
		psi = &psb->Horz;
	else if( nBar == SB_VERT )
		psi = &psb->Vert;

	psi->nMin = nMinPos;
	psi->nMax = nMaxPos;

	if(nMinPos == 0 && nMaxPos == 0)
		HScrollBar_ShowScrollBar(hwnd, nBar, FALSE);

	if( fRedraw ) {
		HDC hDC = GetWindowDC(hwnd);
		HScrollBar_DrawScrollBar(psb, hDC, (nBar == SB_VERT) );
		ReleaseDC(hwnd, hDC);
	}

	return TRUE;
}

BOOL WINAPI HScrollBar_ShowScrollBar(HWND hwnd, int wBar, BOOL fShow)
{
	BOOL fChanged = FALSE;
	DWORD dwStyle, dwNew;

	switch( wBar )
	{
	case SB_CTL:
		ShowWindow(hwnd, fShow ? SW_SHOW : SW_HIDE);
		break;
	case SB_HORZ:
		dwNew = WS_HSCROLL;
		break;
	case SB_VERT:
		dwNew = WS_VSCROLL;
		break;
	case SB_BOTH:
		dwNew = WS_HSCROLL | WS_VSCROLL;
		break;
	}

	dwStyle = GetWindowLong(hwnd, GWL_STYLE);
	if( !fShow ) {
		if( dwStyle & dwNew ) {
			fChanged = TRUE;
			dwStyle &= ~dwNew;
		}
	}
	else {
		if( (dwStyle & dwNew) != dwNew ) {
			fChanged = TRUE;
			dwStyle |= dwNew;
		}
	}

	if( fChanged ) {
		SetWindowLong(hwnd, GWL_STYLE, dwStyle);
		SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOZORDER |
            SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_DRAWFRAME);
	}

	return TRUE;
}

BOOL WINAPI HScrollBar_EnableScrollBar(HWND hwnd, UINT wSBflags, UINT wArrows)
{
	LPSB psb;
	if( !(psb = HScrollBar_GetSB(hwnd)) )
		return FALSE;

	if( wSBflags == SB_CTL )
		return FALSE;
	
	return VSkinScrollBar_EnableArrows(psb, wSBflags, wArrows);
}

BOOL HScrollBar_SetSBParms(LPSCROLLINFO psi, SCROLLINFO si, BOOL* pfScroll, int* plres, BOOL bOldPos)
{
	BOOL fChanged = FALSE;
	if( bOldPos )
		*plres = psi->nPos;

	if( si.fMask & SIF_RANGE ) {
		if( si.nMax < si.nMin )
			si.nMax = si.nMin;

		if( si.nMax != psi->nMax || si.nMin != psi->nMin ) {
			psi->nMax = si.nMax;
			psi->nMin = si.nMin;

			if( !(si.fMask & SIF_PAGE) ) {
				si.fMask |= SIF_PAGE;
				si.nPage = psi->nPage;
			}

			if( !(si.fMask & SIF_POS) ) {
				si.fMask |= SIF_POS;
				si.nPos  = psi->nPos;
			}

			fChanged = TRUE;
		}
	}

	if( si.fMask & SIF_PAGE ) {
		UINT nMaxPage = abs( psi->nMax - psi->nMin ) + 1;
		if( si.nPage > nMaxPage )
			si.nPage = nMaxPage;
		
		if( psi->nPage != si.nPage ) {
			psi->nPage = si.nPage;

			if( !(si.fMask & SIF_POS) ) {
				si.fMask |= SIF_POS;
				si.nPos  = psi->nPos;
			}

			fChanged = TRUE;
		}
	}

	if( si.fMask & SIF_POS ) {
		int nMaxPos = psi->nMax - ((psi->nPage) ? psi->nPage - 1 : 0);
		if( si.nPos < psi->nMin )
			si.nPos = psi->nMin;
		else if( si.nPos > nMaxPos )
			si.nPos = nMaxPos;

		if( psi->nPos != si.nPos ) {
			psi->nPos = si.nPos;
			fChanged = TRUE;
		}
	}

	if( si.fMask & SIF_TRACKPOS ) {
		if( psi->nTrackPos != si.nTrackPos ) {
			psi->nTrackPos = si.nTrackPos;
			fChanged = TRUE;
		}
	}

	if( !bOldPos )
		*plres = psi->nPos; // Return the new pos

	if( si.fMask & SIF_RANGE ) {
		if( *pfScroll = (psi->nMin != psi->nMax) )
			goto CheckPage;
	}
	else if( si.fMask & SIF_PAGE ) {
CheckPage:
		*pfScroll = ( (int)psi->nPage <= (psi->nMax - psi->nMin) );
	}

	return fChanged;
}

BOOL VSkinScrollBar_EnableArrows(LPSB psb, int nBar, UINT wArrows)
{
	HDC hDC;
	UINT uOldFlags;
	BOOL bRetValue = FALSE;

	if( psb == NULL )
		return FALSE;

	uOldFlags = psb->flags;
	if( (hDC = GetWindowDC(psb->hwnd)) == NULL )
		return FALSE;

	// Enable or disable horizontal scrollbar
	if( nBar == SB_HORZ || nBar == SB_BOTH ) {
		if( wArrows == ESB_ENABLE_BOTH )
			psb->flags &= ~ESB_DISABLE_BOTH;
		else
			psb->flags |= wArrows;

		if( uOldFlags != psb->flags ) {
			bRetValue = TRUE;
			if( GetWindowLong(psb->hwnd, GWL_STYLE ) & WS_HSCROLL )
				HScrollBar_DrawScrollBar(psb, hDC, FALSE);
		}
	}

	if( nBar == SB_VERT || nBar == SB_BOTH ) {
		if( wArrows == ESB_ENABLE_BOTH )
			psb->flags &= ~(ESB_DISABLE_BOTH << 2);
		else
			psb->flags |= (wArrows << 2);

		if( uOldFlags != psb->flags ) {
			bRetValue = TRUE;
			if( GetWindowLong(psb->hwnd, GWL_STYLE ) & WS_VSCROLL )
				HScrollBar_DrawScrollBar(psb, hDC, TRUE);
		}
	}

	ReleaseDC(psb->hwnd, hDC);

	return bRetValue;
}

UINT VSkinScrollBar_GetDisableFlags(LPSB psb, BOOL fVert)
{
	return (fVert ? (psb->flags & 0x000c) >> 2 : psb->flags & 0x0003); 
}

LPSB HScrollBar_GetSB(HWND hwnd)
{
	return (LPSB)GetProp(hwnd, g_szPropSB);
}

BOOL HScrollBar_IsScrollInfoActive(LPCSCROLLINFO lpsi)
{
	if( lpsi->nPage > (UINT)lpsi->nMax || lpsi->nMax <= lpsi->nMin || lpsi->nMax == 0 )
		return FALSE;
	else
		return TRUE;
}

BOOL HScrollBar_GetSizeBoxRect(LPSB psb, LPRECT lprc)
{
	DWORD dwStyle;
	RECT  rect;

	SetRectEmpty(lprc);
	dwStyle = GetWindowLong(psb->hwnd, GWL_STYLE);
	if( (dwStyle & WS_HSCROLL) && (dwStyle & WS_VSCROLL) ) {
		GetClientRect(psb->hwnd, &rect);
		ClientToScreen(psb->hwnd, (LPPOINT)&rect);
		ClientToScreen(psb->hwnd, ((LPPOINT)&rect)+1);

		if( psb->fLeftScrollBar ) {
			lprc->left = rect.left - GetSystemMetrics(SM_CXVSCROLL);
			lprc->right = rect.left;
		}
		else {
			lprc->left = rect.right;
			lprc->right = rect.right + GetSystemMetrics(SM_CXVSCROLL);
		}

		lprc->top   = rect.bottom;
		lprc->bottom= rect.bottom + GetSystemMetrics(SM_CYHSCROLL);
		return TRUE;
	}

	return FALSE;
}

BOOL HScrollBar_GetScrollBarRect(LPSB psb, BOOL fVert, LPRECT lprc)
{
	RECT   rect;
	DWORD  dwStyle;

	if( !psb || !lprc )
		return FALSE;
	SetRectEmpty(lprc);
	GetClientRect(psb->hwnd, &rect);
	ClientToScreen(psb->hwnd, (LPPOINT)&rect);
	ClientToScreen(psb->hwnd, ((LPPOINT)&rect)+1);

	dwStyle = GetWindowLong(psb->hwnd, GWL_STYLE);

	if( fVert ) {
		if( psb->fLeftScrollBar ) {
			lprc->right = lprc->left = rect.left;
			if( dwStyle & WS_VSCROLL )
				lprc->left -= GetSystemMetrics(SM_CXVSCROLL);
		}
		else {
			lprc->right = lprc->left = rect.right;
			if( dwStyle & WS_VSCROLL )
				lprc->right += GetSystemMetrics(SM_CXVSCROLL);
		}
		lprc->top = rect.top;
		lprc->bottom = rect.bottom;
	}
	else {
		lprc->top = lprc->bottom = rect.bottom;
		if( dwStyle & WS_HSCROLL )
			lprc->bottom += GetSystemMetrics(SM_CYHSCROLL);
		lprc->left = rect.left;
		lprc->right = rect.right;
	}

	return TRUE;
}

BOOL HScrollBar_GetThumbRect(LPSB psb, LPRECT lprc, BOOL fVert)
{
	if( !psb || !lprc )
		return FALSE;
	SBCALC   sbc;
	RECT     rect;
	HScrollBar_SBCalc(psb, &sbc, fVert);
	GetWindowRect(psb->hwnd, &rect);
	if( fVert )
		SetRect(lprc, sbc.pxLeft, sbc.pxThumbTop, sbc.pxRight, sbc.pxThumbBottom);
	else
		SetRect(lprc, sbc.pxThumbLeft, sbc.pxTop, sbc.pxThumbRight, sbc.pxBottom);

	OffsetRect(lprc, -rect.left, -rect.top);
	return TRUE;
}

BOOL HScrollBar_GetGrooveRect(LPSB psb, LPRECT lprc, BOOL fVert)
{
	if( !psb || !lprc )
		return FALSE;
	SBCALC   sbc;
	RECT     rect;
	HScrollBar_SBCalc(psb, &sbc, fVert);
	GetWindowRect(psb->hwnd, &rect);
	if( fVert )
		SetRect(lprc, sbc.pxLeft, sbc.pxUpArrow, sbc.pxRight, sbc.pxDownArrow);
	else
		SetRect(lprc, sbc.pxUpArrow, sbc.pxTop, sbc.pxDownArrow, sbc.pxBottom);
	OffsetRect(lprc, -rect.left, -rect.top);
	return TRUE;
}

BOOL HScrollBar_DrawSizeBox(LPSB psb, HDC hDC)
{
	RECT rect, rc;
	GetWindowRect(psb->hwnd, &rect);
	HScrollBar_GetSizeBoxRect(psb, &rc);
	OffsetRect(&rc, -rect.left, -rect.top);

	//egamesir 2009.11.17
	HDC hSrcDC   = CreateCompatibleDC(hDC);
	HBITMAP hOldBmp  = (HBITMAP)SelectObject(hSrcDC, psb->hBmp);

	HDC memDC = CreateCompatibleDC(hDC);
	HBITMAP hMemBitmap = CreateCompatibleBitmap(hDC, rc.right-rc.left, rc.bottom-rc.top);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, hMemBitmap);

	BitBlt(memDC, 0, 0, 16, 16, hSrcDC, ptArray[2][5].x, ptArray[2][5].y, SRCCOPY);

	BitBlt(hDC, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, memDC, 0,0, SRCCOPY);

	SelectObject(memDC, hOldBitmap);
	DeleteDC(memDC);
	DeleteObject(hMemBitmap);

	SelectObject(hSrcDC, hOldBmp);
	DeleteDC(hSrcDC);

	return TRUE;
}

BOOL HScrollBar_DrawGroove(LPSB psb, HDC hdc, LPRECT lprc, BOOL fVert)
{
	if( !hdc || !lprc || IsRectEmpty(lprc) )
		return FALSE;

	HDC      hbmpDC;
	HBITMAP  hOldBmp;
	POINT    pt;
	int      nMode;
	hbmpDC = CreateCompatibleDC(hdc);
	hOldBmp = (HBITMAP)SelectObject(hbmpDC, psb->hBmp);

	if( fVert )
		pt = ptArray[2][5];
	else
		pt = ptArray[3][5];

	nMode = SetStretchBltMode(hdc, HALFTONE);
	StretchBlt(hdc, lprc->left, lprc->top, lprc->right - lprc->left, 
		lprc->bottom - lprc->top, hbmpDC, pt.x, pt.y, 16, 16, SRCCOPY);
	SetStretchBltMode(hdc, nMode);

	SelectObject(hbmpDC, hOldBmp);
	DeleteDC(hbmpDC);

	return TRUE;
}

void HScrollBar_DrawScrollBar(LPSB psb, HDC hDC, BOOL fVert)
{
	RECT rcGroove;

	HScrollBar_GetGrooveRect(psb, &rcGroove, fVert);
	//HScrollBar_DrawGroove(psb, hDC, &rcGroove, fVert);		//·ÀÖ¹ÉÁË¸ egamesir 2009.10.20

	if( fVert ) {
		HScrollBar_DrawArrow(psb, hDC, fVert, HTSCROLL_LINEUP, 0);
		HScrollBar_DrawArrow(psb, hDC, fVert, HTSCROLL_LINEDOWN, 0);
		
		if( !HScrollBar_IsScrollInfoActive(&psb->Vert) )
			return ;
		HScrollBar_DrawThumb(psb, hDC, fVert);
	}
	else {
		HScrollBar_DrawArrow(psb, hDC, fVert, HTSCROLL_LINEUP, 0);
		HScrollBar_DrawArrow(psb, hDC, fVert, HTSCROLL_LINEDOWN, 0);

		if( !HScrollBar_IsScrollInfoActive(&psb->Horz) )
			return ;
		HScrollBar_DrawThumb(psb, hDC, fVert);
	}
}

BOOL HScrollBar_DrawArrow(LPSB psb, HDC hdc, BOOL fVert, int nArrow, UINT uState)
{
	RECT      	rect, rc;
	SBCALC    	sbc;
	HDC       	hMemDC;
	HBITMAP   	hOldBmp;
	int       	x, y;
	POINT     	pt;

	GetWindowRect(psb->hwnd, &rect);
	HScrollBar_SBCalc(psb, &sbc, fVert);

	if( uState == 0 )
		uState = VSkinScrollBar_GetState(psb, fVert, nArrow);

	hMemDC  = CreateCompatibleDC(hdc);
	hOldBmp = (HBITMAP)SelectObject(hMemDC, psb->hBmp);

	switch( nArrow )
	{
	case HTSCROLL_LINEUP:
		if( fVert ) {
			x = 0; y = 0;
			SetRect(&rc, sbc.pxLeft, sbc.pxTop, sbc.pxRight, sbc.pxUpArrow);
		}
		else {
			x = 3; y = 0;
			SetRect(&rc, sbc.pxLeft, sbc.pxTop, sbc.pxUpArrow, sbc.pxBottom);
		}
		break;
	case HTSCROLL_LINEDOWN:
		if( fVert ) {
			x = 0; y = 1;
			SetRect(&rc, sbc.pxLeft, sbc.pxDownArrow, sbc.pxRight, sbc.pxBottom);
		}
		else {
			x = 3; y = 1;
			SetRect(&rc, sbc.pxDownArrow, sbc.pxTop, sbc.pxRight, sbc.pxBottom);
		}
		break;
	}

	OffsetRect(&rc, -rect.left, -rect.top);

	switch( uState )
	{
	case SB_STATE_NORMAL:
		break;
	case SB_STATE_HOTTRACKED:
		x += 1;
		break;
	case SB_STATE_PRESSED:
		x += 2;
		break;
	case SB_STATE_DISABLED:
		break;
	}
	pt = ptArray[x][y];

	BitBlt(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hMemDC, pt.x, pt.y, SRCCOPY);

	SelectObject(hMemDC, hOldBmp);
	DeleteDC(hMemDC);

	return TRUE;
	
}

void HScrollBar_DrawThumb(LPSB psb, HDC hdc, BOOL fVert)
{
	SBCALC   	sbc;
	RECT     	rc, rect;
	HDC      	hSrcDC;
	HDC      	memDC;
	HBITMAP  	hOldBmp;
	HBITMAP  	hMemBitmap, hOldBitmap;
	UINT     	uState;
	int      	cx, cy;
	POINT    	pt, pt1;
	
	uState = VSkinScrollBar_GetState(psb, fVert, HTSCROLL_THUMB);

	HScrollBar_SBCalc(psb, &sbc, fVert);
	GetWindowRect(psb->hwnd, &rect);

	if( sbc.pxTop >= sbc.pxBottom || sbc.pxLeft >= sbc.pxRight )
		return ;

	if( (sbc.pxDownArrow - sbc.pxUpArrow) < (sbc.pxThumbBottom - sbc.pxThumbTop) ||
		uState == SB_STATE_DISABLED ) {
		HScrollBar_GetGrooveRect(psb, &rc, fVert);
		HScrollBar_DrawGroove(psb, hdc, &rc, fVert);
		return ;
	}

	if( sbc.pxUpArrow < sbc.pxThumbTop ) {
		if( fVert )
			SetRect(&rc, sbc.pxLeft, sbc.pxUpArrow,  sbc.pxRight, sbc.pxThumbTop);
		else
			SetRect(&rc, sbc.pxUpArrow, sbc.pxTop, sbc.pxThumbTop, sbc.pxBottom);
		OffsetRect(&rc, -rect.left, -rect.top);
		HScrollBar_DrawGroove(psb, hdc, &rc, fVert);
	}

	if( sbc.pxThumbBottom < sbc.pxDownArrow ) {
		if( fVert )
			SetRect(&rc, sbc.pxLeft, sbc.pxThumbBottom, sbc.pxRight, sbc.pxDownArrow);
		else
			SetRect(&rc, sbc.pxThumbBottom, sbc.pxTop, sbc.pxDownArrow, sbc.pxBottom);
		OffsetRect(&rc, -rect.left, -rect.top);
		HScrollBar_DrawGroove(psb, hdc, &rc, fVert);
	}

	hSrcDC   = CreateCompatibleDC(hdc);
	hOldBmp  = (HBITMAP)SelectObject(hSrcDC, psb->hBmp);

	HScrollBar_GetThumbRect(psb, &rc, fVert);
	cx = rc.right - rc.left;
	cy = rc.bottom - rc.top;
	RECT rcMemDC = {0, 0, rc.right-rc.left, rc.bottom-rc.top};

	memDC = CreateCompatibleDC(hdc);
	hMemBitmap = CreateCompatibleBitmap(hdc, cx, cy);
	hOldBitmap = (HBITMAP)SelectObject(memDC, hMemBitmap);
	SetBkColor(memDC, 0xFFFFFF);
	ExtTextOut(memDC, 0, 0, ETO_OPAQUE, &rcMemDC, NULL, 0, NULL);

	POINT pt2;
	switch( uState )
	{
	case SB_STATE_NORMAL:
		pt  = fVert ? ptArray[0][2] : ptArray[3][2];
		pt1 = fVert ? ptArray[0][3] : ptArray[3][3];
		pt2 = fVert ? ptArray[0][4] : ptArray[3][4];
		break;
	case SB_STATE_HOTTRACKED:
		pt  = fVert ? ptArray[1][2] : ptArray[4][2];
		pt1 = fVert ? ptArray[1][3] : ptArray[4][3];
		pt2 = fVert ? ptArray[1][4] : ptArray[4][4];
		break;
	case SB_STATE_PRESSED:
		pt  = fVert ? ptArray[2][2] : ptArray[5][2];
		pt1 = fVert ? ptArray[2][3] : ptArray[5][3];
		pt2 = fVert ? ptArray[2][4] : ptArray[5][4];
		break;
	case SB_STATE_DISABLED:
		break;
	}

	if ( fVert ) {
		for( int i= 4; i < cy - 4; i += 8 )
			BitBlt(memDC, 0,  i, cx, 8, hSrcDC, pt.x, pt.y + 4, SRCCOPY);

		BitBlt(memDC, 0, 0, cx, 4, hSrcDC, pt.x, pt.y, SRCCOPY);
		BitBlt(memDC, 0, cy - 4, cx, 4, hSrcDC, pt2.x, (pt2.y + 16) - 4, SRCCOPY);
		if( cy > 16 + 8 ) {
			int y = (cy - 16) / 2;
			//BitBlt(memDC, 0, y, cx, 16, hSrcDC, ptArray[0][5].x, ptArray[0][5].y, SRCCOPY);
			::TransparentBlt(memDC, 0, y, 17, 17, hSrcDC, ptArray[1][5].x, ptArray[1][5].y, 17, 17,RGB(255,0,255));
		}
	}
	else {
		for( int i=4; i < cx - 4; i += 8 )
			BitBlt(memDC, i, 0, 8, cy, hSrcDC, pt.x + 4, pt.y, SRCCOPY);
		BitBlt(memDC, 0, 0, 4, cy, hSrcDC, pt.x, pt.y, SRCCOPY);
		BitBlt(memDC, cx - 4, 0, 4, cy, hSrcDC, (pt2.x + 16) - 4, pt2.y, SRCCOPY);
		if( cx > 16 + 8 ) {
			int x = (cx - 16) / 2;
			//BitBlt(memDC, x, 0, 16, cy, hSrcDC, pt1.x, pt1.y, SRCCOPY);
			::TransparentBlt(memDC, x, 0, 17, 17, hSrcDC, ptArray[0][5].x, ptArray[0][5].y, 17, 17,RGB(255,0,255));
		}
	}

	BitBlt(hdc, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, memDC, 0,0, SRCCOPY);
	
	SelectObject(memDC, hOldBitmap);
	DeleteDC(memDC);
	DeleteObject(hMemBitmap);

	SelectObject(hSrcDC, hOldBmp);
	DeleteDC(hSrcDC);
}

UINT VSkinScrollBar_HitTest(LPSB psb, BOOL fVert, POINT pt)
{
	SBCALC  sbc;
	UINT    disFlags;
	int     x;

	HScrollBar_SBCalc(psb, &sbc, fVert);

	if( pt.x < sbc.pxLeft  || pt.y < sbc.pxTop || 
		pt.x > sbc.pxRight || pt.y > sbc.pxBottom )
		return HTSCROLL_NONE;

	disFlags = VSkinScrollBar_GetDisableFlags(psb, fVert);
	x = fVert ? pt.y : pt.x;

	if( x < sbc.pxUpArrow ) {
		if( disFlags & ESB_DISABLE_BOTH || disFlags & ESB_DISABLE_LTUP )
			return HTSCROLL_NONE;
		else
			return HTSCROLL_LINEUP;
	}
	else if( x > sbc.pxUpArrow && x < sbc.pxThumbTop ) {
		if( disFlags & ESB_DISABLE_BOTH || disFlags & ESB_DISABLE_LTUP )
			return HTSCROLL_NONE;
		else
			return HTSCROLL_PAGEUP;
	}
	else if( (x >= sbc.pxThumbTop && x <= sbc.pxThumbBottom) &&
		(sbc.pxThumbTop > 0 && sbc.pxThumbBottom > sbc.pxThumbTop) ) {
		return HTSCROLL_THUMB;
	}
	else if( x > sbc.pxThumbBottom && x < sbc.pxDownArrow ) {
		if( disFlags & ESB_DISABLE_BOTH || disFlags & ESB_DISABLE_RTDN )
			return HTSCROLL_NONE;
		else
			return HTSCROLL_PAGEDOWN;
	}
	else if( x >= sbc.pxDownArrow ) {
		if( disFlags & ESB_DISABLE_BOTH || disFlags & ESB_DISABLE_RTDN )
			return HTSCROLL_NONE;
		else
			return HTSCROLL_LINEDOWN;
	}

	return HTSCROLL_NONE;
}

void HScrollBar_SBCalc(LPSB psb, LPSBCALC lpcalc, BOOL fVert)
{
	RECT        	rcBar;
	int         	nRange;
	int         	nWorkingsize;
	int         	nArrowsize;
	int         	nThumbsize;
	int         	nStart;
	int         	nThumbpos;
	SCROLLINFO* 	psi;

	HScrollBar_GetScrollBarRect(psb, fVert, &rcBar);

	lpcalc->pxLeft   = rcBar.left;
	lpcalc->pxTop    = rcBar.top;
	lpcalc->pxRight  = rcBar.right;
	lpcalc->pxBottom = rcBar.bottom;

	if( fVert ) {
		psi = &psb->Vert;
		nArrowsize   = GetSystemMetrics(SM_CYVSCROLL);
		nWorkingsize = (rcBar.bottom - rcBar.top) - nArrowsize * 2;
		nStart = rcBar.top + nArrowsize;
		
		lpcalc->pxUpArrow = rcBar.top + nArrowsize;
		lpcalc->pxDownArrow = rcBar.bottom - nArrowsize;
	}
	else {
		psi = &psb->Horz;
		nArrowsize   = GetSystemMetrics(SM_CXHSCROLL);
		nWorkingsize = (rcBar.right - rcBar.left) - nArrowsize * 2;
		nStart = rcBar.left + nArrowsize;

		lpcalc->pxUpArrow = rcBar.left + nArrowsize;
		lpcalc->pxDownArrow = rcBar.right - nArrowsize;
	}

	nRange = (psi->nMax - psi->nMin) + 1;
	if( nRange > 0 && HScrollBar_IsScrollInfoActive(psi)) {
		nThumbsize = MulDiv(psi->nPage, nWorkingsize, nRange);
		if( nThumbsize < SB_MINTHUMB_SIZE )
			nThumbsize = SB_MINTHUMB_SIZE;
		int pagesize = max(1, psi->nPage);

		nThumbpos = MulDiv(psi->nPos - psi->nMin, nWorkingsize - nThumbsize, nRange - pagesize);
		
		if( nThumbpos < 0 )
			nThumbpos = 0;

		if( nThumbpos >= nWorkingsize - nThumbsize )
			nThumbpos =  nWorkingsize - nThumbsize;

		nThumbpos += nStart;
		lpcalc->pxThumbTop = nThumbpos;
		lpcalc->pxThumbBottom = nThumbpos + nThumbsize;
	}
	else {
		lpcalc->pxThumbTop = 0;
		lpcalc->pxThumbBottom = 0;
	}	
}

BOOL VSkinScrollBar_TrackThumb(LPSB psb, BOOL fVert, POINT pt)
{
	SBCALC        	sbclc;
	int           	nPos;
	int           	nThumbpos;
	int           	nRange;
	int           	nThumbsize;
	int           	nWorksize;
	LPSCROLLINFO  	psi;

	HScrollBar_SBCalc(psb, &sbclc, fVert);
	
	if( fVert ) {
		psi = &psb->Vert;
		nThumbpos = pt.y -  psb->nOffsetPoint;
	}
	else {
		psi = &psb->Horz;
		nThumbpos = pt.x - psb->nOffsetPoint;
	}

	nPos       = 0;
	nThumbpos -= sbclc.pxUpArrow;
	nThumbsize = sbclc.pxThumbBottom - sbclc.pxThumbTop;
	nWorksize  = sbclc.pxDownArrow - sbclc.pxUpArrow;
	nRange     = (psi->nMax - psi->nMin) + 1;

	if( nThumbpos < 0 )
		nThumbpos = 0;
	if( nThumbpos > nWorksize - nThumbsize)
		nThumbpos = nWorksize - nThumbsize;

	if( nRange > 0 )
		nPos = MulDiv(nThumbpos, nRange - psi->nPage, nWorksize - nThumbsize);

	if( psi->nPos != nPos ) {
		psi->nTrackPos = nPos;
		psb->nTrackPos = nPos;
		if( psb->fRichEdit ) {
			DoScrollMsg(psb->hwnd, SB_THUMBPOSITION, nPos, fVert);
		}
		else {
			DoScrollMsg(psb->hwnd, SB_THUMBTRACK, nPos, fVert);
		}
	}

	return TRUE;
}

UINT VSkinScrollBar_GetState(LPSB psb, BOOL fVert, UINT nHit)
{
	BOOL 	fHotTracked;
	BOOL 	fPressed;
	BOOL 	fDisabled;
	UINT 	disFlags;
	UINT 	state;

	if( nHit == HTSCROLL_NONE )
		return 0;

	fHotTracked = fPressed = fDisabled = FALSE;

	disFlags = VSkinScrollBar_GetDisableFlags(psb, fVert);
	switch( nHit ) {
	case HTSCROLL_LINEUP:
		fDisabled = (disFlags & ESB_DISABLE_BOTH || disFlags & ESB_DISABLE_UP);
		break;
	case HTSCROLL_LINEDOWN:
		fDisabled = (disFlags & ESB_DISABLE_BOTH || disFlags & ESB_DISABLE_DOWN);
		break;
	case HTSCROLL_THUMB:
		fDisabled = (disFlags & ESB_DISABLE_BOTH);
		break;
	}

	if( nHit == psb->nLastCode && fVert == psb->fLastVert )
		fHotTracked = TRUE;

	if( !fDisabled && psb->fTracking && fHotTracked )
		fPressed = TRUE;

	if( fDisabled )
		state = SB_STATE_DISABLED;
	else if( fPressed )
		state = SB_STATE_PRESSED;
	else if( fHotTracked )
		state = SB_STATE_HOTTRACKED;
	else
		state = SB_STATE_NORMAL;
	
	return state;
}

void VSkinScrollBar_Track(LPSB psb, BOOL fVert, UINT nHit, POINT pt)
{
	UINT            disFlags;
	LPSCROLLINFO    psi;
	WORD            wSBCode;

	psi = fVert ? &psb->Vert : &psb->Horz;
	disFlags = VSkinScrollBar_GetDisableFlags(psb, fVert);

	switch( nHit )
	{
	case HTSCROLL_THUMB:
		SBCALC sbclc;
		HScrollBar_SBCalc(psb, &sbclc, fVert);
		psi->nTrackPos    = psi->nPos;
		psb->nOffsetPoint = (fVert ? pt.y : pt.x) - sbclc.pxThumbTop;
		break;

	case HTSCROLL_LINEUP:
		wSBCode = SB_LINEUP;
		goto DoScroll;

	case HTSCROLL_LINEDOWN:
		wSBCode = SB_LINEDOWN;
		goto DoScroll;

	case HTSCROLL_PAGEDOWN:
		wSBCode = SB_PAGEDOWN;
		goto DoScroll;

	case HTSCROLL_PAGEUP:
		wSBCode = SB_PAGEUP;
DoScroll:
		psb->nScrollTimerMsg = MAKELONG(fVert ? WM_VSCROLL : WM_HSCROLL, wSBCode);
		DoScrollMsg(psb->hwnd, wSBCode, 0, fVert);
		SetTimer(psb->hwnd, SB_TIMER_DELAY, SB_SCROLL_DELAY, NULL);
		break;
	default:
		return;
	}

	psb->nTrackCode  = nHit;
	psb->fTrackVert  = fVert;
	psb->fTracking   = TRUE;

	VSkinScrollBar_HotTrack(psb, nHit, fVert, TRUE);
	SetCapture(psb->hwnd);

	//VSkinScrollBar_TrackLoop(psb);
}

/*
//----------------------------------------------------------
// Name : VSkinScrollBar_TrackLoop()
// Desc :
//----------------------------------------------------------
void VSkinScrollBar_TrackLoop(LPSB psb)
{
	HWND    hwnd;
	MSG     msg;
	int     cmd;
	//POINT   pt;

	if( !psb->fTracking )
		return;
	hwnd = psb->hwnd;
	while( GetCapture() == hwnd )
	{
		if( !GetMessage(&msg, hwnd, 0, 0) )
			break;

		if( !CallMsgFilter(&msg, MSGF_SCROLLBAR) )
		{
			cmd = msg.message;

			if( msg.hwnd == hwnd &&
				((cmd >= WM_MOUSEFIRST && cmd <= WM_MOUSELAST) ||
				(cmd >= WM_KEYFIRST && cmd <= WM_KEYLAST))
				)
			{
				TRACE("message loop\n");
				VSkinScrollBar_Proc(hwnd, cmd, msg.wParam, msg.lParam);
			}
		}
		else
		{
			TranslateMessage(&msg);
            DispatchMessage(&msg);
		}
	}
}*/

BOOL VSkinScrollBar_HotTrack(LPSB psb, int nHitCode, BOOL fVert, BOOL fMouseDown)
{
	HDC  	hDC;
	UINT 	oldHit;
	BOOL    oldVert;

	oldHit = psb->nLastCode;
	oldVert = psb->fLastVert;

	psb->nLastCode = nHitCode;
	psb->fLastVert = fVert;

	if( (hDC = GetWindowDC(psb->hwnd)) == NULL )
		return FALSE;

	if( nHitCode != oldHit || fVert != oldVert  || fMouseDown) {
		if( oldHit == HTSCROLL_LINEUP )
			HScrollBar_DrawArrow(psb, hDC, oldVert, HTSCROLL_LINEUP, SB_STATE_NORMAL);
		else if( oldHit == HTSCROLL_LINEDOWN )
			HScrollBar_DrawArrow(psb, hDC, oldVert, HTSCROLL_LINEDOWN, SB_STATE_NORMAL);
		else if( oldHit == HTSCROLL_THUMB )
			HScrollBar_DrawThumb(psb, hDC, oldVert);
	
		if( nHitCode == HTSCROLL_LINEUP )
			HScrollBar_DrawArrow(psb, hDC, fVert, HTSCROLL_LINEUP, fMouseDown ? SB_STATE_PRESSED : SB_STATE_HOTTRACKED);
		else if( nHitCode == HTSCROLL_LINEDOWN )
			HScrollBar_DrawArrow(psb, hDC, fVert, HTSCROLL_LINEDOWN, fMouseDown ? SB_STATE_PRESSED : SB_STATE_HOTTRACKED);
		else if( nHitCode == HTSCROLL_THUMB)
			HScrollBar_DrawThumb(psb, hDC, fVert);
	}

	ReleaseDC(psb->hwnd, hDC);
	
	return TRUE;
}

LRESULT VSkinScrollBar_OnStyleChanged(LPSB psb, int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	if( psb->fPreventStyleChange )
		return 0;

	if( nStyleType == GWL_EXSTYLE ) {
		BOOL fOld = psb->fLeftScrollBar;
		if( lpStyleStruct->styleNew & WS_EX_LEFTSCROLLBAR )
			psb->fLeftScrollBar = TRUE;
		else
			psb->fLeftScrollBar = FALSE;

		if( fOld != psb->fLeftScrollBar )
			SetWindowPos(psb->hwnd, NULL, 0, 0, 0, 0, SWP_NOZORDER |
				SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_DRAWFRAME);
	}

	return CallWindowProc(psb->pfnOldProc, psb->hwnd, WM_STYLECHANGED, nStyleType, (LPARAM)lpStyleStruct);
}

LRESULT VSkinScrollBar_OnNcCalcSize(LPSB psb, BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	LRESULT   lr;
	DWORD     dwStyle;
	RECT*     lprc, rect;
	
	lprc = &lpncsp->rgrc[0];
	rect = *lprc;

	dwStyle = GetWindowLong(psb->hwnd, GWL_STYLE);
	
	if( dwStyle & (WS_VSCROLL | WS_HSCROLL) ) {
		psb->fPreventStyleChange = TRUE;
		SetWindowLong(psb->hwnd, GWL_STYLE, dwStyle & ~(WS_VSCROLL|WS_HSCROLL));
	}

	lr = CallWindowProc(psb->pfnOldProc, psb->hwnd,
		WM_NCCALCSIZE, (WPARAM)bCalcValidRects, (LPARAM)lpncsp);

	if( dwStyle & (WS_VSCROLL | WS_HSCROLL) ) {
		SetWindowLong(psb->hwnd, GWL_STYLE, dwStyle);
		psb->fPreventStyleChange = FALSE;
	}

	if( (dwStyle & WS_HSCROLL) && 
		(lprc->bottom - lprc->top) > GetSystemMetrics(SM_CYHSCROLL) ) {
		lprc->bottom -= GetSystemMetrics(SM_CYHSCROLL);
	}

	if( (dwStyle & WS_VSCROLL) &&
		(lprc->right - lprc->left) > GetSystemMetrics(SM_CXVSCROLL) ) {
		if( psb->fLeftScrollBar )
			lprc->left += GetSystemMetrics(SM_CXVSCROLL);
		else
			lprc->right -= GetSystemMetrics(SM_CXVSCROLL);
	}
	return lr;
}

LRESULT VSkinScrollBar_OnNcPaint(LPSB psb, HRGN hRgn)
{
	LRESULT  lr;
	DWORD    dwStyle;
	HDC      hDC;
	
	dwStyle = GetWindowLong(psb->hwnd, GWL_STYLE);
	if( dwStyle & (WS_VSCROLL | WS_HSCROLL) ) {
		psb->fPreventStyleChange = TRUE;
		SetWindowLong(psb->hwnd, GWL_STYLE, dwStyle & ~(WS_VSCROLL|WS_HSCROLL));
	}

	lr = CallWindowProc(psb->pfnOldProc, psb->hwnd, WM_NCPAINT, (WPARAM)hRgn, 0);
	
	if( dwStyle & (WS_VSCROLL | WS_HSCROLL) ) {
		SetWindowLong(psb->hwnd, GWL_STYLE, dwStyle);
		psb->fPreventStyleChange = FALSE;
	}
	
	hDC = GetWindowDC(psb->hwnd);

	ASSERT(hDC);
	ASSERT(psb->hwnd);

	if( dwStyle & (WS_VSCROLL | WS_HSCROLL) )
		HScrollBar_DrawSizeBox(psb, hDC);

	if( dwStyle & WS_VSCROLL )
		HScrollBar_DrawScrollBar(psb, hDC, TRUE);
	
	if( dwStyle & WS_HSCROLL )
		HScrollBar_DrawScrollBar(psb, hDC, FALSE);

	ReleaseDC(psb->hwnd, hDC);

	return lr;
}

LRESULT VSkinScrollBar_OnNcHitTest(LPSB psb, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	RECT rcHorz, rcVert, rcSize;
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);

	HScrollBar_GetScrollBarRect(psb, TRUE, &rcVert);
	HScrollBar_GetScrollBarRect(psb, FALSE, &rcHorz);
	HScrollBar_GetSizeBoxRect(psb, &rcSize);

	if( PtInRect(&rcVert, pt) )
		return HTVSCROLL;
	else if( PtInRect(&rcHorz, pt) )
		return HTHSCROLL;
	else if( PtInRect(&rcSize, pt) )
		return HTSIZE;

	return CallWindowProc(psb->pfnOldProc, psb->hwnd, WM_NCHITTEST, wParam, lParam);
}

LRESULT VSkinScrollBar_OnNcMouseMove(LPSB psb, WPARAM wParam, LPARAM lParam)
{
	if( wParam == HTHSCROLL || wParam == HTVSCROLL ) {
		POINT pt;
		BOOL  fVert;
		int   nHitCode;

		pt.x  = GET_X_LPARAM(lParam); 
		pt.y  = GET_Y_LPARAM(lParam);
		fVert = ( wParam == HTVSCROLL );
		nHitCode  = VSkinScrollBar_HitTest(psb, fVert, pt);
		VSkinScrollBar_HotTrack(psb, nHitCode, fVert, FALSE);
	}
	else if( wParam == HTSIZE ) {
		VSkinScrollBar_HotTrack(psb, HTSCROLL_NONE, FALSE, FALSE);
	}

	return CallWindowProc(psb->pfnOldProc, psb->hwnd, WM_NCMOUSEMOVE, wParam, lParam);
}

LRESULT VSkinScrollBar_OnNcMouseLeave(LPSB psb, WPARAM wParam, LPARAM lParam)
{
	psb->fMouseTracking = FALSE;
	if( psb->fTracking )
		return 0;

	VSkinScrollBar_HotTrack(psb, HTSCROLL_NONE, FALSE, FALSE);
	psb->nLastCode = HTSCROLL_NONE;

	return 0;
}

LRESULT VSkinScrollBar_OnNcLButtonDown(LPSB psb, WPARAM wParam, LPARAM lParam)
{
	if( wParam == HTHSCROLL || wParam == HTVSCROLL ) {
		POINT 			pt;
		BOOL  			fVert;
		int   			nHitCode;
		LPSCROLLINFO    psi;

		pt.x  = GET_X_LPARAM(lParam);
		pt.y  = GET_Y_LPARAM(lParam);
		fVert = (wParam == HTVSCROLL);
		
		psi = (fVert ? &psb->Vert : &psb->Horz);
		nHitCode = VSkinScrollBar_HitTest(psb, fVert, pt);
		psb->nLastCode = nHitCode;
		psb->fLastVert = fVert;
		VSkinScrollBar_Track(psb, fVert, nHitCode, pt);
		return 0;

	}
	return CallWindowProc(psb->pfnOldProc, psb->hwnd, WM_NCLBUTTONDOWN, wParam, lParam);
}

LRESULT VSkinScrollBar_OnMouseMove(LPSB psb, WPARAM wParam, LPARAM lParam)
{
	DWORD   	dwPos;
	POINT   	pt;
	UINT     	nHitCode, nHitTest;
	dwPos = 	GetMessagePos();
	pt.x  = 	GET_X_LPARAM(dwPos);
	pt.y  = 	GET_Y_LPARAM(dwPos);

	if( psb->fTracking && wParam & MK_LBUTTON ) {
		if( psb->nTrackCode == HTSCROLL_THUMB ) {
			VSkinScrollBar_TrackThumb(psb, psb->fTrackVert, pt);
			return 0;
		}

		nHitTest = (UINT)VSkinScrollBar_OnNcHitTest(psb, 0, dwPos);

		if( nHitTest == HTHSCROLL || nHitTest == HTVSCROLL ) {
			BOOL fVert = (nHitTest == HTVSCROLL);
			nHitCode = VSkinScrollBar_HitTest(psb, fVert, pt);

			if( nHitCode != psb->nTrackCode || fVert != psb->fTrackVert) {
				VSkinScrollBar_HotTrack(psb, HTSCROLL_NONE, FALSE, FALSE);
			}
			else {
				VSkinScrollBar_HotTrack(psb, psb->nTrackCode, psb->fTrackVert, TRUE);
			}
		}
		else {
			VSkinScrollBar_HotTrack(psb, HTSCROLL_NONE, FALSE, FALSE);
		}
		return 0;
	}

	return CallWindowProc(psb->pfnOldProc, psb->hwnd, WM_MOUSEMOVE, wParam, lParam);
}

LRESULT VSkinScrollBar_OnLButtonUp(LPSB psb, WPARAM wParam, LPARAM lParam)
{
	if( psb->nTrackCode != HTSCROLL_NONE && psb->fTracking) {
		ReleaseCapture();

		switch( psb->nTrackCode )
		{
		case HTSCROLL_LINEUP:
		case HTSCROLL_LINEDOWN:
		case HTSCROLL_PAGEUP:
		case HTSCROLL_PAGEDOWN:
			KillTimer(psb->hwnd, SB_TIMER_SCROLL);
			DoScrollMsg(psb->hwnd, SB_ENDSCROLL, 0, psb->fTrackVert);
			break;

		case HTSCROLL_THUMB:
			if( psb->fTracking ) {
				DWORD  dwPos;
				POINT  pt;
				dwPos = GetMessagePos();
				pt.x = GET_X_LPARAM(dwPos);
				pt.y = GET_Y_LPARAM(dwPos);
				DoScrollMsg(psb->hwnd, SB_THUMBPOSITION, psb->nTrackPos, psb->fTrackVert);
				DoScrollMsg(psb->hwnd, SB_ENDSCROLL, 0, psb->fTrackVert);
				psb->nLastCode = VSkinScrollBar_HitTest(psb, psb->fTrackVert, pt);
			}
			break;
		}
		
		psb->nOffsetPoint    = 0;
		psb->nScrollTimerMsg = MAKELONG(WM_NULL, 0);
		psb->nTrackCode      = HTSCROLL_NONE;
		psb->fTracking       = FALSE;
		psb->nTrackPos       = 0;

		HDC hdc = GetWindowDC(psb->hwnd);
		HScrollBar_DrawThumb(psb, hdc, psb->fTrackVert);
		ReleaseDC(psb->hwnd, hdc);
		return 0;
	}
	return CallWindowProc(psb->pfnOldProc, psb->hwnd, WM_LBUTTONUP, wParam, lParam);
}

LRESULT VSkinScrollBar_OnTimer(LPSB psb, WPARAM wParam, LPARAM lParam)
{
	
	if(wParam == SB_TIMER_SCROLL) {
		if( psb->nTrackCode == HTSCROLL_NONE ) {
			KillTimer(psb->hwnd, SB_TIMER_SCROLL);
			return 0;
		}

		if( psb->nTrackCode == psb->nLastCode && psb->fTrackVert == psb->fLastVert )
			DoScrollMsg(psb->hwnd, HIWORD(psb->nScrollTimerMsg), 0, psb->fTrackVert);

		return 0;
	}
	else if(wParam == SB_TIMER_DELAY) {
		KillTimer(psb->hwnd, SB_TIMER_DELAY);
		SetTimer(psb->hwnd, SB_TIMER_SCROLL, SB_SCROLL_INTERVAL, 0);
		return 0;
	}
	return CallWindowProc(psb->pfnOldProc, psb->hwnd, WM_TIMER, wParam, lParam);
}

LRESULT CALLBACK VSkinScrollBar_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LPSB psb = HScrollBar_GetSB(hwnd);
	
	if( psb && uMsg == WM_NCMOUSEMOVE ) {
		if(!psb->fMouseTracking && !psb->fTracking) {
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(tme);
			tme.hwndTrack = psb->hwnd;
			tme.dwFlags = TME_LEAVE|TME_NONCLIENT;
			tme.dwHoverTime = 0;
			psb->fMouseTracking = _TrackMouseEvent(&tme);
		}
	}

	switch( uMsg )
	{
	case WM_NCPAINT:
		return VSkinScrollBar_OnNcPaint(psb, (HRGN)wParam);

	case WM_NCCALCSIZE:
		return VSkinScrollBar_OnNcCalcSize(psb, (BOOL)wParam, (NCCALCSIZE_PARAMS*)lParam);

	case WM_NCHITTEST:
		return VSkinScrollBar_OnNcHitTest(psb, wParam, lParam);

	case WM_STYLECHANGED:
		return VSkinScrollBar_OnStyleChanged(psb, (int)wParam, (LPSTYLESTRUCT)lParam);

	case WM_NCLBUTTONDOWN:
		return VSkinScrollBar_OnNcLButtonDown(psb, wParam, lParam);

	case WM_NCMOUSEMOVE:
		return VSkinScrollBar_OnNcMouseMove(psb, wParam, lParam);

	case WM_MOUSEMOVE:
		return VSkinScrollBar_OnMouseMove(psb, wParam, lParam);

	case WM_LBUTTONUP:
		return VSkinScrollBar_OnLButtonUp(psb, wParam, lParam);

	case WM_NCMOUSELEAVE:		
		return VSkinScrollBar_OnNcMouseLeave(psb, wParam, lParam);

    case WM_NCLBUTTONDBLCLK:
	case WM_NCRBUTTONDBLCLK:
		return 0;

	case WM_TIMER:
		return VSkinScrollBar_OnTimer(psb, wParam, lParam);

	case WM_NCDESTROY:
		LRESULT lr = CallWindowProc(psb->pfnOldProc, hwnd, uMsg, wParam, lParam);
		HScrollBar_Uninit(hwnd);
		return lr;
	}
	return CallWindowProc(psb->pfnOldProc, hwnd, uMsg, wParam, lParam);
}