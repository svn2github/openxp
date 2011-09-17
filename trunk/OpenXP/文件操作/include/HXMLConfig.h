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

public://����xml��Ӳ��
	void SaveXMLToDisk(const char *pFileDiskPath);

public://�����ļ�
	void LoadForWrite(const char *pFileDiskPath,
		bool bCreate=true//��bCreateΪtrueʱ��A.pFileDiskPath���������½���B.pFileDiskPath�����򸲸�
		);
	void LinkRoot(const char *pRoot);
	void WriteString(const char * pIndex,const char *pText);
	void WriteInt(const char *pIndex,int nText);
	void WriteDouble(const char *pIndex,double fText);
	void ClearRoot();

	//���뵽XML�ڴ�(��ȡ���ݽ���)
public:
	bool LoadForRead(const char *pFileDiskPath);
	int GetInt(const char *pHeader,const char *pKey);
	const char* GetString(const char *pHeader,const char *pKey);
	double GetDouble(const char *pHeader,const char *pKey);

	//���뵽����(��ȡ���ݽϿ� -- �ṹ��Ҫ�Լ�����)
public:
	bool LoadToContainer(const char *pFileDiskPath);
	size_t GetContainerSize() const{return m_mapConfig.size();}
	void ClearContainer(size_t nMax);//nMax Ϊ�����ļ��У�nIDֵ�����Ǹ���ֵ�������������ǵ�ɾ����
	pConfig GetAttribute(UINT nIndex){return m_mapConfig[nIndex];}//�ӽṹ��IDֵȡ�����������
	std::map<UINT,pConfig> m_mapConfig;//��һ������Ϊ �ṹ�� �е�nID  �����������������������Կ�������Ϊ1��100��145

private://���ö���
	TiXmlDocument *m_xmlDoc;//xml�ĵ�����
	TiXmlElement *m_xmlChild;//xml�Ӷ���
	TiXmlElement *m_xmlRoot;//xml���ڵ�
	
private://������Ҫ
	TiXmlDeclaration* m_xmlDec;//xmlͷ���� 
	TiXmlComment *m_xmlComment;//xmlע��
};


#endif