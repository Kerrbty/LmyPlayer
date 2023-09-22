#pragma once
#include <Windows.h>
#ifdef _cplusplus
extern "C"{
#endif

//////////////////////////////////////////////////////////////////////////
// 接口 
int __stdcall MsgBoxA(HWND hParentWnd, const char* szBody, const char* szTitle = 0);
int __stdcall MsgBoxW(HWND hParentWnd, const wchar_t* szBody, const wchar_t* szTitle = 0);




#ifdef _cplusplus
};
#endif


//////////////////////////////////////////////////////////////////////////
// 不同编译选项 
#ifdef _UNICODE
#define MsgBox MsgBoxW
#else
#define MsgBox MsgBoxA
#endif