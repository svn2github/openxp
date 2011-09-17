#include "StdAfx.h"
#include "../include/HNetCardInfo.h"
#pragma comment(lib,"netapi32.lib")

HNetCardInfo::HNetCardInfo()
{
	m_TotalNetCards = 0;
	_tcscpy(m_szDomain,_T(""));
	_tcscpy(m_szHostName,_T(""));
	m_IPEnableRouter = 2;
	m_EnableDNS = 2;
	m_SystemType = Unknow;
}

HNetCardInfo::~HNetCardInfo()
{
}

BOOL HNetCardInfo::GetSetting()
{
	m_SystemType = GetSystemType();

	if (m_SystemType == Windows9X)
		return GetSettingOfWin9X();
	else if(m_SystemType == WinNT4orHigher)
		return GetSettingOfWinNT();
	else//��֧���ϾɵĲ���ϵͳ
		return FALSE;
}

Win32Type HNetCardInfo::GetSystemType()
{
	Win32Type  SystemType;
	DWORD winVer;
	OSVERSIONINFO *osvi;

	winVer = GetVersion();
	if(winVer < 0x80000000)
	{
		/*NT */
		SystemType = WinNT3;
		osvi = (OSVERSIONINFO *)malloc(sizeof(OSVERSIONINFO));
		if (osvi != NULL)
		{
			memset(osvi,0,sizeof(OSVERSIONINFO));
			osvi->dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			GetVersionEx(osvi);
			if (osvi->dwMajorVersion >= 4L)
				SystemType = WinNT4orHigher;//yup, it is NT 4 or higher!
			free(osvi);
		}
	}
	else if  (LOBYTE(LOWORD(winVer)) < 4)
		SystemType=Win32s;/*Win32s*/
	else
		SystemType=Windows9X;/*Windows9X*/
	return SystemType;
}

BOOL HNetCardInfo::GetSettingOfWin9X()
{
	LONG lRet;
	HKEY hMainKey;
	TCHAR szNameServer[256];

	//�õ����������غ�DNS������
	lRet = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("System\\CurrentControlSet\\Services\\VxD\\MSTCP"),0,KEY_READ,&hMainKey);
	if(lRet == ERROR_SUCCESS)
	{
		DWORD dwType,dwDataSize = 256;
		::RegQueryValueEx(hMainKey,_T("Domain"),NULL,&dwType,(LPBYTE)m_szDomain,&dwDataSize);
		dwDataSize = 256;
		::RegQueryValueEx(hMainKey,_T("Hostname"),NULL,&dwType,(LPBYTE)m_szHostName,&dwDataSize);
		dwDataSize = 256;
		::RegQueryValueEx(hMainKey,_T("EnableDNS"),NULL,&dwType,(LPBYTE)&m_EnableDNS,&dwDataSize);
		dwDataSize = 256;
		::RegQueryValueEx(hMainKey,_T("NameServer"),NULL,&dwType,(LPBYTE)szNameServer,&dwDataSize);
	}
	::RegCloseKey(hMainKey);

	HKEY hNetCard = NULL;
	//����CTcpCfg��ľ�̬�����õ���������Ŀ����Ӧ��MAC��ַ
	m_TotalNetCards = GetMacAddress(m_MacAddr);

	lRet = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("System\\CurrentControlSet\\Services\\Class\\Net"),0,KEY_READ,&hNetCard);
	if(lRet != ERROR_SUCCESS)//�˴�ʧ�ܾͷ���
	{
		if(hNetCard != NULL)
			::RegCloseKey(hNetCard);
		return FALSE;
	}

	DWORD dwSubKeyNum = 0,dwSubKeyLen = 256;
	//�õ��Ӽ��ĸ�����ͨ���������������
	lRet = ::RegQueryInfoKey(hNetCard,NULL,NULL,NULL,&dwSubKeyNum,&dwSubKeyLen,
		NULL,NULL,NULL,NULL,NULL,NULL);
	if(lRet == ERROR_SUCCESS)
	{
		//		m_TotalNetCards = dwSubKeyNum;//���������Դ�Ϊ��
		LPTSTR lpszKeyName = new TCHAR[dwSubKeyLen + 1];
		DWORD dwSize;
		for(int i = 0; i < (int)m_TotalNetCards; i++)
		{
			TCHAR szNewKey[256];
			HKEY hNewKey;
			DWORD dwType = REG_SZ,dwDataSize = 256;
			dwSize = dwSubKeyLen + 1;
			lRet = ::RegEnumKeyEx(hNetCard,i,lpszKeyName,&dwSize,NULL,NULL,NULL,NULL);
			if(lRet == ERROR_SUCCESS)
			{
				lRet = ::RegOpenKeyEx(hNetCard,lpszKeyName,0,KEY_READ,&hNewKey);
				if(lRet == ERROR_SUCCESS)
					::RegQueryValueEx(hNewKey,_T("DriverDesc"),NULL,&dwType,(LPBYTE)m_Cards[i].szDescription,&dwDataSize);
				::RegCloseKey(hNewKey);
				wsprintf(szNewKey,_T("System\\CurrentControlSet\\Services\\Class\\NetTrans\\%s"),lpszKeyName);
				lRet = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,szNewKey,0,KEY_READ,&hNewKey);
				if(lRet == ERROR_SUCCESS)
				{
					dwDataSize = 256;
					::RegQueryValueEx(hNewKey,_T("DefaultGateway"),NULL,&dwType,(LPBYTE)m_Cards[i].szGateWay,&dwDataSize);
					ProcessMultiString(m_Cards[i].szGateWay,dwDataSize);
					dwDataSize = 256;
					::RegQueryValueEx(hNewKey,_T("IPAddress"),NULL,&dwType,(LPBYTE)m_Cards[i].szIpAddress,&dwDataSize);
					ProcessMultiString(m_Cards[i].szIpAddress,dwDataSize);
					dwDataSize = 256;
					::RegQueryValueEx(hNewKey,_T("IPMask"),NULL,&dwType,(LPBYTE)m_Cards[i].szIpMask,&dwDataSize);
					ProcessMultiString(m_Cards[i].szIpMask,dwDataSize);
					//����ǰ��õ���DNS������
					_tcscpy(m_Cards[i].szDNSNameServer,szNameServer);
				}
				::RegCloseKey(hNewKey);
			}
			m_Cards[i].szMacAddr[0] = m_MacAddr[i].b1;
			m_Cards[i].szMacAddr[1] = m_MacAddr[i].b2;
			m_Cards[i].szMacAddr[2] = m_MacAddr[i].b3;
			m_Cards[i].szMacAddr[3] = m_MacAddr[i].b4;
			m_Cards[i].szMacAddr[4] = m_MacAddr[i].b5;
			m_Cards[i].szMacAddr[5] = m_MacAddr[i].b6;
		}
	}
	::RegCloseKey(hNetCard);
	return lRet == ERROR_SUCCESS ? TRUE : FALSE;
}

int HNetCardInfo::GetMacAddress(LPMAC_ADDRESS pMacAddr)
{
	NCB ncb; 
	UCHAR uRetCode;
	int num = 0;
	LANA_ENUM lana_enum; 
	memset(&ncb, 0, sizeof(ncb) ); 
	ncb.ncb_command = NCBENUM; 
	ncb.ncb_buffer = (unsigned char *)&lana_enum; 
	ncb.ncb_length = sizeof(lana_enum); 
	//����������NCBENUM����,�Ի�ȡ��ǰ������������Ϣ,���ж��ٸ�����
	//ÿ�������ı�ŵ� 
	uRetCode = Netbios(&ncb);
	if (uRetCode == 0) 
	{
		num = lana_enum.length;
		//��ÿһ������,�����������Ϊ������,��ȡ��MAC��ַ 
		for (int i = 0; i < num; i++)
		{
			ASTAT Adapter;
			if(GetAddressByIndex(lana_enum.lana[i],Adapter) == 0)
			{
				pMacAddr[i].b1 = Adapter.adapt.adapter_address[0];
				pMacAddr[i].b2 = Adapter.adapt.adapter_address[1];
				pMacAddr[i].b3 = Adapter.adapt.adapter_address[2];
				pMacAddr[i].b4 = Adapter.adapt.adapter_address[3];
				pMacAddr[i].b5 = Adapter.adapt.adapter_address[4];
				pMacAddr[i].b6 = Adapter.adapt.adapter_address[5];
			}
		}
	}
	return num;
}

BOOL HNetCardInfo::GetSettingOfWinNT()
{
	LONG lRtn;
	HKEY hMainKey;
	TCHAR szParameters[256];
	//������������������Ƿ�ʹ��IP·��
	_tcscpy(szParameters,_T("SYSTEM\\ControlSet001\\Services\\Tcpip\\Parameters"));
	lRtn = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,szParameters,0,KEY_READ,&hMainKey);
	if(lRtn == ERROR_SUCCESS)
	{
		DWORD dwType,dwDataSize = 256;
		::RegQueryValueEx(hMainKey,_T("Domain"),NULL,&dwType,(LPBYTE)m_szDomain,&dwDataSize);
		dwDataSize = 256;
		::RegQueryValueEx(hMainKey,_T("Hostname"),NULL,&dwType,(LPBYTE)m_szHostName,&dwDataSize);
		dwDataSize = 256;
		//		BYTE szBuf[16];
		::RegQueryValueEx(hMainKey,_T("IPEnableRouter"),NULL,&dwType,(LPBYTE)&m_IPEnableRouter,&dwDataSize);
	}
	::RegCloseKey(hMainKey);

	//���IP��ַ��DNS��������������
	HKEY hNetCard = NULL;
	m_TotalNetCards = GetMacAddress(m_MacAddr);

	lRtn = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\NetworkCards"),0,KEY_READ,&hNetCard);
	if(lRtn != ERROR_SUCCESS)//�˴�ʧ�ܾͷ���
	{
		if(hNetCard != NULL)
			::RegCloseKey(hNetCard);
		return FALSE;
	}

	DWORD dwSubKeyNum = 0,dwSubKeyLen = 256;
	//�õ��Ӽ��ĸ�����ͨ���������������
	lRtn = ::RegQueryInfoKey(hNetCard,NULL,NULL,NULL,&dwSubKeyNum,&dwSubKeyLen,
		NULL,NULL,NULL,NULL,NULL,NULL);
	if(lRtn == ERROR_SUCCESS)
	{
		m_TotalNetCards = dwSubKeyNum;//���������Դ�Ϊ��
		LPTSTR lpszKeyName = new TCHAR[dwSubKeyLen + 1];
		DWORD dwSize;
		for(int i = 0; i < (int)dwSubKeyNum; i++)
		{
			TCHAR szServiceName[256];
			HKEY hNewKey;
			DWORD dwType = REG_SZ,dwDataSize = 256;
			dwSize = dwSubKeyLen + 1;
			::RegEnumKeyEx(hNetCard,i,lpszKeyName,&dwSize,NULL,NULL,NULL,NULL);
			lRtn = ::RegOpenKeyEx(hNetCard,lpszKeyName,0,KEY_READ,&hNewKey);
			if(lRtn == ERROR_SUCCESS)
			{
				lRtn = ::RegQueryValueEx(hNewKey,_T("Description"),NULL,&dwType,(LPBYTE)m_Cards[i].szDescription,&dwDataSize);
				dwDataSize = 256;
				lRtn = ::RegQueryValueEx(hNewKey,_T("ServiceName"),NULL,&dwType,(LPBYTE)szServiceName,&dwDataSize);
				if(lRtn == ERROR_SUCCESS)
				{
					TCHAR szNewKey[256];
					wsprintf(szNewKey,_T("%s\\Interfaces\\%s"),szParameters,szServiceName);
					HKEY hTcpKey;
					lRtn = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,szNewKey,0,KEY_READ,&hTcpKey);
					if(lRtn == ERROR_SUCCESS)
					{
						dwDataSize = 256;
						::RegQueryValueEx(hTcpKey,_T("DefaultGateway"),NULL,&dwType,(LPBYTE)m_Cards[i].szGateWay,&dwDataSize);
						ProcessMultiString(m_Cards[i].szGateWay,dwDataSize);
						dwDataSize = 256;
						::RegQueryValueEx(hTcpKey,_T("IPAddress"),NULL,&dwType,(LPBYTE)m_Cards[i].szIpAddress,&dwDataSize);
						ProcessMultiString(m_Cards[i].szIpAddress,dwDataSize);
						dwDataSize = 256;
						::RegQueryValueEx(hTcpKey,_T("SubnetMask"),NULL,&dwType,(LPBYTE)m_Cards[i].szIpMask,&dwDataSize);
						ProcessMultiString(m_Cards[i].szIpMask,dwDataSize);
						dwDataSize = 256;
						::RegQueryValueEx(hTcpKey,_T("NameServer"),NULL,&dwType,(LPBYTE)m_Cards[i].szDNSNameServer,&dwDataSize);
					}
					::RegCloseKey(hTcpKey);
				}
			}
			::RegCloseKey(hNewKey);
			m_Cards[i].szMacAddr[0] = m_MacAddr[i].b1;
			m_Cards[i].szMacAddr[1] = m_MacAddr[i].b2;
			m_Cards[i].szMacAddr[2] = m_MacAddr[i].b3;
			m_Cards[i].szMacAddr[3] = m_MacAddr[i].b4;
			m_Cards[i].szMacAddr[4] = m_MacAddr[i].b5;
			m_Cards[i].szMacAddr[5] = m_MacAddr[i].b6;
		}
		delete[] lpszKeyName;
	}
	::RegCloseKey(hNetCard);
	return lRtn == ERROR_SUCCESS ? TRUE : FALSE;
}


UCHAR HNetCardInfo::GetAddressByIndex(int lana_num, ASTAT &Adapter)
{
	NCB ncb; 
	UCHAR uRetCode; 
	memset(&ncb, 0, sizeof(ncb) ); 
	ncb.ncb_command = NCBRESET; 
	ncb.ncb_lana_num = lana_num; 
	//ָ��������,���ȶ�ѡ������������һ��NCBRESET����,�Ա���г�ʼ�� 
	uRetCode = Netbios(&ncb ); 
	memset(&ncb, 0, sizeof(ncb) ); 
	ncb.ncb_command = NCBASTAT; 
	ncb.ncb_lana_num = lana_num;   //ָ�������� 
	strcpy((char *)ncb.ncb_callname,"*      " ); 
	ncb.ncb_buffer = (unsigned char *)&Adapter; 
	//ָ�����ص���Ϣ��ŵı��� 
	ncb.ncb_length = sizeof(Adapter); 
	//����,���Է���NCBASTAT�����Ի�ȡ��������Ϣ 
	uRetCode = Netbios(&ncb ); 
	return uRetCode;
} 

void HNetCardInfo::ProcessMultiString(LPTSTR lpszString, DWORD dwSize)
{ 
	for(int i = 0; i < int(dwSize - 2); i++)
	{
		if(lpszString[i] == _T('\0'))
			lpszString[i] = _T(',');
	}
}