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

	//压缩到缓冲    
	BYTE *pDestBuffer = NULL;
	pDestBuffer = new BYTE[lSrcLenght];
	memset(pDestBuffer,0,lSrcLenght);

	//压缩了之后的数据长度(压缩前初始为源数据长度)
	ULONG lComLen = lSrcLenght;

	/*
	pCompBuffer  压缩之后的缓冲
	lComLen      压缩之后的数据长度
	pSrcBuffer	 源数据缓冲
	lLenght      源数据长度
	*/
	compress(pDestBuffer,&lComLen, pSrcBuffer,lSrcLenght);   

	if (lComLen <= 0) return FALSE;

	//放置到传入数据缓冲
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
		// a 为指针的指针，*a为指针，**a为指针指向的地方，一个int值
	} 
*/

BOOL HZlibComp::CompressDataFromFile( LPCSTR lpszFile,BYTE **pCompBuffer,ULONG &lCompLenght )
{
	CFile file(lpszFile,CFile::modeRead|CFile::shareExclusive);
	ULONG lLenght = file.GetLength();
	if (lLenght <= 0) return FALSE;

	//读取源数据
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

	//读取源数据
	BYTE *pSrcBuffer = NULL;
	pSrcBuffer = new BYTE[lSrcLenght];
	memset(pSrcBuffer,0,lSrcLenght);
	file.Read(pSrcBuffer,lSrcLenght);

	UnCompressData(pSrcBuffer,lSrcLenght,pUnCompBuffer,lUnCompLenght);
	
	delete pSrcBuffer;
	file.Close();
	return TRUE;
}