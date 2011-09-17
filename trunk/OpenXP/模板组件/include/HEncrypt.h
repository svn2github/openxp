#ifndef ENCRYPT_HEAD_FILE
#define ENCRYPT_HEAD_FILE
#pragma once
#pragma warning(disable:4996)
//////////////////////////////////////////////////////////////////////////

//MD5ժҪֵ�ṹ��
typedef struct MD5VAL_STRUCT
{
	unsigned int a;
	unsigned int b;
	unsigned int c;
	unsigned int d;
} MD5VAL;

//MD5 ������
class TEMPLATE_CONTROL_API HMD5Encrypt
{
	//��������
private:
	//���캯��
	HMD5Encrypt() {}

	//���ܺ���
public:
	//��������
	static void EncryptData(LPCTSTR pszSrcData, TCHAR szMD5Result[33]);
	//�������ļ�
	static CString ValueFileMD5(CString strFilePath);
protected:
	//MD5�ļ�ժҪ
	static MD5VAL md5File(FILE * fpin);
};

//////////////////////////////////////////////////////////////////////////

//��������
class TEMPLATE_CONTROL_API HXOREncrypt
{
	//��������
private:
	//���캯��
	HXOREncrypt() {}

	//���ܺ���
public:
	//��������
	static WORD EncryptData(LPCTSTR pszSrcData, LPTSTR pszEncrypData, WORD wSize);
	//�⿪����
	static WORD CrevasseData(LPCTSTR pszEncrypData, LPTSTR pszSrcData, WORD wSize);
};

//////////////////////////////////////////////////////////////////////////

#endif