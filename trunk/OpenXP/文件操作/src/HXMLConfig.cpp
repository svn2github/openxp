#include "stdafx.h"
#include "../include/HXMLConfig.h"
#ifdef _DEBUG
	#pragma comment(lib,"xml/tinyxmld.lib")
#else
	#pragma comment(lib,"xml/tinyxml.lib")
#endif

//���캯��
HXMLConfig::HXMLConfig(void)
{
	m_xmlDoc = new TiXmlDocument();
}

//��������
HXMLConfig::~HXMLConfig(void)
{
	pConfig pCon = NULL;
	size_t nSize = m_mapConfig.size();
	for (size_t i = 0; i <= nSize; ++i)
	{
		pCon = m_mapConfig[i];
		if (pCon != NULL)
		{
			delete pCon;
			pCon = NULL;
		}
	}
	m_mapConfig.clear();

	if (m_xmlDoc)//�ͷŸ��ڵ�
	{
		delete m_xmlDoc;
		m_xmlDoc = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//                     ����XML           
//
//////////////////////////////////////////////////////////////////////////
void HXMLConfig::LoadForWrite(const char *pFileDiskPath,bool bCreate)
{
	if (!bCreate)
	{
		if (LoadForRead(pFileDiskPath))
		{
#ifdef _DEBUG
			AfxMessageBox(_T("�����ļ�ʧ�� -- д���½�ʱ"));
#endif
		}
		return;
	}
	m_xmlDec = new TiXmlDeclaration( "1.0", "gb2312", "yes" );
	m_xmlDoc->LinkEndChild(m_xmlDec);  
	m_xmlRoot = new TiXmlElement("config");
	m_xmlRoot->SetAttribute("Author",  "angus");
	m_xmlDoc->LinkEndChild(m_xmlRoot);  //root 
	m_xmlComment = new TiXmlComment();//comment
	m_xmlComment->SetValue("Settings for config");  
	m_xmlRoot->LinkEndChild(m_xmlComment);  //insert root
}

//������ڵ�
void HXMLConfig::LinkRoot( const char *pRoot )
{
	m_xmlChild = new TiXmlElement(pRoot);  
	m_xmlRoot->LinkEndChild(m_xmlChild);  
}

//д���ִ�
void HXMLConfig::WriteString( const char *pIndex,const char *pText )
{
	m_xmlChild->SetAttribute(pIndex,pText);
}

//д��intֵ
void HXMLConfig::WriteInt( const char *pIndex,int nText )
{
	m_xmlChild->SetAttribute(pIndex,nText);
}

//д��Doubleֵ
void HXMLConfig::WriteDouble( const char *pIndex,double fText )
{
	m_xmlChild->SetDoubleAttribute(pIndex,fText); 
}

//�����ǰ�ڵ�
void HXMLConfig::ClearRoot()
{
	m_xmlChild = NULL;
	delete m_xmlChild;
}

//���浽�ļ�
void HXMLConfig::SaveXMLToDisk( const char *pFileDiskPath )
{
	m_xmlDoc->SaveFile(pFileDiskPath);//
}


//////////////////////////////////////////////////////////////////////////
//
//                     ��XML  
//////////////////////////////////////////////////////////////////////////
bool HXMLConfig::LoadForRead( const char *pFileDiskPath )
{
	if (!m_xmlDoc->LoadFile(pFileDiskPath))
	{
#ifdef _DEBUG
		AfxMessageBox(_T("�����ļ�ʧ�� -- ����ʱ"));
#endif
		return false;
	}
	m_xmlRoot = m_xmlDoc->RootElement();
	return true;
}

//�õ�intֵ
int HXMLConfig::GetInt( const char *pHeader,const char *pKey )
{
	m_xmlChild = m_xmlRoot->FirstChildElement(pHeader);
	int nKey = 255;
	while (true)
	{
		if (m_xmlChild)
		{
			nKey = atoi(m_xmlChild->Attribute(pKey));
			return nKey;
		}
		m_xmlChild = m_xmlChild->NextSiblingElement();
		if (m_xmlChild == NULL) break;
	}
	return nKey;
}

//�õ��ִ�
const char* HXMLConfig::GetString( const char *pHeader,const char *pKey )
{
	m_xmlChild = m_xmlRoot->FirstChildElement(pHeader);
	while(true)
	{
		if (m_xmlChild)
			return m_xmlChild->Attribute(pKey);
		m_xmlChild = m_xmlChild->NextSiblingElement();
		if (m_xmlChild == NULL) break;
	}
	return NULL;
}

//�õ�Doubleֵ
double HXMLConfig::GetDouble( const char *pHeader,const char *pKey )
{
	m_xmlChild = m_xmlRoot->FirstChildElement(pHeader);
	double fKey = 0.0;
	while(true)
	{
		if (m_xmlChild)
		{
			fKey = atol(m_xmlChild->Attribute(pKey));
			return fKey;
		}
		m_xmlChild = m_xmlChild->NextSiblingElement();
		if (m_xmlChild == NULL) break;
	}
	return fKey;
}

//����xml�ļ����ݵ�������
bool HXMLConfig::LoadToContainer( const char *pFileDiskPath )
{
	if (!LoadForRead(pFileDiskPath))
	{
#ifdef _DEBUG
		AfxMessageBox(_T("�����ļ�ʧ�� -- ��������ʱ"));
#endif
		return false;
	}
	static UINT i = 0;
	m_xmlChild = m_xmlRoot->FirstChildElement();
	while(m_xmlChild != NULL)
	{
		pConfig pCon = NULL;
		pCon = new tagConfig();
		if (strcmp(m_xmlChild->Value(),"ALJ_MAX") == 0) 
			continue;
		strcpy(pCon->szFlag,m_xmlChild->Value());
		strcpy(pCon->szName,m_xmlChild->Attribute("name"));
		strcpy(pCon->szFunc,m_xmlChild->Attribute("func"));
		strcpy(pCon->szIntro,m_xmlChild->Attribute("intro"));
		m_mapConfig[i++] = pCon;
		m_xmlChild = m_xmlChild->NextSiblingElement();
	}
	return true;
}

//��������
void HXMLConfig::ClearContainer( size_t nMax )
{
	pConfig pCon = NULL;
	for (size_t i = 0; i <= nMax; ++i)
	{
		pCon = m_mapConfig[i];
		if (pCon != NULL)
		{
			delete pCon;
			pCon = NULL;
		}
	}
	m_mapConfig.clear();
}