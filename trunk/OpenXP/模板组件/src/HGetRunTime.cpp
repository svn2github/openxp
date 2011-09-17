#include "StdAfx.h"
#include "../include/HGetRunTime.h"
#include <time.h>

HGetRunTime::HGetRunTime( TCHAR *pszFunc )
{
	memset(m_szFunc,0,sizeof(m_szFunc));
	if (pszFunc != NULL) strcpy(m_szFunc,pszFunc);
	m_nElapse = clock();
#ifdef _CONSOLE
	#ifdef _IOSTREAM_
		cout << "begine function!\n";
	#else
		printf("begine function!\n");
	#endif
#else
	OutputDebugString("begine function!\n");
#endif
}

HGetRunTime::~HGetRunTime()
{
	m_nElapse = clock()-m_nElapse;
	if(strlen(m_szFunc) == 0)
	{
#ifdef _CONSOLE
	#ifdef _IOSTREAM_
		cout<<"The function's running time: ";
	#else
		printf("The function's running time: ");
	#endif
#else
		OutputDebugString("The function's running time: ");
#endif
	}
	else
	{
#ifdef _CONSOLE
	#ifdef _IOSTREAM_
		cout << m_szFunc <<" fuction running time: ";
	#else
		printf("%s fuction running time: ",m_szFunc);
	#endif
#else
		TRACE("%s fuction running time: ",m_szFunc);
#endif
	}
#ifdef _CONSOLE
	#ifdef _IOSTREAM_
		cout << m_nElapse << endl;
	#else
		printf("%d\n",m_nElapse);
	#endif
#else
	TRACE("%d\n",m_nElapse);
#endif
}
