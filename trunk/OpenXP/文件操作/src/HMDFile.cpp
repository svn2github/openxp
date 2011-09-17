#include "stdafx.h"
#include "../include/HMDFile.h"
#pragma comment(lib,"MDFile/MD_File.lib")


//����png��Alphaͨ��
void DealPngAlphaChannel( CImage *pImage )
{
	ASSERT(pImage!=NULL);
	ASSERT(!pImage->IsNull());
	for(int i = 0; i < pImage->GetWidth(); ++i)
	{
		for(int j = 0; j < pImage->GetHeight(); ++j)
		{
			unsigned char* pucColor = (unsigned char*)pImage->GetPixelAddress(i,j);
			pucColor[0] = pucColor[0]*pucColor[3]/255;
			pucColor[1] = pucColor[1]*pucColor[3]/255;
			pucColor[2] = pucColor[2]*pucColor[3]/255;
		}
	}
}

//Ĭ�Ϲ��캯��
HMDFile::HMDFile()
{
	m_nPlugType = EPK_PLUG;
}

//��������
HMDFile::~HMDFile()
{
	Close();
}

//����������ʱ�ļ�
BOOL HMDFile::CreateTempFile( void *lpBuf,UINT nSize )
{
	TCHAR szTempPath[_MAX_PATH],szTempfile[_MAX_PATH]; 
	DWORD dwRet = GetTempPath(_MAX_PATH, szTempPath);
	if (dwRet == 0)
	{
		return FALSE;
	}
	UINT nRet = GetTempFileName(szTempPath,_T("my_"),0,szTempfile); 
	if (nRet == 0)
	{
		return FALSE;
	}
	CFile m_tempFile(szTempfile,CFile:: modeCreate|CFile:: modeWrite); 
	m_tempFile.Write(lpBuf,nSize); 
	m_tempFile.Close(); 
	CString strBuf(szTempfile);
	m_vecTemp.push_back(strBuf);
	return TRUE;
}

//��ѹ���ļ�
BOOL HMDFile::Open( const char *pDiskFilePath,UINT nPlugType,char *pPassWord )
{
	m_epkFile.EnableErrMSG(true);//����������ʾ
	if (nPlugType == ZIP_PLUG)
		m_epkFile.ConnectProvider("ZipExt.dll");
	else if (nPlugType == RAR_PLUG)
		m_epkFile.ConnectProvider("RarExt.dll");
	m_nPlugType = nPlugType;
	return m_epkFile.Open(const_cast<char*>(pDiskFilePath),true,true,pPassWord);
}

//�õ�������
LPBYTE HMDFile::GetBufferFromEpk( const char *pEpkFileName,DWORD *dwBufferLength )
{
	bool bSuccess = m_epkFile.Locate(const_cast<char*>(pEpkFileName));
	if (!bSuccess)
	{
		return NULL;
	}
	bSuccess = m_epkFile.Read();
	if (!bSuccess)
	{
		return NULL;
	}
	*dwBufferLength = m_epkFile.GetLength();
	if (*dwBufferLength < 0)
	{
		return NULL;
	}
	return m_epkFile.GetBuffer();
}

//�õ�ͼ���ļ�
BOOL HMDFile::GetImageFromEpk( const char *pEpkFileName,CImage *pImg,BOOL bIsAlphaChannel/*=FALSE*/ )
{
	bool bSuccess = m_epkFile.Locate(const_cast<char*>(pEpkFileName));
	if (!bSuccess)
	{
		return FALSE;
	}
	bSuccess = m_epkFile.Read();
	if (!bSuccess)
	{
		return FALSE;
	}
	DWORD dwLength = m_epkFile.GetLength();
	LPBYTE pData = m_epkFile.GetBuffer();
	if (dwLength < 0 || pData == NULL)
	{
		return FALSE;
	}

	HGLOBAL hMem = GlobalAlloc(GMEM_FIXED, dwLength);
	BYTE* pMem = (BYTE*)GlobalLock(hMem);
	memcpy(pMem,pData,dwLength);
	IStream* pSteam;
	CreateStreamOnHGlobal(hMem,FALSE,&pSteam);

	ASSERT(pImg!=NULL);
	if (!pImg->IsNull()) pImg->Destroy();
	if (pImg->Load(pSteam))//�������ʱ��������true����Ϊ����ֵ��HRESULT
	{
		GlobalUnlock(hMem);
		pSteam->Release();
		return FALSE;
	}

	//���Ҫ��ȡ��Alphaͨ��
	if (bIsAlphaChannel)
		DealPngAlphaChannel(pImg);

	GlobalUnlock(hMem);
	pSteam->Release();
	return TRUE;
}

//�ر��ļ���
void HMDFile::Close()
{
	for (size_t i = 0; i < m_vecTemp.size(); ++i)
	{
		DeleteFile(m_vecTemp[i]);
	}
	if (m_nPlugType != 0x0000)
		m_epkFile.FreeProvider();
	m_epkFile.Close();
}