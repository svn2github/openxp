#pragma once

#ifdef __cplusplus
extern "C"{
#endif

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>

#define TME_NONCLIENT				0x00000010
#define WM_NCMOUSELEAVE				0x02A2

// Min thumb size
#define SB_MINTHUMB_SIZE              10

// The scroll timer ID
#define SB_TIMER_DELAY                65533
#define SB_TIMER_SCROLL               65534

// Startup scroll timer delay
#define SB_SCROLL_DELAY               300
#define SB_SCROLL_INTERVAL            40


// Hittest code
#define HTSCROLL_NONE                 0x00000000L
#define HTSCROLL_LINEUP               0x00000001L
#define HTSCROLL_LINEDOWN             0x00000002L
#define HTSCROLL_THUMB                0x00000003L
#define HTSCROLL_PAGEUP               0x00000004L
#define HTSCROLL_PAGEDOWN             0x00000005L
#define HTSCROLL_SIZEBOX              0x00000006L


// status...
#define	SB_STATE_DEFAULT              0x00000000L
#define	SB_STATE_NORMAL               0x00000001L
#define	SB_STATE_HOTTRACKED           0x00000002L
#define	SB_STATE_PRESSED              0x00000003L
#define	SB_STATE_DISABLED             0x00000004L

// Send scroll message
#define DoScrollMsg(hwnd, cmd, pos, fvert) \
	SendMessage((hwnd), (fvert) ? WM_VSCROLL : WM_HSCROLL, MAKEWPARAM((cmd), (pos)), 0);



// ScrollBar data sturcture
typedef struct tagSB {
	HWND             	hwnd;                    // The window handle
	UINT             	flags;                   // Scrollbar disable flags
	DWORD            	style;                   // The window style
	SCROLLINFO       	Horz;                    // Horizontal scroll
	SCROLLINFO       	Vert;                    // Vertical scroll
	WNDPROC          	pfnOldProc;              // The originally window procedure
	BOOL             	fLeftScrollBar;          // Left scrollbar style
	BOOL             	fPreventStyleChange;     // Prevent window style change
	BOOL             	fMouseTracking;
	BOOL             	fTracking;               // Is tracking ?
	BOOL             	fTrackVert;              // Tracking target whether vertical scrollbar
	BOOL                fRichEdit;               // Whether richedit control
	int              	nTrackCode;              // Tracking hittest item code
	int              	nTrackPos;               // Tracking thumb position
	int              	nOffsetPoint;            // Tracking the thumb of mouse point offset
	UINT             	nScrollTimerMsg;         // 32 bit value, low-word -> message, high-word -> scroll command 
	UINT              	nLastCode;               // Last HitTest code
	BOOL             	fLastVert;               // Last HitTest is vertical scrollbar ?
	HBITMAP             hBmp;
} SB, *LPSB;


// Scrollbar size calculate struct
typedef struct tagSBCALC {
	int     pxLeft;
	int     pxTop;
	int     pxRight;
	int     pxBottom;
	union {
		int pxUpArrow;
		int pxLeftArrow;
	};
	union {
		int pxDownArrow;
		int pxRightArrow;
	};
	union {
		int pxThumbTop;
		int pxThumbLeft;
	};
	union {
		int pxThumbBottom;
		int pxThumbRight;
	};
} SBCALC, *LPSBCALC;


// Internal functions
LPSB HScrollBar_GetSB(HWND hwnd);
BOOL HScrollBar_SetSBParms(LPSCROLLINFO psi, SCROLLINFO si, BOOL* pfScroll, int* plres, BOOL bOldPos);
BOOL HScrollBar_IsScrollInfoActive(LPCSCROLLINFO lpsi);
void HScrollBar_SBCalc(LPSB psb, LPSBCALC lpcalc, BOOL fVert);
BOOL HScrollBar_GetSizeBoxRect(LPSB psb, LPRECT lprc);
BOOL HScrollBar_GetScrollBarRect(LPSB psb, BOOL fVert, LPRECT lprc);
BOOL HScrollBar_GetThumbRect(LPSB psb, LPRECT lprc, BOOL fVert);
BOOL HScrollBar_GetGrooveRect(LPSB psb, LPRECT lprc, BOOL fVert);
//int  VSkinScrollBar_GetPosFromPoint(POINT point, BOOL fVert);

// Draw functions
void HScrollBar_DrawScrollBar(LPSB psb, HDC hDC, BOOL fVert);
BOOL HScrollBar_DrawGroove(LPSB psb, HDC hdc, LPRECT lprc, BOOL fVert);
void HScrollBar_DrawThumb(LPSB psb, HDC hdc, BOOL fVert);
BOOL HScrollBar_DrawArrow(LPSB psb, HDC hdc, BOOL fVert, int nArrow, UINT uState);
BOOL HScrollBar_DrawSizeBox(LPSB psb, HDC hDC);
//BOOL VSkinScrollBar_DrawSkinBitmap(HDC hdc, LPRECT lprc, int xSrc, int ySrc, int nWidthSrc, int nHeightSrc,
//						   BOOL bStretchAll, int leftMargin, int topMargin, int rightMargin, int bottomMargin,
//						   BOOL bTrans, COLORREF clrTrans);

//void VSkinScrollBar_TrackLoop(LPSB psb);
void VSkinScrollBar_Track(LPSB psb, BOOL fVert, UINT nHit, POINT pt);
BOOL VSkinScrollBar_TrackThumb(LPSB psb, BOOL fVert, POINT pt);
BOOL VSkinScrollBar_HotTrack(LPSB psb, int nHitCode, BOOL fVert, BOOL fMouseDown);
UINT VSkinScrollBar_HitTest(LPSB psb, BOOL fVert, POINT pt);
BOOL VSkinScrollBar_EnableArrows(LPSB psb, int nBar, UINT wArrows);
UINT VSkinScrollBar_GetDisableFlags(LPSB psb, BOOL fVert);
UINT VSkinScrollBar_GetState(LPSB psb, BOOL fVert, UINT nHit);


// Message functions
LRESULT VSkinScrollBar_OnStyleChanged(LPSB psb, int nStyleType, LPSTYLESTRUCT lpStyleStruct);
LRESULT VSkinScrollBar_OnNcHitTest(LPSB psb, WPARAM wParam, LPARAM lParam);
LRESULT VSkinScrollBar_OnNcPaint(LPSB psb, HRGN hRgn);
LRESULT VSkinScrollBar_OnNcCalcSize(LPSB psb, BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
LRESULT VSkinScrollBar_OnNcMouseMove(LPSB psb, WPARAM wParam, LPARAM lParam);
LRESULT VSkinScrollBar_OnNcLButtonDown(LPSB psb, WPARAM wParam, LPARAM lParam);
LRESULT VSkinScrollBar_OnNcMouseLeave(LPSB psb, WPARAM wParam, LPARAM lParam);
LRESULT VSkinScrollBar_OnMouseMove(LPSB psb, WPARAM wParam, LPARAM lParam);
LRESULT VSkinScrollBar_OnLButtonUp(LPSB psb, WPARAM wParam, LPARAM lParam);
LRESULT VSkinScrollBar_OnTimer(LPSB psb, WPARAM wParam, LPARAM lParam);


// scroll window procedure
LRESULT CALLBACK VSkinScrollBar_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#ifdef __cplusplus
}
#endif