//********************************************************************
//						MD ���� �����ļ� �������Ķ���
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
typedef struct JSPackFileHeader		// �ļ�ͷ��Ϣ
{
	DWORD	JSPK;		// 'J' 'S' 'P' 'K'   0x4B50534A
	BYTE	Version;	// 0x01
	BYTE	ExInfo;		// ExInfo&0x0f �ⲿѹ��������*
						// ExInfo&0x80 �ļ��Ƿ����
	WORD	DirNum;		// ������Ŀ¼����*
	DWORD	FileNum;	// �������ļ�����*
	DWORD	FOffset;	// �ļ���Ϣƫ�ƣ����ļ�ĩβλ�ã�*
}JSPKHeader;

typedef struct JSExtendModule		// �ⲿ��չ�����Ϣ
{
	BYTE  NameSize;		// ������ִ�С*
	char  Name[255];	// �������*
	BYTE  FNameSize;	// ����ļ�����С*
	char  FName[255];	// ����ļ���*
	DWORD Version;		// ����汾*
}JSEXModule;

typedef struct JSDirectoryInfo		// Ŀ¼�ṹ��Ϣ
{
	BYTE NameSize;	// Ŀ¼�����С*
	char Name[255];	// Ŀ¼��*
	WORD MIN_Child;	// ��Ŀ¼��СID*
	WORD MAX_Child; // ��Ŀ¼���ID*
}JSDirInfo;

struct JSFileInfo		// �ļ��ṹ��Ϣ
{
	BYTE  NameSize;	// �������С*
	char  Name[255];// ����*
	DWORD Offset;	// ����ƫ�ƣ����ļ���ʼλ�ã�*
	DWORD SizeOri;	// ѹ��ǰ���ݴ�С*
	DWORD SizeCom;	// ѹ�������ݴ�С*
	WORD Root;		// �ϼ��������� 0xFFFF:��(�������κμ�)*
	BYTE  Attribute;// �ļ����� 0:��ѹ�� 1:�ⲿ��չѹ�� 2:�ڲ�LZWѹ��*
};

struct MaskCode		// �����
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
	void	EnableErrMSG(bool bshowerr);	// ����or��ֹ����������Ϣ
	DWORD	GetErrCode(void);				// �õ��������
	void	EnableExt(bool bEnable);		// ��or������չ����
	void	SetExt(char *path);				// ������չ����·��

	bool	ConnectProvider(char *ProviderName);	// ���ӵ��ⲿFileProvider
	void	FreeProvider();							// �ͷ�FileProvider

	bool	Open(char* FileName,bool bReadOnly=true,bool bPacked=false,char *password=NULL);
	void	Close();
	bool	Locate(char* PathName,char *password=NULL);	// ��λһ���ļ�
	DWORD	GetLength();				// ȡ�õ�ǰ�ļ��Ĵ�С
	bool	Read(LPBYTE *buffer=NULL,DWORD size=0);	// ��ȡ��ǰ�ļ����ݵ����滺��
	bool	Write(LPCVOID buffer,DWORD size);	// д�뵱ǰ�ļ�
	bool	Seek(long move, BYTE method=FILE_CURRENT);	// �ƶ���ǰ�ļ�ָ��

	LPBYTE	GetBuffer(){ return m_lpBuffer; }	// ȡ���ļ����ݻ���
	DWORD	GetBufferLength(){ return m_BufferLen; }
	bool	WriteBufferToFile(char *PathName);	// ������������д���ļ�

private:
	HANDLE	m_hFile;				// �ļ����
	HANDLE	m_hExtra;				// �򿪵���չ�ļ����
	bool	m_bEnableExtra;			// �Ƿ�������չ��ѯ
	char	m_ExtraPath[MAX_PATH];	// ��չĿ¼·��
	char	m_FileName[MAX_PATH];	// �ļ���
	char	m_Password[MAX_PWD];	// �ļ���������
	LPBYTE	m_lpBuffer;				// ���ݻ���
	DWORD	m_BufferLen;			// ���ݻ����С
	bool	m_bShowErrMSG;			// �Ƿ���ʾ����
	DWORD	m_ErrCode;				// ��ǰ�������
	bool	m_bPacked;				// �ļ�Ϊ��
	DWORD	m_LocatedFileID;		// ����λ���ļ�ID

//		JSPackFile�ṹ
	JSPKHeader		m_pkHeader;		// ѹ���ļ�ͷ��Ϣ
	JSEXModule		*m_lppkEXModule;// �ⲿģ����Ϣ
	JSDirInfo		*m_lppkDirInfo;	// Ŀ¼�ṹ��Ϣ
	JSFileInfo		*m_lppkFileInfo;// �ļ���Ϣ

	MaskCode		m_Hash;

//		FileProvider����,����ָ��
	HINSTANCE	m_hInstProvider;
	getProviderTypeDLL	fp_getProviderType;
	getFileSizeDLL		fp_getFileSize;
	setFileHandleDLL	fp_setFileHandle;
	LocateFileDLL		fp_LocateFile;
	ReadDLL				fp_ReadFromPacker;

private:
	// ********* ����ͨ�ļ��Ĳ��� Gen_XXXX() ******** //
	bool	Gen_Open(char* filename, bool readonly,HANDLE *file=NULL); // ���ļ�
	void	Gen_Close(void); // �ر�
	DWORD	Gen_GetFileSize(HANDLE *file=NULL); // ȡ���ļ���С
	bool	Gen_Seek(long move, BYTE method); // �ƶ��ļ�ָ��
	bool	Gen_Read(LPBYTE *buffer, DWORD size,HANDLE *file=NULL); // ��ȡ�ļ�
	bool	Gen_Write(LPCVOID buffer, DWORD size,HANDLE *file=NULL); // д���ļ�

	// ********* ��ѹ�����ļ��Ĳ��� Pak_XXXX() ******** //
	bool	Pak_Open(char* filename,char *password); // ���ļ�
	void	Pak_Close(void); // �ر��ļ�
	bool	Pak_LocateFile(char *pathname,char *password); // ���Ҷ�λ�ļ�
	DWORD	Pak_GetFileSize(void); // ȡ�õ�ǰ�ļ���С
	bool	Pak_Read(LPBYTE *buffer);


private:
	void	ShowErrMSG();	// ��ʾ������Ϣ

	bool	InitPackInfo();	// ��ʼ��ѹ����Ŀ¼�ļ���Ϣ
	bool	Decode(LPVOID in, unsigned in_len, LPVOID out,BYTE Attribute);	// ����������
	bool	AnalyzePath(char **start,char *res);	// �����ļ�·��
	void	SetPassword(BYTE pwd[],BYTE crc[]);		// ���÷�������

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