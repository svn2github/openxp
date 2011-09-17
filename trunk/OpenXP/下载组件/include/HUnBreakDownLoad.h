#ifndef __HUnBreakDownLoader__H__
#define __HUnBreakDownLoader__H__
#pragma once

//Ϊ������ĿԤ������Ϣ
//WM_UNBREAKNPTIFYPROCESS ֪ͨ��ǰ���ؽ���
//WM_UNBREAKNPTIFYFINISH  ֪ͨ����ɹ�
//WM_UNBREAKNPTIFAILUE    ֪ͨ����ʧ��
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