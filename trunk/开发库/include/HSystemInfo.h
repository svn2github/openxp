#ifndef __HSYSTEMINFO__H__
#define __HSYSTEMINFO__H__
#pragma once

typedef struct tagSYSINFO
{
	char chResolution[15];//��Ļ�ֱ���
	char chCPU[50]; //CPU����
	char chProcArch[10];//�������ܹ� <x86  amd64  ia64>
	char chComputer[50];  //������
	char chIP[15];//ip
	char chMemSize[20];//�ڴ��С
	char chOS[12];//����ϵͳ�汾
	BOOL bHaveCamera; //�Ƿ�������ͷ
}SYSINFO;

class HSYSTEM_API HSystemInfo
{
public:
	HSystemInfo();
	~HSystemInfo();
	 void GetSystemInfo(SYSINFO &si);
};

#endif