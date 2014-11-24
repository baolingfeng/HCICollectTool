#pragma once

#include "stdafx.h"
#include "util.h"
#include <Oleacc.h>
#include <OleAuto.h>
#include <UIAutomation.h>
#include <UIAutomationClient.h>

//#include "d3d9.h"
//#include "D3dx9tex.h"

typedef struct struct_ParamData {
	WPARAM wParam;
	LPARAM lParam;
	time_t timer;
	SYSTEMTIME sys;
	POINT pt;
	std::string windowName;
	std::string processName;
} ParamData, *PParamData;


enum FilterType{ BlackList, WhiteList, None, All };

BOOL __declspec(dllexport) WINAPI SetFitlerForHook(FilterType &pFilterType_in, std::vector<std::string> &processNameList_in,
												FilterType &wFilterType_in, std::vector<std::string> &windowNameList_in);

BOOL __declspec(dllexport) WINAPI SetLowKeyboardHook();
BOOL __declspec(dllexport) WINAPI UnlockLowKeyboardHook();

BOOL __declspec(dllexport) WINAPI SetLowMouseHook();
BOOL __declspec(dllexport) WINAPI UnlockLowMouseHook();

LRESULT CALLBACK LLKeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LLMouseHookProc(int nCode, WPARAM wParam, LPARAM lParam);


DWORD WINAPI AccessUIThreadFunction( LPVOID lpParam );
DWORD WINAPI CaptureScreenThreadFunction( LPVOID lpParam );
DWORD WINAPI ReleaseThreadFunction( LPVOID lpParam );


//HRESULT	InitD3D(IDirect3DDevice9** g_pd3dDevice,int* screenW,int* screenH,HWND hWnd);
//BOOL ScreenShot(LPDIRECT3DDEVICE9 lpDevice, std::wstring fileName,RECT rect);