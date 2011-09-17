#ifndef __DOWNLOADCNTL__H__
#define __DOWNLOADCNTL__H__
#pragma once
#include "DownloadMTR.h"

// ��������
typedef struct node
{
	void *info;
	struct node *link;
}NODE;

class DownLoadControl
{
public:
	DownLoadControl();
	virtual ~DownLoadControl();

	int GetTaskNum();
	int AddTask( char *pszUrl, char *pszSavePath, char *pszSaveFileName, int nThreadNum );
	int SelTask( int nPos );  // ѡ��һ������
	int RemoveTask();		  // ɾ��ѡ�е�����

	// ��ѯһ�������״̬
	int QueryState( int nPos, 
		int &nFileTotaleSize, 
		int &nTotalDownloadedSize,
		int &nTotalDownloadedSize_ThisTimes,
		int &nDownloadElapsedTime ); 

private:
	DownLoadMTR *m_pCurDownloadMTR;
	int m_nCurDownloadMTRPos;
	NODE *m_pHead;   // ָ��ͷ����ָ��,Ϊ��infoָ�����һ���ռ�,����������¼����Ľ�����
	CCriticalSection m_csCntl;  // ���ʹ�����
};

#endif