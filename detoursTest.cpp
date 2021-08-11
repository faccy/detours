#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <Windows.h>
//包含Detour的头文件和库文件
#include "detours.h"
#pragma comment (lib,"detours.lib")
//保存函数原型
static int (WINAPI* OldMessageBoxW)(
	_In_opt_ HWND hWnd,
	_In_opt_ LPCWSTR lpText,
	_In_opt_ LPCWSTR lpCaption,
	_In_ UINT uType) = MessageBoxW;

//改写函数
static int WINAPI NewMessageBoxW(
	_In_opt_ HWND hWnd,
	_In_opt_ LPCWSTR lpText,
	_In_opt_ LPCWSTR lpCaption,
	_In_ UINT uType)
{
	return OldMessageBoxW(NULL, L"new MessageBox", L"Please", MB_OK);
}

//开始下钩子
void StartHook() {
	//开始事务
	DetourTransactionBegin();
	//更新线程信息  
	DetourUpdateThread(GetCurrentThread());
	//将拦截的函数附加到原函数的地址上
	DetourAttach(&(PVOID&)OldMessageBoxW, NewMessageBoxW);
	//结束事务
	DetourTransactionCommit();
}

//解除钩子
void EndHook() {
	//开始事务
	DetourTransactionBegin();
	//更新线程信息 
	DetourUpdateThread(GetCurrentThread());
	//将拦截的函数从原函数的地址上解除
	DetourDetach(&(PVOID&)OldMessageBoxW, NewMessageBoxW);
	//结束事务
	DetourTransactionCommit();
}

int _tmain(int argc, _TCHAR* argv[])
{
	MessageBoxW(NULL, L"还没有Hook", L"测试", MB_OK);
	StartHook();
	MessageBoxW(NULL, L"Hook后的", L"测试", MB_OK);

	Sleep(4000);
	EndHook();
}
