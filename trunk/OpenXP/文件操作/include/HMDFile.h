#ifndef _HMDFILE_H_ 
#define _HMDFILE_H_
#pragma once
#include <atlimage.h>
#include <vector>
#ifndef FILE_CONTROL_CLASS
	#include "MDFile/MD_File.h"
#endif

//zip��rar�ļ���֧�ּ��ܵ�
#define EPK_PLUG 0x0000
#define ZIP_PLUG 0x0001
#define RAR_PLUG 0x0002

class FILE_CONTROL_API HMDFile
{
	//�ⲿ�ӿ�
public:
	HMDFile();
	~HMDFile();

public:

	//pDiskFilePath -- ѹ���ļ��ڴ����ϵľ���·��
	//nPlugType     -- ѹ���ļ����ļ�����(epk��zip��rar)
	//pPassWord     -- ѹ���ļ�������
	//����ֵ        -- ���Ƿ�ʧ��
	BOOL Open(const char *pDiskFilePath,UINT nPlugType=EPK_PLUG,char *pPassWord=NULL);//��ѹ���ļ�(epk��zip��rar)

	void Close();//�ر��ļ���
	
	//pEpkFileName   -- Ҫ��ȡ���ļ���ѹ���ļ����������
	//dwBufferLength -- ��ǰ��ȡ�ļ��Ļ�������С
	//����ֵ         -- ������ȡ�ļ������ݻ��� 
	LPBYTE GetBufferFromEpk(const char *pEpkFileName,DWORD *dwBufferLength);//��ѹ���ļ��õ�������

	//pEpkFileName    -- Ҫ��ȡ���ļ���ѹ���ļ����������
	//pImg            -- CImage����ָ��
	//bIsAlphaChannel -- �Ƿ����ȡͼ���ļ���Alphaͨ��
	BOOL GetImageFromEpk(const char *pEpkFileName,CImage *pImg,BOOL bIsAlphaChannel=FALSE);//��ѹ���ļ�ֱ�ӵõ�ͼ���ļ�

	//���崦��
protected:
	BOOL CreateTempFile(void *lpBuf,UINT nSize);

	//��Ա����
private:
	CMD_File					m_epkFile;
	std::vector<CString>		m_vecTemp;
	UINT						m_nPlugType;
};

#endif