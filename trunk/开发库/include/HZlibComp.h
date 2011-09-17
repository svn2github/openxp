#ifndef __HZLIBCOMP__H__
#define __HZLIBCOMP__H__
#pragma once
#ifndef FILE_CONTROL_CLASS
	#include "zlib/zlib.h"
	#include "zlib/zconf.h"
#endif

//该类暂时用不了

class FILE_CONTROL_API HZlibComp
{
public:
	HZlibComp();
	~HZlibComp();
public:
	/*
	pSrcBuffer		压缩数据
	lSrcLenght		压缩数据长度
	pCompBuffer		压缩数据缓冲
	lCompLenght		压缩之后的数据长度
	注意：调用者删除pCompBuffer这个数据堆栈空间
	*/
	BOOL CompressData(BYTE *pSrcBuffer,ULONG lSrcLenght,BYTE **pCompBuffer,ULONG &lCompLenght);
	/*
	pSrcBuffer		解压缩数据
	lSrcLenght		压缩前数据长度
	pUnCompBuffer	解压缩数据缓冲
	lUnCompLenght	解压缩之后的数据长度
	注意：调用者删除pUnCompBuffer这个数据堆栈空间
	*/
	BOOL UnCompressData(BYTE *pSrcBuffer,ULONG lSrcLenght,BYTE **pUnCompBuffer,ULONG &lUnCompLenght);




	/*
		lpszFile		文件路径
		pCompBuffer		压缩数据缓冲
		lCompLenght		压缩之后的数据长度
		注意：调用者删除pCompBuffer这个数据堆栈空间
	*/
	BOOL CompressDataFromFile(LPCSTR lpszFile,BYTE **pCompBuffer,ULONG &lCompLenght);
	/*
		lpszFile			文件路径
		lSrcLenght			压缩前数据长度
		pUnCompBuffer		解压缩数据缓冲
		lUnCompLenght		解压缩之后的数据长度
		注意：调用者删除pUnCompBuffer这个数据堆栈空间
	*/
	BOOL UnCompressDataFromFile(LPCSTR lpszFile,ULONG lSrcLenght,BYTE **pUnCompBuffer,ULONG &lUnCompLenght);
};

#endif