#ifndef _HMDFILE_H_ 
#define _HMDFILE_H_
#pragma once
#include <atlimage.h>
#include <vector>
#ifndef FILE_CONTROL_CLASS
	#include "MDFile/MD_File.h"
#endif

//zip、rar文件不支持加密的
#define EPK_PLUG 0x0000
#define ZIP_PLUG 0x0001
#define RAR_PLUG 0x0002

class FILE_CONTROL_API HMDFile
{
	//外部接口
public:
	HMDFile();
	~HMDFile();

public:

	//pDiskFilePath -- 压缩文件在磁盘上的绝对路径
	//nPlugType     -- 压缩文件的文件类型(epk、zip、rar)
	//pPassWord     -- 压缩文件的密码
	//返回值        -- 打开是否失败
	BOOL Open(const char *pDiskFilePath,UINT nPlugType=EPK_PLUG,char *pPassWord=NULL);//打开压缩文件(epk、zip、rar)

	void Close();//关闭文件流
	
	//pEpkFileName   -- 要提取的文件在压缩文件里面的名字
	//dwBufferLength -- 当前提取文件的缓冲区大小
	//返回值         -- 返回提取文件的数据缓冲 
	LPBYTE GetBufferFromEpk(const char *pEpkFileName,DWORD *dwBufferLength);//从压缩文件得到缓冲区

	//pEpkFileName    -- 要提取的文件在压缩文件里面的名字
	//pImg            -- CImage对象指针
	//bIsAlphaChannel -- 是否该提取图像文件有Alpha通道
	BOOL GetImageFromEpk(const char *pEpkFileName,CImage *pImg,BOOL bIsAlphaChannel=FALSE);//从压缩文件直接得到图像文件

	//缓冲处理
protected:
	BOOL CreateTempFile(void *lpBuf,UINT nSize);

	//成员数据
private:
	CMD_File					m_epkFile;
	std::vector<CString>		m_vecTemp;
	UINT						m_nPlugType;
};

#endif