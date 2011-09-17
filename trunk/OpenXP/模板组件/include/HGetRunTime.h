#ifndef __HGETRUNTIME__H__
#define __HGETRUNTIME__H__
#pragma once

class TEMPLATE_CONTROL_API HGetRunTime
{
public:
	HGetRunTime(TCHAR *pszFunc = NULL);
	~HGetRunTime();
private:
	int		m_nElapse;
	TCHAR	m_szFunc[260];
};

#ifdef _DEBUG
	#define ALJ_GET_FUNC_TIME\
		HGetRunTime HRunT;
#else
	#define ALJ_GET_FUNC_TIME\
		HGetRunTime HRunT(__FUNCTION__);
#endif

#endif