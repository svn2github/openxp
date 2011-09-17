#ifndef __ZIPFILE__H__
#define __ZIPFILE__H__
#pragma once
#include <map>
#ifndef FILE_CONTROL_CLASS
	#include "zip/zip.h"
	#include "zip/unzip.h"
#endif

//该文件不能处理有密码的压缩文件
//HZipFile暂时不能用，UnZipFile可以解压winRar压缩起来的Zip文件
struct ZipEntryInfo
{
	int			m_nIndex;
	long		m_nSize;
	CString	    m_sName;
};

//ZIP文件操作类
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

// UNZIP文件操作类
class FILE_CONTROL_API HUnzipFile
{
public:  //所有的nIndex都是ZIP文件在视觉上的索引，从1开始
	HUnzipFile(void);
	virtual ~HUnzipFile(void);

	BOOL Open(LPCTSTR pZipFileName, LPCTSTR pPassword=NULL);
	void Close();//关闭Zip文件，释放资源

	int GetEntryCount() const;//得到Zip文件中文件的数量
	LPCTSTR GetItemName(int nIndex);//得到Zip文件中文件的名字 

	BOOL GetEntryInfo(int nIndex, ZipEntryInfo& info);
	BOOL GetEntryInfo(LPCTSTR pName, ZipEntryInfo& info);
	BOOL UnZipAll(LPCTSTR pDiskDir);//解压所有到磁盘路径
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