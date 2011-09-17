#include "stdafx.h"
#include "../include/HAdoConnect.h"
#include <assert.h>
#include <math.h>
#pragma warning(disable:4996)

//效验结果宏
#define EfficacyResult(hResult) { if (FAILED(hResult)) _com_issue_error(hResult); }

//////////////////////////////////////////////////////////////////////////
CADOError::CADOError()
{
	m_enErrorType = ErrorType_Nothing;
}

void CADOError::SetErrorInfo( enADOErrorType enErrorType, LPCTSTR pszDescribe )
{
	//设置错误
	m_enErrorType = enErrorType;
	sprintf(m_szErrorDescribe,"%s",pszDescribe);
	//抛出错误
	throw this;
}
//////////////////////////////////////////////////////////////////////////

//构造函数
HAdoConnect::HAdoConnect() : m_dwResumeConnectCount(30L),m_dwResumeConnectTime(30L)
{
	::CoInitialize(NULL);//初始化com组件

	//引用计数
	m_nRef = 0;

	//状态变量
	m_dwConnectCount = 0;
	m_dwConnectErrorTime = 0L;

	//创建对象
	m_DBCommand.CreateInstance(__uuidof(Command));
	m_DBRecordset.CreateInstance(__uuidof(Recordset));
	m_DBConnection.CreateInstance(__uuidof(Connection));

	//效验数据
	assert(m_DBCommand != NULL);
	assert(m_DBRecordset != NULL);
	assert(m_DBConnection != NULL);
	if (m_DBCommand == NULL) throw _T("数据库命令对象创建失败");
	if (m_DBRecordset == NULL) throw _T("数据库记录集对象创建失败");
	if (m_DBConnection == NULL) throw _T("数据库连接对象创建失败");

	//设置变量
	m_DBCommand->CommandType = adCmdStoredProc;

	return;
}

//析构函数
HAdoConnect::~HAdoConnect()
{
	//关闭连接
	CloseConnection();

	//释放对象
	m_DBCommand.Release();
	m_DBRecordset.Release();
	m_DBConnection.Release();

	// 释放环境
	::CoUninitialize();
	return;
}

//打开连接
bool __cdecl HAdoConnect::OpenConnection()
{
	//连接数据库
	try
	{
		//关闭连接
		CloseConnection();
		//连接数据库
		EfficacyResult(m_DBConnection->Open(_bstr_t(m_szConnect),L"",L"",adConnectUnspecified));
		m_DBConnection->CursorLocation = adUseClient;
		m_DBCommand->ActiveConnection = m_DBConnection;

		//设置变量
		m_dwConnectCount = 0L;
		m_dwConnectErrorTime = 0L;
		return true;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); 
	}
	return false;
}

//关闭记录
bool __cdecl HAdoConnect::CloseRecordset()
{
	try
	{
		if (IsRecordsetOpened())
		{ 
			EfficacyResult(m_DBRecordset->Close());
		}
		return true;
	}
	catch (CComError & ComError)
	{ 
		SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); 
	}
	return false;
}

//关闭连接
bool __cdecl HAdoConnect::CloseConnection()
{
	try
	{
		CloseRecordset();
		if ((m_DBConnection != NULL)&&(m_DBConnection->GetState() != adStateClosed))
		{
			EfficacyResult(m_DBConnection->Close());
		}
		return true;
	}
	catch (CComError & ComError)
	{ 
		SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); 
	}
	return false;
}

//重新连接
bool __cdecl HAdoConnect::TryConnectAgain(bool bFocusConnect, CComError * pComError)
{
	try
	{
		//判断重连
		bool bReConnect = bFocusConnect;
		if (bReConnect == false)
		{
			DWORD dwNowTime = (DWORD)time(NULL);
			if ((m_dwConnectErrorTime+m_dwResumeConnectTime) > dwNowTime)
			{
				bReConnect = true;
			}
		}
		if ((bReConnect == false) && (m_dwConnectCount>m_dwResumeConnectCount))
		{
			bReConnect = true;
		}

		//设置变量
		m_dwConnectCount++;
		m_dwConnectErrorTime = (DWORD)time(NULL);
		if (bReConnect == false)
		{
			if (pComError != NULL)
			{ 
				SetErrorInfo(ErrorType_Connect,GetComErrorDescribe(*pComError));
			}
			return false;
		}

		//重新连接
		OpenConnection();
		return true;
	}
	catch (CADOError *pADOError)
	{
		//重新连接错误
		if (pComError != NULL)
		{ 
			SetErrorInfo(ErrorType_Connect,GetComErrorDescribe(*pComError));
		}
		else
		{ 
			throw pADOError;
		}
	}
	return false;
}

//设置信息
bool __cdecl HAdoConnect::SetConnectionInfo(LPCTSTR szIP, WORD wPort, LPCTSTR szData, LPCTSTR szName, LPCTSTR szPass)
{
	//效验参数 
	assert(szIP!=NULL);
	assert(szData!=NULL);
	assert(szName!=NULL);
	assert(szPass!=NULL);

	//构造连接字符串
	sprintf(m_szConnect,_T("Provider=SQLOLEDB.1;Password=%s;Persist Security Info=True;User ID=%s;Initial Catalog=%s;Data Source=%s,%ld;"),
		szPass,szName,szData,szIP,wPort);
	return true;
}

//是否连接错误
bool __cdecl HAdoConnect::IsConnectError()
{
	try
	{
		//状态判断
		if (m_DBConnection == NULL)
		{
			return true;
		}
		if (m_DBConnection->GetState() == adStateClosed)
		{
			return true;
		}

		//参数判断
		long lErrorCount = m_DBConnection->Errors->Count;
		if (lErrorCount > 0L)
		{
			ErrorPtr pError = NULL;
			for(long i = 0; i < lErrorCount; i++)
			{
				pError = m_DBConnection->Errors->GetItem(i);
				if (pError->Number == 0x80004005)
				{
					return true;
				}
			}
		}
		return false;
	}
	catch (CComError & ComError)
	{ 
		SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); 
	}
	return false;
}

//是否打开
bool __cdecl HAdoConnect::IsRecordsetOpened()
{
	if (m_DBRecordset == NULL)
	{
		return false;
	}	
	if (m_DBRecordset->GetState() == adStateClosed)
	{ 
		return false;
	}
	return true;
}

//往下移动
void __cdecl HAdoConnect::MoveToNext()
{
	try
	{ 
		m_DBRecordset->MoveNext(); 
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); 
	}
	return;
}

//移到开头
void __cdecl HAdoConnect::MoveToFirst()
{
	try
	{ 
		m_DBRecordset->MoveFirst(); 
	}
	catch (CComError & ComError)
	{ 
		SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError));
	}
	return;
}

//是否结束
bool __cdecl HAdoConnect::IsEndRecordset()
{
	try
	{
		return (m_DBRecordset->EndOfFile == VARIANT_TRUE); 
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError));
	}
	return true;
}

//获取数目
long __cdecl HAdoConnect::GetRecordCount()
{
	try
	{
		if (m_DBRecordset == NULL)
		{ 
			return 0;
		}
		return m_DBRecordset->GetRecordCount();
	}
	catch (CComError & ComError)
	{ 
		SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError));
	}

	return 0;
}

//获取大小
long __cdecl HAdoConnect::GetActualSize(LPCTSTR pszParamName)
{
	assert(pszParamName!=NULL);
	try 
	{
		return m_DBRecordset->Fields->Item[pszParamName]->ActualSize;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); 
	}
	return -1;
}

//绑定对象
//bool __cdecl HAdoConnect::BindToRecordset(CADORecordBinding * pBind)
//{
/*assert(pBind!=NULL);
try 
{
IADORecordBindingPtr pIBind(m_DBRecordset);
pIBind->BindToRecordset(pBind);
return true;
}
catch (CComError & ComError) { SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); }*/

//	return false;
//}

//获取参数
bool __cdecl HAdoConnect::GetFieldValue(LPCTSTR lpFieldName, BYTE & bValue)
{
	try
	{
		bValue = 0;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch(vtFld.vt)
		{
		case VT_BOOL:
			{
				bValue = (vtFld.boolVal != 0)?1:0;
				break;
			}
		case VT_I2:
		case VT_UI1:
			{
				bValue = (vtFld.iVal > 0)?1:0;
				break;
			}
		case VT_NULL:
		case VT_EMPTY:
			{
				bValue = 0;
				break;
			}
		default: 
			bValue = (BYTE)vtFld.iVal;
		}	
		return true;
	}
	catch (CComError & ComError)
	{ 
		SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); 
	}

	return false;
}

//获取参数
bool __cdecl HAdoConnect::GetFieldValue(LPCTSTR lpFieldName, UINT & ulValue)
{
	try
	{
		ulValue = 0L;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if ((vtFld.vt != VT_NULL)&&(vtFld.vt != VT_EMPTY))
		{
			ulValue = vtFld.lVal;
		}
		return true;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError));
	}
	return false;
}

//获取参数
bool __cdecl HAdoConnect::GetFieldValue(LPCTSTR lpFieldName, DOUBLE & dbValue)
{	
	try
	{
		dbValue = 0.0L;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch(vtFld.vt)
		{
		case VT_R4:
			{
				dbValue = vtFld.fltVal;
				break;
			}
		case VT_R8:
			{
				dbValue = vtFld.dblVal;
				break;
			}
		case VT_DECIMAL:
			{
				dbValue = vtFld.decVal.Lo32;
				dbValue *= (vtFld.decVal.sign == 128)?-1:1;
				dbValue /= pow((float)10,vtFld.decVal.scale); 
				break;
			}
		case VT_UI1:
			{
				dbValue = vtFld.iVal;
				break;
			}
		case VT_I2:
		case VT_I4:
			{
				dbValue = vtFld.lVal;
				break;
			}
		case VT_NULL:
		case VT_EMPTY:
			{
				dbValue = 0.0L;
				break;
			}
		default: 
			dbValue = vtFld.dblVal;
		}
		return true;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); 
	}
	return false;
}

//获取参数
bool __cdecl HAdoConnect::GetFieldValue(LPCTSTR lpFieldName, LONG & lValue)
{
	try
	{
		lValue = 0L;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if ((vtFld.vt != VT_NULL)&&(vtFld.vt != VT_EMPTY))
		{ 
			lValue = vtFld.lVal;
		}
		return true;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); 
	}
	return false;
}

//获取参数
bool __cdecl HAdoConnect::GetFieldValue(LPCTSTR lpFieldName, DWORD & ulValue)
{
	try
	{
		ulValue = 0L;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if ((vtFld.vt != VT_NULL)&&(vtFld.vt != VT_EMPTY))
		{
			ulValue = vtFld.ulVal;
		}
		return true;
	}
	catch (CComError & ComError)
	{ 
		SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); 
	}
	return false;
}

//获取参数
bool __cdecl HAdoConnect::GetFieldValue(LPCTSTR lpFieldName, INT & nValue)
{
	try
	{
		nValue = 0;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch(vtFld.vt)
		{
		case VT_BOOL:
			{
				nValue = vtFld.boolVal;
				break;
			}
		case VT_I2:
		case VT_UI1:
			{
				nValue = vtFld.iVal;
				break;
			}
		case VT_NULL:
		case VT_EMPTY:
			{
				nValue = 0;
				break;
			}
		default: 
			nValue = vtFld.iVal;
		}	
		return true;
	}
	catch (CComError & ComError)
	{ 
		SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError));
	}
	return false;
}

//获取参数
bool __cdecl HAdoConnect::GetFieldValue(LPCTSTR lpFieldName, __int64 & llValue)
{
	try
	{
		llValue = 0L;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if ((vtFld.vt != VT_NULL)&&(vtFld.vt != VT_EMPTY))
		{
			llValue = vtFld.llVal;
		}
		return true;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); 
	}
	return false;
}

//获取参数
bool __cdecl HAdoConnect::GetFieldValue(LPCTSTR lpFieldName, TCHAR szBuffer[], UINT uSize)
{
	try
	{
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if (vtFld.vt == VT_BSTR)
		{
			lstrcpy(szBuffer,(char*)_bstr_t(vtFld));
			return true;
		}
		return false;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); 
	}
	return false;
}

//获取参数
bool __cdecl HAdoConnect::GetFieldValue(LPCTSTR lpFieldName, WORD & wValue)
{
	try
	{
		wValue = 0L;
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		if ((vtFld.vt != VT_NULL)&&(vtFld.vt != VT_EMPTY))
		{
			wValue = (WORD)vtFld.ulVal;
		}
		return true;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); 
	}
	return false;
}

//获取参数
bool __cdecl HAdoConnect::GetFieldValue(LPCTSTR lpFieldName, COleDateTime & Time)
{
	try
	{
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch(vtFld.vt)
		{
		case VT_DATE:
			{
				COleDateTime TempTime(vtFld);
				Time = TempTime;
				break;
			}
		case VT_EMPTY:
		case VT_NULL:
			{
				Time.SetStatus(COleDateTime::null);
				break;
			}
		default:
			return false;
		}
		return true;
	}
	catch (CComError & ComError)
	{ 
		SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError));
	}
	return false;
}

//获取参数
bool __cdecl HAdoConnect::GetFieldValue(LPCTSTR lpFieldName, bool & bValue)
{
	try
	{
		_variant_t vtFld = m_DBRecordset->Fields->GetItem(lpFieldName)->Value;
		switch(vtFld.vt)
		{
		case VT_BOOL:
			{
				bValue = (vtFld.boolVal == 0)?false:true;
				break;
			}
		case VT_EMPTY:
		case VT_NULL:
			{
				bValue = false;
				break;
			}
		default:
			return false;
		}
		return true;
	}
	catch (CComError & ComError)
	{ 
		SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError));
	}
	return false;
}

//获取返回数值
long __cdecl HAdoConnect::GetReturnValue()
{
	try
	{
		_ParameterPtr Parameter;
		long lParameterCount = m_DBCommand->Parameters->Count;
		for (long i = 0; i < lParameterCount; i++)
		{
			Parameter = m_DBCommand->Parameters->Item[i];
			if (Parameter->Direction == adParamReturnValue)
			{ 
				return Parameter->Value.lVal;
			}
		}
		assert(FALSE);
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError));
	}
	return 0;
}

//删除参数
void __cdecl HAdoConnect::ClearAllParameters()
{
	try
	{
		long lParameterCount = m_DBCommand->Parameters->Count;
		if (lParameterCount > 0L)
		{
			for (long i = lParameterCount; i > 0; i--)
			{
				m_DBCommand->Parameters->Delete(i-1);
			}
		}
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError));
	}
	return;
}

//设置存储过程
void __cdecl HAdoConnect::SetSPName(LPCTSTR pszSpName)
{
	assert(pszSpName != NULL);
	try
	{ 
		m_DBCommand->CommandText = pszSpName; 
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); 
	}
	return;
}

//获得参数
void __cdecl HAdoConnect::GetParameterValue(LPCTSTR pszParamName, _variant_t & vtValue)
{
	//效验参数
	assert(pszParamName != NULL);

	//获取参数
	try 
	{
		vtValue.Clear();
		vtValue = m_DBCommand->Parameters->Item[pszParamName]->Value;
	}
	catch (CComError & ComError)
	{
		SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); 
	}
	return;
}

//插入参数
void __cdecl HAdoConnect::AddParamter(LPCTSTR pszName, ADOCG::ParameterDirectionEnum Direction, ADOCG::DataTypeEnum Type, long lSize, _variant_t & vtValue)
{
	assert(pszName != NULL);
	try 
	{
		_ParameterPtr Parameter = m_DBCommand->CreateParameter(pszName,Type,Direction,lSize,vtValue);
		m_DBCommand->Parameters->Append(Parameter);
	}
	catch (CComError & ComError)
	{ 
		SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError)); 
	}
	return;
}

//执行语句
bool __cdecl HAdoConnect::Execute(LPCTSTR pszCommand)
{
	assert(pszCommand != NULL);
	try
	{
		m_DBConnection->CursorLocation = adUseClient;
		m_DBConnection->Execute(pszCommand,NULL,adExecuteNoRecords);
		return true;
	}
	catch (CComError & ComError) 
	{ 
		if (IsConnectError() == true)
		{
			TryConnectAgain(false,&ComError);
		}
		else
		{
			SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError));
		}
	}
	return false;
}

//执行命令
bool __cdecl HAdoConnect::ExecuteCommand(bool bRecordset)
{
	try 
	{
		//关闭记录集
		CloseRecordset();
		//执行命令
		if (bRecordset == true)
		{
			m_DBRecordset->PutRefSource(m_DBCommand);
			m_DBRecordset->CursorLocation = adUseClient;
			EfficacyResult(m_DBRecordset->Open((IDispatch *)m_DBCommand,vtMissing,adOpenForwardOnly,adLockReadOnly,adOptionUnspecified));
		}
		else 
		{
			m_DBConnection->CursorLocation=adUseClient;
			EfficacyResult(m_DBCommand->Execute(NULL,NULL,adExecuteNoRecords));
		}
		return true;
	}
	catch (CComError & ComError)
	{ 
		if (IsConnectError() == true)
		{	
			TryConnectAgain(false,&ComError);
		}
		else
		{
			SetErrorInfo(ErrorType_Other,GetComErrorDescribe(ComError));
		}
	}
	return false;
}

//获取错误
LPCTSTR HAdoConnect::GetComErrorDescribe(CComError & ComError)
{
	_bstr_t bstrDescribe(ComError.Description());
	sprintf(m_szErrorDescribe,_T("ADO 错误：0x%8x，%s"),ComError.Error(),(LPCTSTR)bstrDescribe);
	return m_szErrorDescribe;
}

//设置错误
void HAdoConnect::SetErrorInfo(enADOErrorType enErrorType, LPCTSTR pszDescribe)
{
	m_ADOError.SetErrorInfo(enErrorType,pszDescribe);
	return;
}