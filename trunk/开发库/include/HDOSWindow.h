#ifndef _DBWINDOW_
#define _DBWINDOW_
#pragma once
#include <windows.h>

//����̨���ʱ���ı���ɫ
#define WDS_T_RED    FOREGROUND_RED
#define WDS_T_GREEN   FOREGROUND_GREEN
#define WDS_T_BLUE   FOREGROUND_BLUE
#define WDS_T_INTENSITY  FOREGROUND_INTENSITY
//����̨���ʱ���ı�������ɫ
#define WDS_BG_RED   BACKGROUND_RED
#define WDS_BG_GREEN BACKGROUND_GREEN
#define WDS_BG_BLUE   BACKGROUND_BLUE
#define WDS_BG_INTENSITY BACKGROUND_INTENSITY 

#ifdef _DEBUG

//���ÿ���̨������ڱ���
extern "C" BOOL DEBUGGING_CONTROL_API HWindowTile(LPCTSTR tile);
//��ʽ���ı����
extern "C" BOOL DEBUGGING_CONTROL_API HWindowWrite(LPCTSTR fmt,...);
//����ɫ��ʽ���ı����
extern "C" BOOL DEBUGGING_CONTROL_API HWindowWriteW(WORD Attrs,LPCTSTR fmt,...);

//�����(�����ǰ���ĵ����ơ��к�)
extern "C" BOOL DEBUGGING_CONTROL_API HWindowDocument(const char* file,UINT nLine);

//���ù��λ��
extern "C" void DEBUGGING_CONTROL_API HWindowSetPos(int nx,int ny);

#else

#define HWindowTile
#define HWindowWrite
#define HWindowWriteW
#define HWindowDocument
#define HWindowSetPos

#endif

#endif