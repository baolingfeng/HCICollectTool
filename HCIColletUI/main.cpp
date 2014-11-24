#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <vector>
#include <string>

#include "../HCICollectDll/HCICollectDll.h"
/* usually, the resource editor creates this file to us: */
#include "resource.h"

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL IsNeedProcess(string processName, string windowName, string parentWindowName);
LRESULT CALLBACK ClipMonitorProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
void SetFilter();

volatile HWND global_hwnd = NULL;
HWND cbhwnd = NULL;

FilterType g_pFilterType;
FilterType g_wFilterType;
std::vector<std::string> g_processNameList;
std::vector<std::string> g_windowNameList;

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE h0, LPWSTR lpCmdLine, int nCmdShow)
{
	HINSTANCE hinst = GetModuleHandle(NULL);
	
	WNDCLASSEX wnd;
	memset( &wnd, 0, sizeof( wnd ) );
	wnd.cbSize = sizeof( wnd );
    wnd.lpszClassName = _T("clipmonitor");
    wnd.lpfnWndProc = (WNDPROC)ClipMonitorProc;
    wnd.hInstance = hinst;

    RegisterClassEx(&wnd);

	global_hwnd = CreateWindowEx(0, wnd.lpszClassName, _T("Clip Monitor"),
		 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, wnd.hInstance, NULL);
	//AddClipboardFormatListener(global_hwnd);
	//HWND cbhwnd = SetClipboardViewer(global_hwnd);

	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile(_T("log"), &wfd);
	if(hFind == INVALID_HANDLE_VALUE || !(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		CreateDirectory(_T("log"),NULL);
		CreateDirectory(_T("log/screen"),NULL);
		CreateDirectory(_T("log/detail"),NULL);
	}
	else
	{
		hFind = FindFirstFile(_T("log/screen"), &wfd);
		if(hFind == INVALID_HANDLE_VALUE || !(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			CreateDirectory(_T("log/screen"),NULL);
		}

		hFind = FindFirstFile(_T("log/detail"), &wfd);
		if(hFind == INVALID_HANDLE_VALUE || !(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			CreateDirectory(_T("log/detail"),NULL);
		}
	}
	//CloseHandle(hFind);

	SetFilter();

	HWND hDlg;
	hDlg = CreateDialogParam(hInst, MAKEINTRESOURCE(IDD_DIALOG1), 0, DialogProc, 0);
	ShowWindow(hDlg, nCmdShow);

	BOOL ret;
	MSG msg;
	while((ret = GetMessage(&msg, 0, 0, 0)) != 0) {
	  if(ret == -1) /* error found */
		return -1;

	  if(!IsDialogMessage(hDlg, &msg)) {
		TranslateMessage(&msg); /* translate virtual-key messages */
		DispatchMessage(&msg); /* send it to dialog procedure */
	  }
	}

	return 0;
}

BOOL IsNeedProcess(string processName, string windowName, string parentWindowName)
{	
	if(g_pFilterType == FilterType::None || g_wFilterType == FilterType::None)
	{
		return FALSE;
	}

	if(g_pFilterType == FilterType::WhiteList)
	{
		bool flag = false;
		for(int i=0; i<g_processNameList.size(); i++)
		{
			int index = processName.find(g_processNameList[i]);
			if(index >= 0)
			{
				flag = true;
				break;
			}
		}

		if(!flag)
		{
			return FALSE;
		}
	}
	else if(g_pFilterType == FilterType::BlackList)
	{
		for(int i=0; i<g_processNameList.size(); i++)
		{
			int index = processName.find(g_processNameList[i]);
			if(index >= 0)
			{
				return FALSE;
			}
		}
	}

	//Specific setting for browser: IE, Chrome, Firefox
	string title = windowName;
	int index = processName.find("iexplore");
	if(index >= 0)
	{
		HWND ieHwnd = FindWindow(_T("IEFrame"), NULL);
		title = GetWindowNameStr(ieHwnd);
	}

	index = processName.find("chrome");
	if(index >= 0)
	{
		if(windowName == "Chrome Legacy Window")
		{
			title = parentWindowName;
		}
	}

	if(g_wFilterType == FilterType::WhiteList)
	{
		bool flag = false;
		for(int i=0; i<g_windowNameList.size(); i++)
		{
			int index = title.find(g_windowNameList[i]);
			if(index >= 0)
			{
				flag = true;
				break;
			}
		}

		if(!flag)
		{
			return FALSE;
		}
	}
	else if(g_wFilterType == FilterType::BlackList)
	{
		for(int i=0; i<g_windowNameList.size(); i++)
		{
			int index = title.find(g_windowNameList[i]);
			if(index >= 0)
			{
				printf("Black List!!!!\n");
				return FALSE;
			}
		}
	}
}

LRESULT CALLBACK ClipMonitorProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	//if(uMsg == WM_CLIPBOARDUPDATE)
	if(uMsg == WM_CHANGECBCHAIN ||  uMsg == WM_DRAWCLIPBOARD)
	{
		 if (!OpenClipboard(NULL))
		 {
			printf("Open Clip Board Error\n");
			return DefWindowProc(hwnd, uMsg, wParam, lParam);;
		 }
		 HANDLE hData = GetClipboardData(CF_TEXT);
		 if(hData == NULL)
		 {
			printf("Get Clip Board Data Error\n");
			CloseClipboard();
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		 }
		
		 char* pszText = static_cast<char*>( GlobalLock(hData) );
		 std::string text( pszText );
		 ReplaceAll(text,"\n","\\n");
		 ReplaceAll(text,"\t","\\t");

		 GlobalUnlock( hData );
		 CloseClipboard();

		 string timestamp = GetSysLocalTimeStr();
		 
		 HWND focusHwnd = GetForegroundWindow();
		 string windowname = GetWindowNameStr(focusHwnd);
		 string parentWindowName = GetNotNullParentNameStr(focusHwnd);
		 DWORD processId;
		 string processName = GetProcessNameStr(focusHwnd, &processId);

		 if(!IsNeedProcess(processName, windowname,parentWindowName))
		 {
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		 }

		 printf("copydata: %s, %s, %s\n", timestamp.c_str(), text.c_str(), windowname.c_str());
		 
		 FILE* fcopy;
		 errno_t err = _tfopen_s(&fcopy,_T("log/copypaste.txt"),_T("a"));
		 if(err == 0)
		 {
			fprintf_s(fcopy, "%s\n",timestamp.c_str());
			fprintf_s(fcopy, "%s\n",windowname.c_str());
			fprintf_s(fcopy, "%s\n",processName.c_str());
			fprintf_s(fcopy, "%s\n",parentWindowName.c_str());
			fprintf_s(fcopy, "%s\n\n", text.c_str());
			fclose(fcopy);
		 }
	}
	
	return DefWindowProc(hwnd, uMsg, wParam, lParam);;
}

void SetFilter()
{
	g_processNameList.clear();
	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile(_T("filter_config.txt"), &wfd);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		g_pFilterType = FilterType::None;
		g_wFilterType = FilterType::None;
		CloseHandle(hFind);
		return;
	}
	//CloseHandle(hFind);

	char temp[256];
	char type[256];
	FILE* fconfig = _tfopen(_T("filter_config.txt"), _T("r"));
	
	while(!feof(fconfig))
	{
		fscanf(fconfig,"%s %s", temp, type);
		if(strcmp(temp,"Process") == 0)
		{
			if(strcmp(type,"ALL") == 0)
			{
				g_pFilterType = FilterType::All;
			}
			else if(strcmp(type,"WHITE") == 0)
			{
				g_pFilterType = FilterType::WhiteList;
			}
			else if(strcmp(type,"BLACK") == 0)
			{
				g_pFilterType = FilterType::BlackList;
			}
			else
			{
				g_pFilterType = FilterType::None;
			}
		
			while(!feof(fconfig))
			{
				fscanf(fconfig,"%s", temp);
				if(strcmp(temp,"#") == 0)
				{
					break;
				}

				string pname = string(temp);
				g_processNameList.push_back(pname);
			}
		}
		else if(strcmp(temp,"Window") == 0)
		{
			if(strcmp(type,"ALL") == 0)
			{
				g_wFilterType = FilterType::All;
			}
			else if(strcmp(type,"WHITE") == 0)
			{
				g_wFilterType = FilterType::WhiteList;
			}
			else if(strcmp(type,"BLACK") == 0)
			{
				g_wFilterType = FilterType::BlackList;
			}
			else
			{
				g_wFilterType = FilterType::None;
			}
		
			while(!feof(fconfig))
			{
				fscanf(fconfig,"%s", temp);
				if(strcmp(temp,"#") == 0)
				{
					break;
				}

				string pname = string(temp);
				g_windowNameList.push_back(pname);
			}
		}
	}

	fclose(fconfig);

	SetFitlerForHook(g_pFilterType, g_processNameList, g_wFilterType, g_windowNameList);
	
}


INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDCANCEL:
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			return TRUE;
		case IDC_BUTTON1:
			TCHAR btnText[256];
			GetDlgItemText(hDlg,IDC_BUTTON1,btnText,256);
			
			HWND btnNextTC = GetDlgItem(hDlg,IDC_BUTTON2);
			HWND combo = GetDlgItem(hDlg,IDC_COMBO1);

			if(_tcscmp(btnText,_T("Start Logging")) == 0)
			{
				SetDlgItemText(hDlg,IDC_BUTTON1,_T("End Logging"));

				EnableWindow(btnNextTC,TRUE);
				EnableWindow(combo,TRUE);

				SetLowMouseHook();
				SetLowKeyboardHook();
				cbhwnd = SetClipboardViewer(global_hwnd);
			}
			else
			{
				SetDlgItemText(hDlg,IDC_BUTTON1,_T("Start Logging"));
				EnableWindow(btnNextTC,FALSE);
				EnableWindow(combo,FALSE);

				UnlockLowMouseHook();
				UnlockLowKeyboardHook();
				ChangeClipboardChain(global_hwnd, cbhwnd); 
			}

			return TRUE;
		}
		break;

	case WM_CLOSE:
		if(MessageBox(hDlg, TEXT("Close the program?"), TEXT("Close"),
			MB_ICONQUESTION | MB_YESNO) == IDYES)
		{
			DestroyWindow(hDlg);
		}
		return TRUE;

	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
	case WM_INITDIALOG:
		SetWindowPos(hDlg,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		RECT dlgRect;
		GetWindowRect(hDlg, &dlgRect);

		RECT taskbarRect;	
		HWND taskbar = FindWindow(_T("MSTaskListWClass"),NULL);
		int taskbarHeight = 0;
		if(taskbar && GetWindowRect(taskbar, &taskbarRect)) {
			taskbarHeight = taskbarRect.bottom - taskbarRect.top;
		}

		int x = screenWidth - (dlgRect.right - dlgRect.left);
		int y = screenHeight - taskbarHeight - (dlgRect.bottom - dlgRect.top);
		SetWindowPos(hDlg, NULL, x, y, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);

		//MessageBox(hDlg, _T("init the dialog?"), _T("init..."),NULL);
		HWND combo = GetDlgItem(hDlg,IDC_COMBO1);

		SendMessage(combo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(_T( "Pass" )));
		SendMessage(combo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(_T( "Fail" )));
		SendMessage(combo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(_T( "Others" )));
		SendMessage(combo, CB_SETCURSEL, 0, 0);

		return TRUE;
	}

  return FALSE;
}