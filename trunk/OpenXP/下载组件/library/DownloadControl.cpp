#include "StdAfx.h"
#include "DownLoadControl.h"

DownLoadControl::DownLoadControl()
{
	/* 生成头结点 create head node */
	m_pHead = (NODE *)new NODE; 
	m_pHead->info = (void*)new int;
	*(int*)m_pHead->info = 0;
	m_pHead->link = NULL;

	m_pCurDownloadMTR = NULL;
	m_nCurDownloadMTRPos = 0;
}

DownLoadControl::~DownLoadControl()
{
	/* 删除头结点 delete head node */
	if(m_pHead)
	{
		delete m_pHead->info;
		m_pHead->info = NULL;

		delete m_pHead;
		m_pHead = NULL;
	}
}

int DownLoadControl::GetTaskNum()
{
	return *(int*)m_pHead->info;
}

int DownLoadControl::AddTask( char *pszUrl, char *pszSavePath, char *pszSaveFileName, int nThreadNum )
{
	m_csCntl.Lock();

	int nPos = 1;   // 新加入的任务在链表中的位置

	m_pCurDownloadMTR = new DownLoadMTR;
	m_pCurDownloadMTR->SetThreadCount(nThreadNum);

	// 构造一个新结点
	NODE *pNew = new NODE;
	pNew->info = (void*)m_pCurDownloadMTR;
	pNew->link = NULL;

	// 找到最后的位置,插入结点
	NODE *loop = m_pHead;
	while(loop->link)
	{
		loop = loop->link;
		nPos++;
	}
	loop->link = pNew;

	// 结点数增加
	(*(int*)m_pHead->info)++;

	// 开始下载
	int ret = m_pCurDownloadMTR->Download(pszUrl, pszSavePath, pszSaveFileName);

	m_nCurDownloadMTRPos = nPos;

	m_csCntl.Unlock();

	return nPos;
}

int DownLoadControl::SelTask(int nPos)
{
	if(nPos < 1 || nPos > GetTaskNum())
		return -1;  // 选择的任务超出范围

	m_csCntl.Lock();

	if(m_nCurDownloadMTRPos != nPos)
	{
		m_nCurDownloadMTRPos = nPos;

		NODE *loop = m_pHead;
		for(int i = 0; i < m_nCurDownloadMTRPos; i++)
			loop = loop->link;

		m_pCurDownloadMTR = (DownLoadMTR*)loop->info;
	}

	m_csCntl.Unlock();

	return 0;
}

// 返回新指向的任务位置
int DownLoadControl::RemoveTask()
{
	if(0 == m_nCurDownloadMTRPos)
		return -1;

	m_csCntl.Lock();

	NODE *loop = m_pHead;
	for(int i = 1; i < m_nCurDownloadMTRPos; i++)
		loop = loop->link;

	ASSERT((DownLoadMTR*)loop->link->info == m_pCurDownloadMTR);

	// 停止任务并且销毁
	m_pCurDownloadMTR = (DownLoadMTR*)loop->link->info;
	m_pCurDownloadMTR->StopDownload();
	delete m_pCurDownloadMTR;

	// 处理链表
	NODE *p = loop->link;
	loop->link = loop->link->link;
	delete p;

	// 调整当前指向的任务,优先级是[后一个任务,前一个任务]
	if(1 == *(int*)m_pHead->info)  // 如果删除的是最后一个任务
	{
		*(int*)m_pHead->info = 0;
		m_nCurDownloadMTRPos = 0;
		m_pCurDownloadMTR = NULL;
	}
	else
	{
		if(loop->link)  // 存在后一个任务
			m_pCurDownloadMTR = (DownLoadMTR*)loop->link->info;
		else
		{
			m_pCurDownloadMTR = (DownLoadMTR*)loop->info;
			m_nCurDownloadMTRPos--;
		}

		(*(int*)m_pHead->info)--;
	} // End of if( 1 == *(int*)m_pHead->info )

	m_csCntl.Unlock();

	return m_nCurDownloadMTRPos;
}

int DownLoadControl::QueryState( int nPos, 
							  int &nFileTotaleSize, 
							  int &nTotalDownloadedSize,
							  int &nTotalDownloadedSize_ThisTimes,
							  int &nDownloadElapsedTime )
{
	if(nPos < 1 || nPos > GetTaskNum())
		return -1;  // 选择的任务超出范围

	m_csCntl.Lock();

	NODE *loop = m_pHead;
	for(int i = 0; i < nPos; i++)
		loop = loop->link;

	DownLoadMTR *tmp = (DownLoadMTR*)loop->info;
	nDownloadElapsedTime = tmp->GetDownloadElapsedTime();
	nFileTotaleSize = tmp->GetFileTotaleSize();
	if(nFileTotaleSize > 0)
	{
		nTotalDownloadedSize = tmp->GetTotalDownloadedSize();
		nTotalDownloadedSize_ThisTimes = tmp->GetTotalDownloadedSize_ThisTimes();
	}	

	m_csCntl.Unlock();

	return 1;
}