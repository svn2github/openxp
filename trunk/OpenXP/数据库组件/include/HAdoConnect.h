#ifndef __HADOCONNECT__H__
#define __HADOCONNECT__H__
#pragma once

//ADO 导入库
#import "MSADO15.DLL" rename_namespace("ADOCG") rename("EOF","EndOfFile")
using namespace ADOCG;
typedef _com_error CComError;		

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

//数据库错误代码
enum enADOErrorType
{
	ErrorType_Nothing = 0,									//没有错误
	ErrorType_Connect = 1,									//连接错误
	ErrorType_Other	  = 2,									//其他错误
};

//////////////////////////////////////////////////////////////////////////

//ADO 错误类
class CADOError
{
public:
	CADOError();
	virtual ~CADOError(){}

public://功能接口
	virtual enADOErrorType __cdecl GetErrorType() { return m_enErrorType; }//错误类型
	virtual LPCTSTR __cdecl GetErrorDescribe() { return m_szErrorDescribe; }//错误描述

public://功能函数
	void SetErrorInfo(enADOErrorType enErrorType, LPCTSTR pszDescribe);//设置错误

protected://变量定义
	enADOErrorType					m_enErrorType;						//错误代号
	TCHAR							m_szErrorDescribe[MAX_PATH];		//错误信息
};

//////////////////////////////////////////////////////////////////////////

//ADO 查询类
class DATABASE_CONTROL_API HAdoConnect
{
public:
	HAdoConnect(void);
	~HAdoConnect(void);

public://管理接口
	virtual bool __cdecl OpenConnection();//打开连接
	virtual bool __cdecl CloseRecordset();//关闭记录
	virtual bool __cdecl CloseConnection();//关闭连接
	virtual bool __cdecl TryConnectAgain(bool bFocusConnect, CComError * pComError);//重新连接
	virtual bool __cdecl SetConnectionInfo(LPCTSTR szIP, WORD wPort, LPCTSTR szData, LPCTSTR szName, LPCTSTR szPass);//设置信息

	//状态接口
public:
	virtual bool __cdecl IsConnectError();//是否连接错误
	virtual bool __cdecl IsRecordsetOpened();//是否打开

	//记录集接口
public:
	virtual void __cdecl MoveToNext();//往下移动
	virtual void __cdecl MoveToFirst();//移到开头
	virtual bool __cdecl IsEndRecordset();//是否结束
	virtual long __cdecl GetRecordCount();//获取数目
	virtual long __cdecl GetActualSize(LPCTSTR pszParamName);//获取大小
	//virtual bool __cdecl BindToRecordset(CADORecordBinding * pBind);//绑定对象

	//字段接口
public:
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, BYTE & bValue);//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, WORD & wValue);//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, INT & nValue);//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, LONG & lValue);//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, DWORD & ulValue);//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, UINT & ulValue);//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, DOUBLE & dbValue);//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, __int64 & llValue);//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, TCHAR szBuffer[], UINT uSize);//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, COleDateTime & Time);//获取参数
	virtual bool __cdecl GetFieldValue(LPCTSTR lpFieldName, bool & bValue);//获取参数

	//命令对象接口
public:
	virtual void __cdecl SetSPName(LPCTSTR pszSpName);//设置存储过程
	virtual void __cdecl AddParamter(LPCTSTR pszName, ADOCG::ParameterDirectionEnum Direction, ADOCG::DataTypeEnum Type, long lSize, _variant_t & vtValue);//插入参数
	virtual void __cdecl ClearAllParameters();//删除参数
	virtual void __cdecl GetParameterValue(LPCTSTR pszParamName, _variant_t & vtValue);//获得参数
	virtual long __cdecl GetReturnValue();//获取返回数值

	//执行接口
public:
	virtual bool __cdecl Execute(LPCTSTR pszCommand);//执行语句
	virtual bool __cdecl ExecuteCommand(bool bRecordset);//执行命令

	//内部函数
private:
	LPCTSTR GetComErrorDescribe(CComError & ComError);//获取错误
	void SetErrorInfo(enADOErrorType enErrorType, LPCTSTR pszDescribe);//设置错误

	//信息变量
protected:
	CADOError                       m_ADOError;                        //ADO错误信息
	TCHAR							m_szConnect[MAX_PATH];				//连接字符串
	TCHAR							m_szErrorDescribe[MAX_PATH];		//错误信息
	ULONG                           m_nRef;                             //引用计数 

	//状态变量
protected:
	DWORD							m_dwConnectCount;					//重试次数
	DWORD							m_dwConnectErrorTime;				//错误时间
	const DWORD						m_dwResumeConnectCount;				//恢复次数
	const DWORD						m_dwResumeConnectTime;				//恢复时间

	//内核变量
protected:
	_CommandPtr						m_DBCommand;						//命令对象
	_RecordsetPtr					m_DBRecordset;						//记录集对象
	_ConnectionPtr					m_DBConnection;						//数据库对象
};

#endif