#include "StdAfx.h"
#include "../include/GlobalFunc.h"



DWORD FtoDw( float f )
{
	return (*(DWORD*)&f);
}

float GetRandomFloat(float lowBound, float highBound)
{
	if( lowBound >= highBound )
		return lowBound;
	float f = (rand() % 10000) * 0.0001f; 
	return (f * (highBound - lowBound)) + lowBound; 
}

// 以2为底的数字n的log值.
int	Log2( int n )
{
	for( int i = 1 ; i < 64 ; i++ )
	{
		n = n >> 1;
		if( n == 1 ) return i;
	}
	return 1;
}

// 求得2^n值
int	Pow2( int n )
{
	int val = 1;
	val = val << n;
	return val;
}

// 检查pt是否包含在 rc内.(名为PtInRect()的 API函数不能进行负数(-)值处理.)
BOOL IsInRect( RECT* rc, POINT pt )
{
	if( ( rc->left <= pt.x ) && ( pt.x <= rc->right ) &&
		( rc->bottom <= pt.y ) && ( pt.y <= rc->top ) )
		return TRUE;
	return FALSE;
}

bool IsMachineBigEndian()
{
	unsigned short test = 0x1122;
	unsigned char  *cp = reinterpret_cast<unsigned char*>(&test);
	return (*cp == 0x11);//true  大端  false 小端
}

BOOL IsChineseChar( const char ch )
{
	if( ch & 0x08 )
	{
		// ch 为汉字或全角字符
		return TRUE;
	}
	return FALSE;
}