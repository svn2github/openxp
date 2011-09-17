#include "stdafx.h"
#include "../include/HZipFile.h"

//////////////////////////////////////////////////////////////////////////
//Zipѹ���ļ�

HZipFile::HZipFile( void )
{
	m_hZip = NULL;
}

HZipFile::~HZipFile( void )
{
	Close();//ǿ���ͷ���Դһ�� 
}

//����Zip�ļ�
BOOL HZipFile::ZipCreate( LPCTSTR pZipFileName, LPCTSTR pZipPassword )
{
	m_hZip = CreateZip(pZipFileName,pZipPassword);
	if (!m_hZip)
	{
		return FALSE;
	}
	return TRUE;
}

//�����ļ���Zip
BOOL HZipFile::ZipFileAdd( LPCTSTR pDestName, LPCTSTR pSrcDiskPath )
{
	ZRESULT zRet = ZipAdd(m_hZip,pDestName, pSrcDiskPath);
	if (zRet == ZR_FAILED)
	{
		return FALSE;
	}
	return TRUE;
}

//�ر�Zip���ͷ���Դ
void HZipFile::Close()
{
	if (!m_hZip)
	{
		CloseZip(m_hZip);//�ͷ���Դһ��
	} 
}

//////////////////////////////////////////////////////////////////////////
//UnZip��ѹ���ļ�

HUnzipFile::HUnzipFile( void )
{
	m_hZip = NULL;
}

HUnzipFile::~HUnzipFile( void )
{
	if (!m_hZip)
	{
		Close();
	}
}

//��Zip�ļ�
BOOL HUnzipFile::Open( LPCTSTR pFileName, LPCTSTR pPassword )
{
	m_hZip = OpenZip(pFileName,pPassword);
	if (!m_hZip)
	{
		return FALSE;
	}
	//���浱ǰZip�ڲ��ļ���Ϣ������
	ZIPENTRY ze; 
	GetZipItem(m_hZip,-1,&ze); 
	int nCount = ze.index;
	for (int i = 0; i < nCount; i++)
	{ 
		GetZipItem(m_hZip,i,&ze);
		ZipEntryInfo info;
		info.m_nIndex = ze.index;
		info.m_nSize = ze.unc_size;
		info.m_sName = ze.name;
		m_vslEntryInfo[info.m_sName] = info;
	}

	return TRUE;
}

//��ѹȫ����Դ��������
BOOL HUnzipFile::UnZipAll( LPCTSTR pDiskDir )
{
	ZRESULT zRet;
	zRet = SetUnzipBaseDir(m_hZip,pDiskDir);
	if (zRet == ZR_FAILED)
	{
		return FALSE;
	}
	ZIPENTRY ze; 
	zRet = GetZipItem(m_hZip,-1,&ze); 
	if (zRet == ZR_FAILED)
	{
		return FALSE;
	}
	int nCount = ze.index;
	for (int i = 0; i < nCount; i++)
	{ 
		zRet = GetZipItem(m_hZip,i,&ze);
		if (zRet == ZR_FAILED)
		{
			return FALSE;
		}
		zRet = UnzipItem(m_hZip,i,ze.name);
		if (zRet == ZR_FAILED)
		{
			return FALSE;
		}
	}
	return TRUE;
}

//��ѹ�����ļ������̴�����
BOOL HUnzipFile::Unzip2File( int nIndex, LPCTSTR pDestDiskDir )
{
	if (nIndex < 1)
	{
		return FALSE;
	}
	ZRESULT zRet;
	ZIPENTRY ze; 
	zRet = GetZipItem(m_hZip,nIndex,&ze);
	if (zRet == ZR_FAILED)
	{
		return FALSE;
	}

	if (pDestDiskDir != NULL)
	{
		ZRESULT zRet;
		zRet = SetUnzipBaseDir(m_hZip,pDestDiskDir);
		if (zRet == ZR_FAILED)
		{
			return FALSE;
		}
	}

	zRet = UnzipItem(m_hZip,nIndex,ze.name);
	if (zRet == ZR_FAILED)
	{
		return FALSE;
	}
	return TRUE;
}

//��ѹ�����ļ������̴��ļ���
BOOL HUnzipFile::Unzip2File( LPCTSTR pName, LPCTSTR pDestDiskDir )
{
	int iIndex = 1;
	std::map<CString, ZipEntryInfo>::iterator pIt;
	for (pIt = m_vslEntryInfo.begin(); pIt != m_vslEntryInfo.end(); ++pIt)
	{
		if (strcmp(pIt->first,pName))
		{
			return Unzip2File(iIndex,pDestDiskDir);
		}
		iIndex++;
	}
	return FALSE;
}

//��ѹ�ļ���������������
BOOL HUnzipFile::Unzip2Buffer( int nIndex, void* pBuffer, int nSize )
{
	ZRESULT zRet;
	ZIPENTRY ze; 
	zRet = GetZipItem(m_hZip,-1,&ze); 
	if (zRet == ZR_FAILED)
	{
		return FALSE;
	}
	if (nIndex > ze.index)
	{
		return FALSE;
	}

	ZipEntryInfo info;
	GetEntryInfo(nIndex,info);
	zRet = UnzipItem(m_hZip,info.m_nIndex,pBuffer,nSize);
	if (zRet == ZR_FAILED)
	{
		return FALSE;
	}
	return TRUE;
}

//��ѹ�ļ������������ļ���
BOOL HUnzipFile::Unzip2Buffer( LPCTSTR pName, void* pBuffer, int nSize )
{
	int iIndex = 1;
	std::map<CString, ZipEntryInfo>::iterator pIt;
	for (pIt = m_vslEntryInfo.begin(); pIt != m_vslEntryInfo.end(); ++pIt)
	{
		if (strcmp(pIt->first,pName))
		{
			return Unzip2Buffer(iIndex,pBuffer,nSize);
		}
		iIndex++;
	}
	return FALSE;
}

//�õ�Zip�ļ����ļ�������
int HUnzipFile::GetEntryCount() const
{
	if (m_vslEntryInfo.empty())
	{
		return -1;
	}
	return (int)m_vslEntryInfo.size();
}

//�õ�Zip�ļ���Ϣ
BOOL HUnzipFile::GetEntryInfo( int nIndex, ZipEntryInfo& info )
{
	if (nIndex < 1)
	{
		return NULL;
	}

	if (m_vslEntryInfo.empty())
	{
		return FALSE;
	}
	if (nIndex > (int)m_vslEntryInfo.size())
	{
		return FALSE;
	}

	nIndex--;
	int iIndex = 0;
	std::map<CString, ZipEntryInfo>::iterator pIt;
	for (pIt = m_vslEntryInfo.begin(); pIt != m_vslEntryInfo.end(); ++pIt)
	{
		if (iIndex == nIndex)
		{
			info.m_nIndex = pIt->second.m_nIndex;
			info.m_nSize = pIt->second.m_nSize;
			info.m_sName = pIt->second.m_sName;
			goto __SUCCESS__;
		}
		iIndex++;
	}
__SUCCESS__:
	return TRUE;
}

//��Zip�ļ����õ���Ϣ
BOOL HUnzipFile::GetEntryInfo( LPCTSTR pName, ZipEntryInfo& info )
{
	std::map<CString, ZipEntryInfo>::iterator pIt;
	for (pIt = m_vslEntryInfo.begin(); pIt != m_vslEntryInfo.end(); ++pIt)
	{
		if (strcmp(pIt->first,pName) == 0)
		{
			info = m_vslEntryInfo[pName];
			return TRUE;
		}
	}
	return FALSE;
}

//�õ�Zip�ļ��������ļ�������
LPCTSTR HUnzipFile::GetItemName( int nIndex )
{
	if (nIndex < 1)
	{
		return NULL;
	}
	if (m_vslEntryInfo.empty())
	{
		return FALSE;
	}
	if (nIndex > (int)m_vslEntryInfo.size())
	{
		return FALSE;
	}

	int iIndex = 0;
	nIndex--;
	std::map<CString, ZipEntryInfo>::iterator pIt;
	for (pIt = m_vslEntryInfo.begin(); pIt != m_vslEntryInfo.end(); ++pIt)
	{
		if (iIndex == nIndex)
		{
			return pIt->first;
		}
		iIndex++;
	}
	return NULL;
}

//�ر�Zip�ļ����ͷ���Դ
void HUnzipFile::Close()
{
	CloseZip(m_hZip);
}