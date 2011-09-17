#ifndef _FILE_VERSION_H_ 
#define _FILE_VERSION_H_ 
#pragma once 

class HSYSTEM_API HFileVersion 
{ 
public: 
	//��ʼ������,��ʼ���ļ�·�� 
	HFileVersion(CString strPath); 
	virtual ~HFileVersion(); 

	//ȡ�����е��ļ��汾��Ϣ������Ϣ����\n�ָ� 
	CString GetFullVersion(); 

	//ȡ�ñ�ע 
	CString GetComments(); 
	//ȡ���ڲ����� 
	CString GetInternalName(); 
	//ȡ�ò�Ʒ���� 
	CString GetProductName(); 
	//ȡ�ù�˾���� 
	CString GetCompanyName(); 
	//ȡ�ð�Ȩ 
	CString GetLegalCopyright(); 
	//ȡ�ò�Ʒ�汾 
	CString GetProductVersion(); 
	//ȡ���ļ����� 
	CString GetFileDescription(); 
	//ȡ�úϷ��̱� 
	CString GetLegalTrademarks(); 
	//ȡ�ø������ڲ��汾˵�� 
	CString GetPrivateBuild(); 
	//ȡ���ļ��汾 
	CString GetFileVersion(); 
	//ȡ��Դ�ļ��� 
	CString GetOriginalFilename(); 
	//ȡ�������ڲ��汾˵�� 
	CString GetSpecialBuild(); 

private: 
	//����ļ�������Ϣ 
	CString GetVer(CString strSubBlock); 
	//����·�� 
	CString m_strPath; 
}; 

#endif //_FILE_VERSION_H_