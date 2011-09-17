#ifndef __HSYSTEMINFO__H__
#define __HSYSTEMINFO__H__
#pragma once

typedef struct tagSYSINFO
{
	char chResolution[15];//屏幕分辨率
	char chCPU[50]; //CPU类型
	char chProcArch[10];//处理器架构 <x86  amd64  ia64>
	char chComputer[50];  //机器名
	char chIP[15];//ip
	char chMemSize[20];//内存大小
	char chOS[12];//操作系统版本
	BOOL bHaveCamera; //是否有摄像头
}SYSINFO;

class HSYSTEM_API HSystemInfo
{
public:
	HSystemInfo();
	~HSystemInfo();
	 void GetSystemInfo(SYSINFO &si);
};

#endif