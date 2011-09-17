#ifndef __HNETCARDINFO__H__
#define __HNETCARDINFO__H__
#pragma once
#include <Nb30.h>

//����ϵͳ����
enum Win32Type
{
	Unknow,
	Win32s,
	Windows9X,
	WinNT3,
	WinNT4orHigher
};

//������Ϣ�����ݽṹ��������¼�����ĳ��̼��ͺţ���֮�󶨵�IP��ַ������
//DNS���У���������������ַ
typedef struct tagNET_CARD
{
	TCHAR szDescription[256];		//Realtek PCIe GBE Family Controller
	BYTE  szMacAddr[6];				//MAC��ַ��
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
	//����������
	typedef struct tagASTAT 
	{ 
		ADAPTER_STATUS adapt; 
		NAME_BUFFER    NameBuff [30]; 
	}ASTAT,*LPASTAT; 

	//�洢������MAC��ַ�Ľṹ
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
	int			m_TotalNetCards;//ϵͳ��������
	TCHAR		m_szDomain[16];//����
	TCHAR		m_szHostName[16];//������
	int			m_IPEnableRouter;//�Ƿ�����IP·��0-������,1-����,2-δ֪
	int			m_EnableDNS;//�Ƿ�����DNS����0-������,1-����,2-δ֪
	NET_CARD    m_Cards[MAX_CARD];//Ĭ�ϵ������������10
private:
	MAC_ADDRESS m_MacAddr[MAX_CARD];//����10������
	Win32Type   m_SystemType;//����ϵͳ����
};

#endif