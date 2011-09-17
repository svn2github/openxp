#ifndef __HAPIHOOK__H__
#define __HAPIHOOK__H__
#pragma once


typedef int (WINAPI *PFCALLBACK)(WPARAM wParam,LPARAM lParam);

class AFX_EXT_CLASS HAPIHook
{
public:
	HAPIHook();
	~HAPIHook();
public:
/************************ע��API����***************************/ 
	//��װ����
	HHOOK	StartHook(PFCALLBACK pCallBack);
	//ж�ع���
	BOOL	StopHook(); 
/************************ע��API����***************************/ 

/************************ע�뺯������***************************/ 
	//ע�뵽dll�к���
	void*	InterceptDllMember(HMODULE hModule,//������������
							char *szDllName,//dll·��
							char *szFunctionName,//��Ҫע��ĺ���
							DWORD pNewFunction//���滻�ĺ���
							);
/************************ע����������***************************/ 

public:
	PFCALLBACK			m_pCallBack;//�ص�����<Hook API>
protected:
	CRITICAL_SECTION	m_cs;//������<Hook Fuction>
};

extern HAPIHook *g_pHook;



//����ֵ������ָ�� 
template <typename T> 
inline void SetFuncPointerVal(T &func, DWORD val)
{
	__asm
	{ 
		push eax
		push ebx
		mov eax, [val] 
		mov ebx, [func]
		mov [ebx], eax
		pop ebx
		pop eax 
	}
}

// oldFunc    ��ע��ĺ���
// newFunc    �滻����
// outOldFunc ���ر�ע�뺯���ĵ�ַ���Ա�����
#define HookMemberFunc(oldFunc, newFunc, outOldFunc) \
{ \
	DWORD _pold1st, _pold2nd; \
	__asm \
	{ \
		mov _pold1st, offset oldFunc  /*get the 1st call target address*/ \
	} \
	/*get the 2nd jump target address*/ \
	memcpy(&_pold2nd, (char*)_pold1st+1, 4); \
	_pold2nd += 5 + _pold1st; \
	SetFuncPointerVal(outOldFunc, _pold2nd); /*get the 2nd jump address as function pointer and return it*/\
	DWORD _pnew; \
	/* then we overwrite the 2nd jump address with the function of ourselves*/\
	__asm { mov _pnew, offset newFunc } \
	_pnew -= (DWORD)_pold1st + 5; \
	DWORD _dwOldProtect, _dwOldProtect2; \
	VirtualProtect( ( char * )_pold1st + 1, 4, PAGE_EXECUTE_READWRITE, &_dwOldProtect ); \
	memcpy((char*)_pold1st+1, &_pnew, 4); \
	VirtualProtect( ( char * )_pold1st + 1, 4, _dwOldProtect, &_dwOldProtect2 ); \
}

#endif