#ifndef ENCRYPT_HEAD_FILE
#define ENCRYPT_HEAD_FILE
#pragma once
#pragma warning(disable:4996)
//////////////////////////////////////////////////////////////////////////

//MD5摘要值结构体
typedef struct MD5VAL_STRUCT
{
	unsigned int a;
	unsigned int b;
	unsigned int c;
	unsigned int d;
} MD5VAL;

//MD5 加密类
class TEMPLATE_CONTROL_API HMD5Encrypt
{
	//函数定义
private:
	//构造函数
	HMD5Encrypt() {}

	//功能函数
public:
	//生成密文
	static void EncryptData(LPCTSTR pszSrcData, TCHAR szMD5Result[33]);
	//生成密文件
	static CString ValueFileMD5(CString strFilePath);
protected:
	//MD5文件摘要
	static MD5VAL md5File(FILE * fpin);
};

//////////////////////////////////////////////////////////////////////////

//异或加密类
class TEMPLATE_CONTROL_API HXOREncrypt
{
	//函数定义
private:
	//构造函数
	HXOREncrypt() {}

	//功能函数
public:
	//生成密文
	static WORD EncryptData(LPCTSTR pszSrcData, LPTSTR pszEncrypData, WORD wSize);
	//解开密文
	static WORD CrevasseData(LPCTSTR pszEncrypData, LPTSTR pszSrcData, WORD wSize);
};

//////////////////////////////////////////////////////////////////////////

#endif