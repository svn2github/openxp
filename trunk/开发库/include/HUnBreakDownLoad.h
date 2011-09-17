#ifndef __HUnBreakDownLoader__H__
#define __HUnBreakDownLoader__H__
#pragma once

//为窗口项目预留的消息
//WM_UNBREAKNPTIFYPROCESS 通知当前下载进度
//WM_UNBREAKNPTIFYFINISH  通知任务成功
//WM_UNBREAKNPTIFAILUE    通知任务失败
#ifdef _CONSOLE
#else
	#ifndef WM_UNBREAKNPTIFYPROCESS
	#define WM_UNBREAKNPTIFYPROCESS  WM_USER+1999
	#endif
#endif

#ifdef _CONSOLE
extern "C" int HDOWNLOAD_API HUnBreakDownLoaderAddTask(const char *pURL,
							const char *pSaveDiskPath,
							const char *pSaveFileName,
							const long lMissonID 
							);
#else
extern "C" int HDOWNLOAD_API HUnBreakDownLoaderAddTask(const char *pURL,
							const char *pSaveDiskPath,
							const char *pSaveFileName,
							CWnd *pMsgWnd,
							const long lMissonID
							);
#endif

extern "C" int HDOWNLOAD_API HUnBreakDownLoaderEnd();

#endif