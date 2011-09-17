#ifndef __HZLIBCOMP__H__
#define __HZLIBCOMP__H__
#pragma once
#ifndef FILE_CONTROL_CLASS
	#include "zlib/zlib.h"
	#include "zlib/zconf.h"
#endif

//������ʱ�ò���

class FILE_CONTROL_API HZlibComp
{
public:
	HZlibComp();
	~HZlibComp();
public:
	/*
	pSrcBuffer		ѹ������
	lSrcLenght		ѹ�����ݳ���
	pCompBuffer		ѹ�����ݻ���
	lCompLenght		ѹ��֮������ݳ���
	ע�⣺������ɾ��pCompBuffer������ݶ�ջ�ռ�
	*/
	BOOL CompressData(BYTE *pSrcBuffer,ULONG lSrcLenght,BYTE **pCompBuffer,ULONG &lCompLenght);
	/*
	pSrcBuffer		��ѹ������
	lSrcLenght		ѹ��ǰ���ݳ���
	pUnCompBuffer	��ѹ�����ݻ���
	lUnCompLenght	��ѹ��֮������ݳ���
	ע�⣺������ɾ��pUnCompBuffer������ݶ�ջ�ռ�
	*/
	BOOL UnCompressData(BYTE *pSrcBuffer,ULONG lSrcLenght,BYTE **pUnCompBuffer,ULONG &lUnCompLenght);




	/*
		lpszFile		�ļ�·��
		pCompBuffer		ѹ�����ݻ���
		lCompLenght		ѹ��֮������ݳ���
		ע�⣺������ɾ��pCompBuffer������ݶ�ջ�ռ�
	*/
	BOOL CompressDataFromFile(LPCSTR lpszFile,BYTE **pCompBuffer,ULONG &lCompLenght);
	/*
		lpszFile			�ļ�·��
		lSrcLenght			ѹ��ǰ���ݳ���
		pUnCompBuffer		��ѹ�����ݻ���
		lUnCompLenght		��ѹ��֮������ݳ���
		ע�⣺������ɾ��pUnCompBuffer������ݶ�ջ�ռ�
	*/
	BOOL UnCompressDataFromFile(LPCSTR lpszFile,ULONG lSrcLenght,BYTE **pUnCompBuffer,ULONG &lUnCompLenght);
};

#endif