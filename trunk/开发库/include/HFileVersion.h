#ifndef _FILE_VERSION_H_ 
#define _FILE_VERSION_H_ 
#pragma once 

class HSYSTEM_API HFileVersion 
{ 
public: 
	//初始化函数,初始化文件路径 
	HFileVersion(CString strPath); 
	virtual ~HFileVersion(); 

	//取得所有的文件版本信息，各信息间以\n分隔 
	CString GetFullVersion(); 

	//取得备注 
	CString GetComments(); 
	//取得内部名称 
	CString GetInternalName(); 
	//取得产品名称 
	CString GetProductName(); 
	//取得公司名称 
	CString GetCompanyName(); 
	//取得版权 
	CString GetLegalCopyright(); 
	//取得产品版本 
	CString GetProductVersion(); 
	//取得文件描述 
	CString GetFileDescription(); 
	//取得合法商标 
	CString GetLegalTrademarks(); 
	//取得个人用内部版本说明 
	CString GetPrivateBuild(); 
	//取得文件版本 
	CString GetFileVersion(); 
	//取得源文件名 
	CString GetOriginalFilename(); 
	//取得特殊内部版本说明 
	CString GetSpecialBuild(); 

private: 
	//获得文件各种信息 
	CString GetVer(CString strSubBlock); 
	//保存路径 
	CString m_strPath; 
}; 

#endif //_FILE_VERSION_H_