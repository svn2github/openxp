//********************************************************************
//						MD 引擎 数据文件 管理部件的定义
//		Powered by @J--S Studio			Johnhans		2004.2
//********************************************************************

#ifndef MD_FILE_H
#define MD_FILE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef	MDENGINE2P_EXPORTS
#define MDENGINE_API __declspec(dllexport)
#else
#define MDENGINE_API __declspec(dllimport)
#endif

#include "common.h"
typedef struct JSPackFileHeader		// 文件头信息
{
	DWORD	JSPK;		// 'J' 'S' 'P' 'K'   0x4B50534A
	BYTE	Version;	// 0x01
	BYTE	ExInfo;		// ExInfo&0x0f 外部压缩方案量*
						// ExInfo&0x80 文件是否加密
	WORD	DirNum;		// 本包中目录数量*
	DWORD	FileNum;	// 本包中文件数量*
	DWORD	FOffset;	// 文件信息偏移（距文件末尾位置）*
}JSPKHeader;

typedef struct JSExtendModule		// 外部扩展组件信息
{
	BYTE  NameSize;		// 组件名字大小*
	char  Name[255];	// 组件名字*
	BYTE  FNameSize;	// 组件文件名大小*
	char  FName[255];	// 组件文件名*
	DWORD Version;		// 组件版本*
}JSEXModule;

typedef struct JSDirectoryInfo		// 目录结构信息
{
	BYTE NameSize;	// 目录名域大小*
	char Name[255];	// 目录名*
	WORD MIN_Child;	// 子目录最小ID*
	WORD MAX_Child; // 子目录最大ID*
}JSDirInfo;

struct JSFileInfo		// 文件结构信息
{
	BYTE  NameSize;	// 名字域大小*
	char  Name[255];// 名称*
	DWORD Offset;	// 数据偏移（距文件起始位置）*
	DWORD SizeOri;	// 压缩前数据大小*
	DWORD SizeCom;	// 压缩后数据大小*
	WORD Root;		// 上级（所属） 0xFFFF:根(不属于任何级)*
	BYTE  Attribute;// 文件属性 0:不压缩 1:外部扩展压缩 2:内部LZW压缩*
};

struct MaskCode		// 掩码表
{
	BYTE	b[8];
	WORD	w[8];
	DWORD	d[8];
	BYTE	s[16];
	BYTE	c[16];
};

typedef DWORD ( *getProviderTypeDLL)(void);
typedef bool ( *setFileHandleDLL)(HANDLE,char *,char *);
typedef DWORD ( *getFileSizeDLL)(void);
typedef bool ( *LocateFileDLL)(char *,char *);
typedef bool ( *ReadDLL)(LPVOID,DWORD len);

typedef char* ( *getProviderNameDLL)(void);
typedef DWORD ( *getProviderVersionDLL)(void);
typedef char* ( *getProviderInfoDLL)(void);
typedef DWORD ( *getMemNeededDLL)(DWORD);
typedef bool ( *EncodeDLL)(LPBYTE,DWORD,LPBYTE,DWORD&);
typedef bool ( *DecodeDLL)(LPBYTE,DWORD,LPBYTE);

class MDENGINE_API CMD_File  
{
public:
	CMD_File();
	CMD_File(char *ProviderName);
	virtual ~CMD_File();

public:
	void	EnableErrMSG(bool bshowerr);	// 允许or禁止弹出错误信息
	DWORD	GetErrCode(void);				// 得到错误代码
	void	EnableExt(bool bEnable);		// 打开or禁用扩展搜索
	void	SetExt(char *path);				// 设置扩展搜索路径

	bool	ConnectProvider(char *ProviderName);	// 联接到外部FileProvider
	void	FreeProvider();							// 释放FileProvider

	bool	Open(char* FileName,bool bReadOnly=true,bool bPacked=false,char *password=NULL);
	void	Close();
	bool	Locate(char* PathName,char *password=NULL);	// 定位一个文件
	DWORD	GetLength();				// 取得当前文件的大小
	bool	Read(LPBYTE *buffer=NULL,DWORD size=0);	// 读取当前文件数据到引擎缓冲
	bool	Write(LPCVOID buffer,DWORD size);	// 写入当前文件
	bool	Seek(long move, BYTE method=FILE_CURRENT);	// 移动当前文件指针

	LPBYTE	GetBuffer(){ return m_lpBuffer; }	// 取得文件数据缓冲
	DWORD	GetBufferLength(){ return m_BufferLen; }
	bool	WriteBufferToFile(char *PathName);	// 将缓冲区数据写入文件

private:
	HANDLE	m_hFile;				// 文件句柄
	HANDLE	m_hExtra;				// 打开的扩展文件句柄
	bool	m_bEnableExtra;			// 是否允许扩展查询
	char	m_ExtraPath[MAX_PATH];	// 扩展目录路径
	char	m_FileName[MAX_PATH];	// 文件名
	char	m_Password[MAX_PWD];	// 文件访问密码
	LPBYTE	m_lpBuffer;				// 数据缓冲
	DWORD	m_BufferLen;			// 数据缓冲大小
	bool	m_bShowErrMSG;			// 是否显示错误
	DWORD	m_ErrCode;				// 当前错误代码
	bool	m_bPacked;				// 文件为包
	DWORD	m_LocatedFileID;		// 被定位的文件ID

//		JSPackFile结构
	JSPKHeader		m_pkHeader;		// 压缩文件头信息
	JSEXModule		*m_lppkEXModule;// 外部模块信息
	JSDirInfo		*m_lppkDirInfo;	// 目录结构信息
	JSFileInfo		*m_lppkFileInfo;// 文件信息

	MaskCode		m_Hash;

//		FileProvider部分,函数指针
	HINSTANCE	m_hInstProvider;
	getProviderTypeDLL	fp_getProviderType;
	getFileSizeDLL		fp_getFileSize;
	setFileHandleDLL	fp_setFileHandle;
	LocateFileDLL		fp_LocateFile;
	ReadDLL				fp_ReadFromPacker;

private:
	// ********* 对普通文件的操作 Gen_XXXX() ******** //
	bool	Gen_Open(char* filename, bool readonly,HANDLE *file=NULL); // 打开文件
	void	Gen_Close(void); // 关闭
	DWORD	Gen_GetFileSize(HANDLE *file=NULL); // 取得文件大小
	bool	Gen_Seek(long move, BYTE method); // 移动文件指针
	bool	Gen_Read(LPBYTE *buffer, DWORD size,HANDLE *file=NULL); // 读取文件
	bool	Gen_Write(LPCVOID buffer, DWORD size,HANDLE *file=NULL); // 写入文件

	// ********* 对压缩包文件的操作 Pak_XXXX() ******** //
	bool	Pak_Open(char* filename,char *password); // 打开文件
	void	Pak_Close(void); // 关闭文件
	bool	Pak_LocateFile(char *pathname,char *password); // 查找定位文件
	DWORD	Pak_GetFileSize(void); // 取得当前文件大小
	bool	Pak_Read(LPBYTE *buffer);


private:
	void	ShowErrMSG();	// 显示错误信息

	bool	InitPackInfo();	// 初始化压缩包目录文件信息
	bool	Decode(LPVOID in, unsigned in_len, LPVOID out,BYTE Attribute);	// 解码数据流
	bool	AnalyzePath(char **start,char *res);	// 分析文件路径
	void	SetPassword(BYTE pwd[],BYTE crc[]);		// 设置访问密码

	BYTE	Encrypt(BYTE s,BYTE bit);
	WORD	Encrypt(WORD s);
	DWORD	Encrypt(DWORD s);
	LPBYTE	GetCRCByte(bool r=false);
	void	Encrypt(char *s,BYTE len);
	void	Encrypt(BYTE *s,DWORD len);
	void	EncryptHead(JSPackFileHeader &head);
	void	EncryptReset();
};

#endif