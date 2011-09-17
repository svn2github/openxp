#ifndef _DBWINDOW_
#define _DBWINDOW_
#pragma once
#include <windows.h>

//控制台输出时的文本颜色
#define WDS_T_RED    FOREGROUND_RED
#define WDS_T_GREEN   FOREGROUND_GREEN
#define WDS_T_BLUE   FOREGROUND_BLUE
#define WDS_T_INTENSITY  FOREGROUND_INTENSITY
//控制台输出时的文本背景颜色
#define WDS_BG_RED   BACKGROUND_RED
#define WDS_BG_GREEN BACKGROUND_GREEN
#define WDS_BG_BLUE   BACKGROUND_BLUE
#define WDS_BG_INTENSITY BACKGROUND_INTENSITY 

#ifdef _DEBUG

//设置控制台输出窗口标题
extern "C" BOOL DEBUGGING_CONTROL_API HWindowTile(LPCTSTR tile);
//格式化文本输出
extern "C" BOOL DEBUGGING_CONTROL_API HWindowWrite(LPCTSTR fmt,...);
//带颜色格式化文本输出
extern "C" BOOL DEBUGGING_CONTROL_API HWindowWriteW(WORD Attrs,LPCTSTR fmt,...);

//新添加(输出当前的文档名称、行号)
extern "C" BOOL DEBUGGING_CONTROL_API HWindowDocument(const char* file,UINT nLine);

//设置光标位置
extern "C" void DEBUGGING_CONTROL_API HWindowSetPos(int nx,int ny);

#else

#define HWindowTile
#define HWindowWrite
#define HWindowWriteW
#define HWindowDocument
#define HWindowSetPos

#endif

#endif