#ifndef __ITHREADPOOL__H__
#define __ITHREADPOOL__H__
#pragma once
#include <cassert>
#include <vector>
#include <queue>
#include <windows.h>
#pragma warning(disable: 4530)
#pragma warning(disable: 4786)
using namespace std;

class IUnBreakThreadWork //��������
{
public:
	virtual void DoWork(void *pPara=NULL) = 0;//���̳߳ص��õ��麯��
};

class HUnBreakThreadPool
{
public:
	HUnBreakThreadPool(DWORD nNum=4);//nNum�̳߳ع�ģ
	~HUnBreakThreadPool(void);
	int AdjustSize(int nNum);//�����̳߳ع�ģ
	void Call(void (*pFunc)(void *), void *pPara = NULL);//�����̳߳�
	void Call(IUnBreakThreadWork *p, void *pPara = NULL);//�����̳߳�
	bool EndAndWait(DWORD dwWaitTime = INFINITE);//�����̳߳�, ��ͬ���ȴ�
	inline void End()
	{//�����̳߳�
		SetEvent(m_hEventEnd);
	}
	inline DWORD GetSize() const
	{//�õ��߳���
		return (DWORD)m_nThreadNum;
	}
	inline DWORD GetRunningSize() const
	{//�õ��������е��߳�
		return (DWORD)m_nThreadRunning;
	}
	inline bool IsRunning()
	{//�Ƿ���������
		return m_nThreadNum>0;
	}

protected:
	static DWORD WINAPI DefaultJobProc(LPVOID lpParameter = NULL);//�����߳�
	static void CallProc(void *pPara);//�����û������麯��

private:
	typedef struct tagCallProcessPara //�û�����ṹ
	{
	public:
		tagCallProcessPara(IUnBreakThreadWork *p, void *pPara)
		{
			pObject = p;
			pParam = pPara;
		}
	public:
		IUnBreakThreadWork *pObject;//�û�����
		void *pParam;//�û�����
	}CallProcessPara,*pCallProcessPara;

	typedef struct tagWorkItem//�û����������ṹ
	{
	public:
		tagWorkItem(void (*pFunc)(void *) = NULL, void *pPara = NULL)
		{
			pFunction = pFunc;
			pParam = pPara;
		}
	public:
		void (*pFunction)(void  *);//����
		void *pParam; //����
	}WorkItem,*pWorkItem;

	typedef struct tagThreadItem//�̳߳��е��߳̽ṹ
	{
	public:
		tagThreadItem(HUnBreakThreadPool *p)
		{
			pThreadPool = p;
			hThread = NULL;
			nLastBeginTime = 0;
			nCount = 0;
			bIsRunning = false;
		} 
		~tagThreadItem()
		{
			if(hThread)
			{
				CloseHandle(hThread);
				hThread = NULL;
			}
		}
	public:
		HANDLE hThread; //�߳̾��
		HUnBreakThreadPool *pThreadPool;//�̳߳ص�ָ��
		DWORD nLastBeginTime;//���һ�����п�ʼʱ��
		DWORD nCount; //���д���
		bool bIsRunning;//�Ƿ���������
	}ThreadItem,*pThreadItem;

protected:
	volatile long m_nThreadNum;//�߳�����
	volatile long m_nThreadRunning;//���е��߳���
	queue<pWorkItem> m_queWorkItem; //��������
	vector<pThreadItem>  m_vecThreadItem; //�߳�����
	CRITICAL_SECTION m_csThreadVector;//���������ٽ�
	CRITICAL_SECTION m_csWorkQueue;//�߳������ٽ�
	HANDLE m_hEventEnd;//����֪ͨ
	HANDLE m_hEventComplete;//����¼�
	HANDLE m_hSemaphoreCall;//�����ź�
	HANDLE m_hSemaphoreDel;//ɾ���߳��ź�
};

#endif