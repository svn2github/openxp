#ifndef __HADOCONNECT__H__
#define __HADOCONNECT__H__
#pragma once

//ADO �����
#import "MSADO15.DLL" rename_namespace("ADOCG") rename("EOF","EndOfFile")
using namespace ADOCG;
typedef _com_error CComError;		

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

//���ݿ�������
enum enADOErrorType
{
	ErrorType_Nothing = 0,									//û�д���
	ErrorType_Connect = 1,									//���Ӵ���
	ErrorType_Other	  = 2,									//��������
};

//////////////////////////////////////////////////////////////////////////

//ADO ������
class CADOError
{
public:
	CADOError();
	virtual ~CADOError(){}

public://���ܽӿ�
	virtual enADOErrorType __cdecl GetErrorType() { return m_enErrorType; }//��������
	virtual LPCTSTR __cdecl GetErrorDescribe() { return m_szErrorDescribe; }//��������

public://���ܺ���
	void SetErrorInfo(enADOErrorType enErrorType, LPCTSTR pszDescribe);//���ô���

protected://��������
	enADOErrorType					m_enErrorType;						//�������
	TCHAR							m_szErrorDescribe[MAX_PATH];		//������Ϣ
};

//////////////////////////////////////////////////////////////////////////

//ADO ��ѯ��
class DATABASE_CONTROL_API HAdoConnect
{
public:
	HAdoConnect(void);
	~HAdoConnect(void);

public://����ӿ�
	virtual bool __cdecl OpenConnection();//������
	virtual bool __cdecl CloseRecordset();//�رռ�¼
	virtual bool __cdecl CloseConnection();//�ر�����
	virtual bool __cdecl TryConnectAgain(bool bFocusConnect, CComError * pComError);//��������
	virtual bool __cdecl SetConnectionInfo(LPCTSTR szIP, WORD wPort, LPCTSTR szData, LPCTSTR szName, LPCTSTR szPass);//������Ϣ

	//״̬�ӿ�
public:
	virtual bool __cdecl IsConnectError();//�Ƿ����Ӵ���
	virtual bool __cdecl IsRecordsetOpened();//�Ƿ��

	//��¼���ӿ�
public:
	virtual void __cdecl MoveToNext();//�����ƶ�
	virtual void __cdecl MoveToFirst();//�Ƶ���ͷ
	virtual bool __cdecl IsEndRecordset();//�Ƿ����
	virtual long __cdecl GetRecordCount();//��ȡ��Ŀ
	virtual long __cdecl GetActualSize(LPCTSTR pszParamName);//��ȡ��С
	//virtual bool __cdecl BindToRecordset(CADORecordBinding * pBind);//�󶨶���

	//�ֶνӿ�
public:
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, BYTE & bValue);//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, WORD & wValue);//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, INT & nValue);//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, LONG & lValue);//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, DWORD & ulValue);//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, UINT & ulValue);//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, DOUBLE & dbValue);//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, __int64 & llValue);//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, TCHAR szBuffer[], UINT uSize);//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, COleDateTime & Time);//��ȡ����
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, bool & bValue);//��ȡ����

	//�������ӿ�
public:
	virtual void __cdecl SetSPName(LPCTSTR pszSpName);//���ô洢����
	virtual void __cdecl AddParamter(LPCTSTR pszName, ADOCG::ParameterDirectionEnum Direction, ADOCG::DataTypeEnum Type, long lSize, _variant_t & vtValue);//�������
	virtual void __cdecl ClearAllParameters();//ɾ������
	virtual void __cdecl GetParameterValue(LPCTSTR pszParamName, _variant_t & vtValue);//��ò���
	virtual long __cdecl GetReturnValue();//��ȡ������ֵ

	//ִ�нӿ�
public:
	virtual bool __cdecl Execute(LPCTSTR pszCommand);//ִ�����
	virtual bool __cdecl ExecuteCommand(bool bRecordset);//ִ������

	//�ڲ�����
private:
	LPCTSTR GetComErrorDescribe(CComError & ComError);//��ȡ����
	void SetErrorInfo(enADOErrorType enErrorType, LPCTSTR pszDescribe);//���ô���

	//��Ϣ����
protected:
	CADOError                       m_ADOError;                        //ADO������Ϣ
	TCHAR							m_szConnect[MAX_PATH];				//�����ַ���
	TCHAR							m_szErrorDescribe[MAX_PATH];		//������Ϣ
	ULONG                           m_nRef;                             //���ü��� 

	//״̬����
protected:
	DWORD							m_dwConnectCount;					//���Դ���
	DWORD							m_dwConnectErrorTime;				//����ʱ��
	const DWORD						m_dwResumeConnectCount;				//�ָ�����
	const DWORD						m_dwResumeConnectTime;				//�ָ�ʱ��

	//�ں˱���
protected:
	_CommandPtr						m_DBCommand;						//�������
	_RecordsetPtr					m_DBRecordset;						//��¼������
	_ConnectionPtr					m_DBConnection;						//���ݿ����
};

#endif