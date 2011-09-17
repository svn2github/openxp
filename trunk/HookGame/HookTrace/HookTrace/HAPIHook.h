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
/************************注入API钩子***************************/ 
	//安装钩子
	HHOOK	StartHook(PFCALLBACK pCallBack);
	//卸载钩子
	BOOL	StopHook(); 
/************************注入API钩子***************************/ 

/************************注入函数钩子***************************/ 
	//注入到dll中函数
	void*	InterceptDllMember(HMODULE hModule,//宿主函数所在
							char *szDllName,//dll路径
							char *szFunctionName,//将要注入的函数
							DWORD pNewFunction//被替换的函数
							);
/************************注入其他钩子***************************/ 

public:
	PFCALLBACK			m_pCallBack;//回调函数<Hook API>
protected:
	CRITICAL_SECTION	m_cs;//互斥量<Hook Fuction>
};

extern HAPIHook *g_pHook;



//设置值到函数指针 
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

// oldFunc    被注入的函数
// newFunc    替换函数
// outOldFunc 返回被注入函数的地址，以便再用
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