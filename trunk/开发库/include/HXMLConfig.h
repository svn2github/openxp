/********************************************************************
created:	2009/08/17
created:	8:17:2009   13:40
filename: 	HXMLConfig.h
author:		Xingping Huang
Qualifier:  write config or read config
email:		77967292@qq.com	
Special:    	
*********************************************************************/
#ifndef __HXMLCONFIG__H__
#define __HXMLCONFIG__H__
#pragma once
#include <map>
#ifndef FILE_CONTROL_CLASS
	#include "xml/tinystr.h"
	#include "xml/tinyxml.h"
#endif

typedef struct tagConfig
{
public:
	tagConfig(){
		memset(&szFlag,0,sizeof(szFlag));
		memset(&szName,0,sizeof(szName));
		memset(&szFunc,0,sizeof(szFunc));
		memset(&szIntro,0,sizeof(szIntro));
	}
public:
	TCHAR szFlag[MAX_PATH];
	TCHAR szName[MAX_PATH];
	TCHAR szFunc[MAX_PATH];
	TCHAR szIntro[MAX_PATH];
}Config,*pConfig;

class FILE_CONTROL_API HXMLConfig
{
public:
	HXMLConfig(void);
	~HXMLConfig(void);

public://保存xml到硬盘
	void SaveXMLToDisk(const char *pFileDiskPath);

public://创建文件
	void LoadForWrite(const char *pFileDiskPath,
		bool bCreate=true//当bCreate为true时：A.pFileDiskPath不存在则新建；B.pFileDiskPath存在则覆盖
		);
	void LinkRoot(const char *pRoot);
	void WriteString(const char * pIndex,const char *pText);
	void WriteInt(const char *pIndex,int nText);
	void WriteDouble(const char *pIndex,double fText);
	void ClearRoot();

	//读入到XML内存(提取数据较慢)
public:
	bool LoadForRead(const char *pFileDiskPath);
	int GetInt(const char *pHeader,const char *pKey);
	const char* GetString(const char *pHeader,const char *pKey);
	double GetDouble(const char *pHeader,const char *pKey);

	//读入到容器(提取数据较快 -- 结构体要自己配置)
public:
	bool LoadToContainer(const char *pFileDiskPath);
	size_t GetContainerSize() const{return m_mapConfig.size();}
	void ClearContainer(size_t nMax);//nMax 为配置文件中，nID值最大的那个数值，这样可以显是的删除掉
	pConfig GetAttribute(UINT nIndex){return m_mapConfig[nIndex];}//从结构体ID值取出对象的数据
	std::map<UINT,pConfig> m_mapConfig;//第一个参数为 结构体 中的nID  而并不是容器的索引，所以可能排序为1，100，145

private://配置对象
	TiXmlDocument *m_xmlDoc;//xml文档对象
	TiXmlElement *m_xmlChild;//xml子对象
	TiXmlElement *m_xmlRoot;//xml根节点
	
private://创建需要
	TiXmlDeclaration* m_xmlDec;//xml头声明 
	TiXmlComment *m_xmlComment;//xml注释
};


#endif