#ifndef ENCRYPT_HEAD_FILE
#define ENCRYPT_HEAD_FILE
#pragma once
#pragma warning(disable:4996)
//////////////////////////////////////////////////////////////////////////

//MD5 ������
class TEMPLATE_CONTROL_CLASS CMD5Encrypt
{
	//��������
private:
	//���캯��
	CMD5Encrypt() {}

	//���ܺ���
public:
	//��������
	static void EncryptData(LPCTSTR pszSrcData, TCHAR szMD5Result[33]);
};

//////////////////////////////////////////////////////////////////////////

//��������
class TEMPLATE_CONTROL_CLASS CXOREncrypt
{
	//��������
private:
	//���캯��
	CXOREncrypt() {}

	//���ܺ���
public:
	//��������
	static WORD EncryptData(LPCTSTR pszSrcData, LPTSTR pszEncrypData, WORD wSize);
	//�⿪����
	static WORD CrevasseData(LPCTSTR pszEncrypData, LPTSTR pszSrcData, WORD wSize);
};

//////////////////////////////////////////////////////////////////////////

#endif