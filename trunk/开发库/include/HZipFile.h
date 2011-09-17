#ifndef __ZIPFILE__H__
#define __ZIPFILE__H__
#pragma once
#include <map>
#ifndef FILE_CONTROL_CLASS
	#include "zip/zip.h"
	#include "zip/unzip.h"
#endif

//���ļ����ܴ����������ѹ���ļ�
//HZipFile��ʱ�����ã�UnZipFile���Խ�ѹwinRarѹ��������Zip�ļ�
struct ZipEntryInfo
{
	int			m_nIndex;
	long		m_nSize;
	CString	    m_sName;
};

//ZIP�ļ�������
class FILE_CONTROL_API HZipFile
{
public:
	HZipFile(void);
	virtual ~HZipFile(void);
	BOOL ZipCreate(LPCTSTR pZipFileName, LPCTSTR pZipPassword=NULL);
	BOOL ZipFileAdd(LPCTSTR pDestZipName, LPCTSTR pSrcDiskPath);
	void Close();
protected:
	HZIP m_hZip;
};

// UNZIP�ļ�������
class FILE_CONTROL_API HUnzipFile
{
public:  //���е�nIndex����ZIP�ļ����Ӿ��ϵ���������1��ʼ
	HUnzipFile(void);
	virtual ~HUnzipFile(void);

	BOOL Open(LPCTSTR pZipFileName, LPCTSTR pPassword=NULL);
	void Close();//�ر�Zip�ļ����ͷ���Դ

	int GetEntryCount() const;//�õ�Zip�ļ����ļ�������
	LPCTSTR GetItemName(int nIndex);//�õ�Zip�ļ����ļ������� 

	BOOL GetEntryInfo(int nIndex, ZipEntryInfo& info);
	BOOL GetEntryInfo(LPCTSTR pName, ZipEntryInfo& info);
	BOOL UnZipAll(LPCTSTR pDiskDir);//��ѹ���е�����·��
	BOOL Unzip2File(int nIndex, LPCTSTR pDestDiskDir=NULL);
	BOOL Unzip2File(LPCTSTR pName, LPCTSTR pDestDiskDir=NULL);
	BOOL Unzip2Buffer(int nIndex, void* pBuffer, int nSize);
	BOOL Unzip2Buffer(LPCTSTR pName, void* pBuffer, int nSize);

protected:
	typedef std::map<CString, ZipEntryInfo> EntryInfoVsl;
	HZIP m_hZip;
	EntryInfoVsl m_vslEntryInfo;
};

#endif