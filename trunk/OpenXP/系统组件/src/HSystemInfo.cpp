#include "StdAfx.h"
#include "../include/HSystemInfo.h"
#include<winsock2.h>
#pragma comment(lib,"ws2_32")


HSystemInfo::HSystemInfo(void)
{
}

HSystemInfo::~HSystemInfo(void)
{
}

void HSystemInfo::GetSystemInfo( SYSINFO &sysinfo )
{
	/////计算机名称///////
	memset(sysinfo.chComputer,0,sizeof(sysinfo.chComputer));
	DWORD len=sizeof(sysinfo.chComputer);
	if(!GetComputerName(sysinfo.chComputer,&len))
		return;
	///////操作系统版本//////////
	sysinfo.chOS[0]=0;
	OSVERSIONINFO osvi;
	memset(&osvi,0,sizeof(osvi));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if(!GetVersionEx(&osvi))
		return;
	switch (osvi.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:
		if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1) 
			strcat(sysinfo.chOS,"Windows 7");
		if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0) 
			strcat(sysinfo.chOS,"Windows Vista");
		if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
			strcat(sysinfo.chOS,"Windows 2003");
		if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
			strcat(sysinfo.chOS,"Windows XP");
		if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
			strcat(sysinfo.chOS,"Windows 2000");
		if (osvi.dwMajorVersion <= 4)
			strcat(sysinfo.chOS,"Windows NT");
		break;
	case VER_PLATFORM_WIN32_WINDOWS:
		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
			strcat(sysinfo.chOS,"Windows 95");
		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
			strcat(sysinfo.chOS,"Windows 98");
		break;
	}
	//////////处理器架构///////////
	SYSTEM_INFO si; 
	ZeroMemory(&si, sizeof(SYSTEM_INFO)); 
	::GetSystemInfo(&si); 
	switch (si.wProcessorArchitecture) 
	{ 
	case PROCESSOR_ARCHITECTURE_INTEL: 
		strcpy(sysinfo.chProcArch,"x86");
		break; 
	case PROCESSOR_ARCHITECTURE_AMD64: 
		strcpy(sysinfo.chProcArch,"amd64");
		break; 
	case PROCESSOR_ARCHITECTURE_IA64: 
		strcpy(sysinfo.chProcArch,"ia64");
		break; 
	} 

	//////////CPU信息///////////
	HKEY hKey;
	DWORD dwType;
	BYTE pData[260]={0};
	DWORD dwLength;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",0, KEY_ALL_ACCESS, &hKey);
	RegQueryValueEx(hKey,"ProcessorNameString",NULL,&dwType,pData,&dwLength);
	if (NULL == hKey)
		RegCloseKey (hKey);
	hKey = NULL;
	strcpy(sysinfo.chCPU,(char*)pData);
	//////////////屏幕分辨率////////////////////////////////
	int cx,cy;
	cx=::GetSystemMetrics(SM_CXSCREEN); 
	cy=::GetSystemMetrics(SM_CYSCREEN); 
	char Display[15];
	wsprintf(Display,"%d*%d",cx,cy);
	strcpy(sysinfo.chResolution,Display);
	////内存大小////////////////
	MEMORYSTATUS mem;
	mem.dwLength=sizeof(mem);
	GlobalMemoryStatus(&mem);
	memset(sysinfo.chMemSize,0,sizeof(sysinfo.chMemSize));
	strcpy(sysinfo.chMemSize,itoa(mem.dwTotalPhys/1024/1024+2,sysinfo.chMemSize,10));
	strcat(sysinfo.chMemSize,"MB");
	///////有无摄像头///////////////////
	char szVfwName[MAX_PATH];
	char szVfwVersion[MAX_PATH];	
	memset(szVfwName,0,MAX_PATH);
	memset(szVfwVersion,0,MAX_PATH);	
	capGetDriverDescription(0, szVfwName, MAX_PATH, szVfwVersion, MAX_PATH);//获取摄像头信息
	if(lstrlen(szVfwName) == 0)
		sysinfo.bHaveCamera=FALSE; //没有摄像头
	else
		sysinfo.bHaveCamera=TRUE; //有摄像头
	///////本机IP///////////////////
	memset(sysinfo.chIP,0,sizeof(sysinfo.chIP));
	WSADATA wsaData;
	char name[255];
	PHOSTENT hostinfo; 
	if (WSAStartup(MAKEWORD(2,0),&wsaData) == 0) 
	{
		if(gethostname(name, sizeof(name)) == 0) 
		{ 
			if((hostinfo = gethostbyname(name)) != NULL)
			{ 
				strcpy(sysinfo.chIP,inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list));
			} 
		} 
		WSACleanup();
	} 
}