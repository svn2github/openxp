#include "StdAfx.h"
#include "../include/HZlibComp.h"

HZlibComp::HZlibComp()
{
}

HZlibComp::~HZlibComp()
{
}

BOOL HZlibComp::CompressData( BYTE *pSrcBuffer,ULONG lSrcLenght,BYTE **pCompBuffer,ULONG &lCompLenght )
{
	ASSERT(NULL != pSrcBuffer);
	if (NULL == pSrcBuffer) return FALSE;

	//ѹ��������    
	BYTE *pDestBuffer = NULL;
	pDestBuffer = new BYTE[lSrcLenght];
	memset(pDestBuffer,0,lSrcLenght);

	//ѹ����֮������ݳ���(ѹ��ǰ��ʼΪԴ���ݳ���)
	ULONG lComLen = lSrcLenght;

	/*
	pCompBuffer  ѹ��֮��Ļ���
	lComLen      ѹ��֮������ݳ���
	pSrcBuffer	 Դ���ݻ���
	lLenght      Դ���ݳ���
	*/
	compress(pDestBuffer,&lComLen, pSrcBuffer,lSrcLenght);   

	if (lComLen <= 0) return FALSE;

	//���õ��������ݻ���
	*pCompBuffer = new BYTE[lComLen];
	memcpy(*pCompBuffer,pDestBuffer,sizeof(BYTE)*lComLen);
	lCompLenght = lComLen;

	delete pDestBuffer;
	return TRUE;
}

BOOL HZlibComp::UnCompressData( BYTE *pSrcBuffer,ULONG lSrcLenght,BYTE **pUnCompBuffer,ULONG &lUnCompLenght )
{
	ASSERT(NULL != pSrcBuffer);
	if (NULL == pSrcBuffer) return FALSE;

	ULONG lUnCompLen = lSrcLenght;
	*pUnCompBuffer = new BYTE[lSrcLenght];
	memset(*pUnCompBuffer,0,sizeof(BYTE)*lUnCompLen);
	uncompress(*pUnCompBuffer,&lUnCompLen,pSrcBuffer,lSrcLenght);
	lUnCompLenght = lUnCompLen;
	return TRUE;
}


/*
	void func(int **a)
	{
		// a Ϊָ���ָ�룬*aΪָ�룬**aΪָ��ָ��ĵط���һ��intֵ
	} 
*/

BOOL HZlibComp::CompressDataFromFile( LPCSTR lpszFile,BYTE **pCompBuffer,ULONG &lCompLenght )
{
	CFile file(lpszFile,CFile::modeRead|CFile::shareExclusive);
	ULONG lLenght = file.GetLength();
	if (lLenght <= 0) return FALSE;

	//��ȡԴ����
	BYTE *pSrcBuffer = NULL;
	pSrcBuffer = new BYTE[lLenght];
	memset(pSrcBuffer,0,lLenght);
	file.Read(pSrcBuffer,lLenght);

	CompressData(pSrcBuffer,lLenght,pCompBuffer,lCompLenght);

	delete pSrcBuffer;
	file.Close();
	return TRUE;
}

BOOL HZlibComp::UnCompressDataFromFile( LPCSTR lpszFile,ULONG lSrcLenght,BYTE **pUnCompBuffer,ULONG &lUnCompLenght )
{
	CFile file(lpszFile,CFile::modeRead|CFile::shareExclusive);
	if (lSrcLenght <= 0) return FALSE;

	//��ȡԴ����
	BYTE *pSrcBuffer = NULL;
	pSrcBuffer = new BYTE[lSrcLenght];
	memset(pSrcBuffer,0,lSrcLenght);
	file.Read(pSrcBuffer,lSrcLenght);

	UnCompressData(pSrcBuffer,lSrcLenght,pUnCompBuffer,lUnCompLenght);
	
	delete pSrcBuffer;
	file.Close();
	return TRUE;
}