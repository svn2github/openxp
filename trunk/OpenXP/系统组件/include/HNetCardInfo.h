#ifndef __HNETCARDINFO__H__
#define __HNETCARDINFO__H__
#pragma once
#include <Nb30.h>

//操作系统类型
enum Win32Type
{
	Unknow,
	Win32s,
	Windows9X,
	WinNT3,
	WinNT4orHigher
};

//网卡信息的数据结构，包括记录网卡的厂商及型号，与之绑定的IP地址，网关
//DNS序列，子网掩码和物理地址
typedef struct tagNET_CARD
{
	TCHAR szDescription[256];		//Realtek PCIe GBE Family Controller
	BYTE  szMacAddr[6];				//MAC地址簇
									//TCHAR szAddr[128];wsprintf(szAddr,"%02x-%02x-%02x-%02x-%02x-%02x",szMacAddr[0],szMacAddr[1],szMacAddr[2],szMacAddr[3],szMacAddr[4],szMacAddr[5]);
	TCHAR szGateWay[128];			//192.168.1.1
	TCHAR szIpAddress[128];			//192.168.1.58
	TCHAR szIpMask[128];			//255.255.255.0
	TCHAR szDNSNameServer[128];		//8.8.8.8,8.8.4.4
}NET_CARD,*LPNET_CARD;


#define MAX_CARD  10

class HSYSTEM_API HNetCardInfo
{
private:
	//网络适配器
	typedef struct tagASTAT 
	{ 
		ADAPTER_STATUS adapt; 
		NAME_BUFFER    NameBuff [30]; 
	}ASTAT,*LPASTAT; 

	//存储网卡的MAC地址的结构
	typedef struct tagMAC_ADDRESS
	{
		BYTE b1,b2,b3,b4,b5,b6;
	}MAC_ADDRESS,*LPMAC_ADDRESS;
public:
	HNetCardInfo();
	~HNetCardInfo();
public:
	BOOL GetSetting();
private:
	BOOL GetSettingOfWin9X();
	BOOL GetSettingOfWinNT();
	Win32Type GetSystemType();
	void ProcessMultiString(LPTSTR lpszString,DWORD dwSize);
	UCHAR GetAddressByIndex(int lana_num,ASTAT & Adapter);
	int GetMacAddress(LPMAC_ADDRESS pMacAddr);
public:
	int			m_TotalNetCards;//系统的网卡数
	TCHAR		m_szDomain[16];//域名
	TCHAR		m_szHostName[16];//主机名
	int			m_IPEnableRouter;//是否允许IP路由0-不允许,1-允许,2-未知
	int			m_EnableDNS;//是否允许DNS解析0-不允许,1-允许,2-未知
	NET_CARD    m_Cards[MAX_CARD];//默认的最大网卡数是10
private:
	MAC_ADDRESS m_MacAddr[MAX_CARD];//允许10个网卡
	Win32Type   m_SystemType;//操作系统类型
};

#endif