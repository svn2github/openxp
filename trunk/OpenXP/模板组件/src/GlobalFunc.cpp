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

// ��2Ϊ�׵�����n��logֵ.
int	Log2( int n )
{
	for( int i = 1 ; i < 64 ; i++ )
	{
		n = n >> 1;
		if( n == 1 ) return i;
	}
	return 1;
}

// ���2^nֵ
int	Pow2( int n )
{
	int val = 1;
	val = val << n;
	return val;
}

// ���pt�Ƿ������ rc��.(��ΪPtInRect()�� API�������ܽ��и���(-)ֵ����.)
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
	return (*cp == 0x11);//true  ���  false С��
}

BOOL IsChineseChar( const char ch )
{
	if( ch & 0x08 )
	{
		// ch Ϊ���ֻ�ȫ���ַ�
		return TRUE;
	}
	return FALSE;
}