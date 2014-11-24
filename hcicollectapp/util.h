#include "stdafx.h"
#include<Psapi.h>
#include<stdio.h>
#include<string>
#include<ctime>
#include <codecvt>


using namespace std;


string __declspec(dllexport) WINAPI GetTimeStr(time_t timer);
wstring __declspec(dllexport) WINAPI GetTimeWStr(time_t timer);

wstring __declspec(dllexport) WINAPI GetWindowNameWStr(HWND hwnd);
string __declspec(dllexport) WINAPI GetWindowNameStr(HWND hwnd);

wstring __declspec(dllexport) WINAPI GetWindowClassWStr(HWND hwnd);
string __declspec(dllexport) WINAPI GetWindowClassStr(HWND hwnd);

wstring __declspec(dllexport) WINAPI GetProcessNameWStr(HWND hwnd, DWORD *processId);
string __declspec(dllexport) WINAPI GetProcessNameStr(HWND hwnd, DWORD *processId);

wstring __declspec(dllexport) WINAPI GetMouseEventNameWStr(WPARAM wParam);
string __declspec(dllexport) WINAPI GetMouseEventNameStr(WPARAM wParam);

wstring __declspec(dllexport) WINAPI GetNotNullParentNameWStr(HWND hwnd);
string __declspec(dllexport) WINAPI GetNotNullParentNameStr(HWND hwnd);

wstring __declspec(dllexport) GetSysLocalTimeWStr();
string __declspec(dllexport) GetSysLocalTimeStr();

string __declspec(dllexport) GetSysLocalTimeStr(SYSTEMTIME sys);

string to_utf8(const wchar_t* buffer, int len);
string to_utf8(const wstring& str);

void ReplaceAll(string& str, string from,string to);