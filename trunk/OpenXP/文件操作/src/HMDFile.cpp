#include "stdafx.h"
#include "../include/HMDFile.h"
#pragma comment(lib,"MDFile/MD_File.lib")


//运算png的Alpha通道
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

//默认构造函数
HMDFile::HMDFile()
{
	m_nPlugType = EPK_PLUG;
}

//析构函数
HMDFile::~HMDFile()
{
	Close();
}

//创建缓冲临时文件
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

//打开压缩文件
BOOL HMDFile::Open( const char *pDiskFilePath,UINT nPlugType,char *pPassWord )
{
	m_epkFile.EnableErrMSG(true);//开启错误提示
	if (nPlugType == ZIP_PLUG)
		m_epkFile.ConnectProvider("ZipExt.dll");
	else if (nPlugType == RAR_PLUG)
		m_epkFile.ConnectProvider("RarExt.dll");
	m_nPlugType = nPlugType;
	return m_epkFile.Open(const_cast<char*>(pDiskFilePath),true,true,pPassWord);
}

//得到缓冲区
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

//得到图像文件
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
	if (pImg->Load(pSteam))//导入出错时，返回是true，因为返回值是HRESULT
	{
		GlobalUnlock(hMem);
		pSteam->Release();
		return FALSE;
	}

	//如果要求取出Alpha通道
	if (bIsAlphaChannel)
		DealPngAlphaChannel(pImg);

	GlobalUnlock(hMem);
	pSteam->Release();
	return TRUE;
}

//关闭文件流
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