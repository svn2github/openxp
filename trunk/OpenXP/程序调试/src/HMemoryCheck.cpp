#include "stdafx.h"
#include "../include/HMemoryCheck.h"

class MemoryCheck
{
	friend void HCreateCheckPointA();
	friend void HShowCheckPointA(BOOL bShowAll);
public:
	MemoryCheck(void);

#ifdef _DEBUG
protected:
	void StartPoint();
	void EndPoint(BOOL bShowAll=TRUE);
private:
	CMemoryState m_oldMemState;
	CMemoryState m_newMemState;
	CMemoryState m_diffMemState;
#endif

public:
	~MemoryCheck(void);
};

MemoryCheck::MemoryCheck(void)
{
}

MemoryCheck::~MemoryCheck(void)
{
}

#ifdef _DEBUG
void MemoryCheck::StartPoint()
{	
	AfxEnableMemoryTracking(TRUE);
	afxMemDF = allocMemDF;
	m_oldMemState.Checkpoint();
}

void MemoryCheck::EndPoint( BOOL bShowAll )
{
	m_newMemState.Checkpoint();
	if(m_diffMemState.Difference(m_oldMemState, m_newMemState))
	{
		TRACE(TEXT("\n\n\n.......................Detected Memory State!.......................\n"));
		if (bShowAll)
		{
			m_diffMemState.DumpAllObjectsSince();
		}
		else
		{
			m_diffMemState.DumpStatistics();
		}
		TRACE(TEXT(".......................Detected Memory State!.......................\n\n\n"));
	}
}

MemoryCheck g_memCheck;
void HCreateCheckPointA()
{
	g_memCheck.StartPoint();	
}

void HShowCheckPointA( BOOL bShowAll )
{
	g_memCheck.EndPoint(bShowAll);
}
#endif;