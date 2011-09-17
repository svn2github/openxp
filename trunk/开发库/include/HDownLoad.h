#ifndef __HDOWNLOAD__H__
#define __HDOWNLOAD__H__
#pragma once

//������   hWndOwn -- ӵ�д���
//����ֵ�� -1:�ظ���ʼ�� 1:��ʼ���ɹ�
extern "C" int HDOWNLOAD_API HDownloaderInit(HWND hWndOwn);// �����ھ��

//������   
//����ֵ�� 
extern "C" void HDOWNLOAD_API HDownloaderEnd();

//������   
//����ֵ�� -1:δ��ʼ��
extern "C" int HDOWNLOAD_API HDownloaderGetTaskNum();

//������  pURL -- �����ļ���ַ  pSavePath -- ����·��  pSaveFileName -- �����ļ���  nThreadNum -- �߳�����
//����ֵ��-1:δ��ʼ�� n:�¼��������ı��
extern "C" int HDOWNLOAD_API HDownloaderAddTask(char *pURL, char *pSavePath, char *pSaveFileName, int nThreadNum);

//������  nIndex -- ���������
//����ֵ��-1:δ��ʼ���������ų�����Χ
extern "C" int HDOWNLOAD_API HDownloaderSelTask(int nIndex); 

//������
//����ֵ��-1:δ��ʼ����������
//���ã�  �Ƴ�ѡ�е�����
extern "C" int HDOWNLOAD_API HDownloaderRemoveTask();		 

//������  nIndex -- ��������  nFileSize -- �����ļ���С nDownloadedSize -- ���صĴ�С
//        nDownloadedSize_ThisTimes -- �������ص�����(��nDownloadedSize��ͬ����Ϊ�п��ܶϵ�֮�������ģ�������һ�����صĴ�С���ڵ��μ���֮��)        
//����ֵ��-1:δ��ʼ���������ų�����Χ
extern "C" int HDOWNLOAD_API HDownloaderQueryState(int nIndex,int &nFileSize,int &nDownloadedSize,int &nDownloadedSize_ThisTimes,int &nSpendTime); 

#endif