#ifndef __HSCROLLBAR_H__
#define __HSCROLLBAR_H__
#pragma once
#include <windows.h>

#ifdef __cplusplus
extern "C"{
#endif

extern "C" BOOL UI_CONTROL_API WINAPI HookSetup();
extern "C" BOOL UI_CONTROL_API WINAPI HookDestroy();
extern "C" BOOL UI_CONTROL_API WINAPI HScrollBar(HWND hwnd, HBITMAP hBmp);
extern "C" BOOL UI_CONTROL_API WINAPI HScrollBar_Uninit(HWND hwnd);
extern "C" BOOL UI_CONTROL_API WINAPI hScrollBar_IsValid(HWND hwnd);
extern "C" BOOL UI_CONTROL_API WINAPI HScrollBar_GetScrollInfo(HWND hwnd, int fnBar, LPSCROLLINFO lpsi);
extern "C" int  UI_CONTROL_API WINAPI HScrollBar_SetScrollInfo(HWND hwnd, int fnBar, LPCSCROLLINFO psi, BOOL fRedraw);
extern "C" int  UI_CONTROL_API WINAPI HScrollBar_GetScrollPos(HWND hwnd, int fnBar);
extern "C" int  UI_CONTROL_API WINAPI HScrollBar_SetScrollPos(HWND hwnd, int nBar, int nPos, BOOL fRedraw);
extern "C" BOOL UI_CONTROL_API WINAPI HScrollBar_GetScrollRange(HWND hwnd, int nBar, LPINT lpMinPos, LPINT lpMaxPos);
extern "C" BOOL UI_CONTROL_API WINAPI HScrollBar_SetScrollRange(HWND hwnd, int nBar, int nMinPos, int nMaxPos, BOOL fRedraw);
extern "C" BOOL UI_CONTROL_API WINAPI HScrollBar_ShowScrollBar(HWND hwnd, int wBar, BOOL fShow);
extern "C" BOOL UI_CONTROL_API WINAPI HScrollBar_EnableScrollBar(HWND hwnd, UINT wSBflags, UINT wArrows);

#ifdef __cplusplus
}
#endif


#endif	//__SKINSCROLLBAR_H__