// HCICollectDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "util.h"
#include "HCICollectDll.h"
#include <vector>

#pragma comment (lib,"Ole32.lib")
#pragma comment (lib,"Psapi.lib")
#pragma comment (lib,"OleAcc.lib")

//#pragma comment (lib,"d3d9.lib")
//#pragma comment (lib,"d3dx9.lib")

#pragma comment(lib, "gdiplus.lib")

/*--------------Gobal Variable-----------------------*/
HHOOK g_hLLKeyBoardHook = NULL;
HHOOK g_hLLMouseHook = NULL;
FILE* g_fMouseFile = NULL;
FILE* g_fAccTimeOut = NULL;

IUIAutomation *g_pAutomation=NULL;
IUIAutomationTreeWalker* g_pControlWalker = NULL;
CRITICAL_SECTION  _critical;

DWORD preProcessId = -1;
HWND preWindow = NULL;
HWND preLeftDownWindow = NULL;
HWND preWheelWindow = NULL;

SYSTEMTIME preKeyTime;
SYSTEMTIME preMouseMoveTime;
SYSTEMTIME preLClickTime;
SYSTEMTIME preMouseWheelTime;

RECT SCREEN_RECT;
//int SCREEN_WIDTH;
//int SCREEN_HEIGHT;

FilterType g_pFilterType = FilterType::None;
FilterType g_wFilterType = FilterType::None;

std::vector<string> processNameList;
std::vector<string> windowNameList;

vector<HANDLE> threadList;

BOOL DirectX_Capture = TRUE;
//IDirect3DDevice9* g_pd3dDevice;

/*--------------Gobal Variable-----------------------*/

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

	InitializeCriticalSection(& _critical);

	CoInitializeEx(NULL,COINIT_MULTITHREADED);
	HRESULT hr = CoCreateInstance(__uuidof(CUIAutomation), NULL,
		CLSCTX_INPROC_SERVER, __uuidof(IUIAutomation), (void**)&g_pAutomation);

	g_pAutomation->get_ContentViewWalker(&g_pControlWalker);

	SCREEN_RECT.left = 0;
	SCREEN_RECT.top = 0;
	SCREEN_RECT.right = GetSystemMetrics(SM_CXSCREEN);
	SCREEN_RECT.bottom = GetSystemMetrics(SM_CYSCREEN);
	//SCREEN_WIDTH =  GetSystemMetrics(SM_CXSCREEN);
	//SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN);

	CoUninitialize();
	//CreateThread(NULL, 0, ReleaseThreadFunction, NULL, 0, NULL);  

	/*
	if(!g_pd3dDevice)
	{
		if(FAILED(InitD3D(&g_pd3dDevice,&SCREEN_WIDTH,&SCREEN_HEIGHT,NULL)))
		{
			DirectX_Capture = FALSE;
		}
	}
	*/

	return TRUE;
}

DWORD WINAPI ReleaseThreadFunction( LPVOID lpParam )
{
	while(1)
	{
		Sleep(1000);
		for(int i=0; i<threadList.size(); i++)
		{
			WaitForSingleObject(threadList[i],INFINITE);

			CloseHandle(threadList[i]);
		}
	}

	return 0;
}

HMODULE WINAPI ModuleFromAddress(PVOID pv) 
{
	MEMORY_BASIC_INFORMATION mbi;
	if (::VirtualQuery(pv, &mbi, sizeof(mbi)) != 0)
	{
		return (HMODULE)mbi.AllocationBase;
	}
	else
	{
		return NULL;
	}
}

BOOL WINAPI SetFitlerForHook(FilterType &pFilterType_in, std::vector<std::string> &processNameList_in,
	FilterType &wFilterType_in, std::vector<std::string> &windowNameList_in)
{
	g_pFilterType = pFilterType_in;
	processNameList = processNameList_in;
	g_wFilterType = wFilterType_in;
	windowNameList = windowNameList_in;

	return TRUE;
}

BOOL WINAPI SetLowKeyboardHook()
{
	g_hLLKeyBoardHook = SetWindowsHookEx(
                  WH_KEYBOARD_LL,      /* Type of hook */
                  LLKeyboardHookProc,    /* Hook process */
                  ModuleFromAddress(LLKeyboardHookProc),//hInstance,        /* Instance */
                  NULL);

	return g_hLLKeyBoardHook != NULL;
}

BOOL WINAPI SetLowMouseHook()
{
	g_hLLMouseHook = SetWindowsHookEx(
                  WH_MOUSE_LL,      /* Type of hook */
                  LLMouseHookProc,    /* Hook process */
                  ModuleFromAddress(LLMouseHookProc),//hInstance,        /* Instance */
                  NULL);

	return g_hLLMouseHook != NULL;
}

BOOL WINAPI UnlockLowKeyboardHook()
{
	BOOL bOk = UnhookWindowsHookEx(g_hLLKeyBoardHook);
	g_hLLKeyBoardHook = NULL;

	return bOk;
}

BOOL WINAPI UnlockLowMouseHook()
{
	BOOL bOk = UnhookWindowsHookEx(g_hLLMouseHook);
	g_hLLMouseHook = NULL;

	return bOk;
}

BOOL IsNeedProcess(std::string processName, std::string windowName, std::string parentWindowName)
{	
	if(g_pFilterType == FilterType::None || g_wFilterType == FilterType::None)
	{
		return FALSE;
	}

	if(g_pFilterType == FilterType::WhiteList)
	{
		bool flag = false;
		for(int i=0; i<processNameList.size(); i++)
		{
			int index = processName.find(processNameList[i]);
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
		for(int i=0; i<processNameList.size(); i++)
		{
			int index = processName.find(processNameList[i]);
			if(index >= 0)
			{
				return FALSE;
			}
		}
	}

	//Specific setting for browser: IE, Chrome, Firefox
	std::string title = windowName;
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
		for(int i=0; i<windowNameList.size(); i++)
		{
			int index = title.find(windowNameList[i]);
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
		for(int i=0; i<windowNameList.size(); i++)
		{
			int index = title.find(windowNameList[i]);
			if(index >= 0)
			{
				//printf("Black List!!!!\n");
				return FALSE;
			}
		}
	}
}

LRESULT CALLBACK LLKeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0 || nCode == HC_NOREMOVE)
		return ::CallNextHookEx(NULL, nCode, wParam, lParam);
	
   if (lParam & 0x40000000)	// Check the previous key state
	{
		return ::CallNextHookEx(NULL, nCode, wParam, lParam);
	}

	if(wParam == WM_KEYDOWN)
	{
		//time_t timer;
		//time(&timer);

		KBDLLHOOKSTRUCT  *pkbhs = (KBDLLHOOKSTRUCT *)lParam;

		//check that the message is from keyboard or is synthesized by SendInput API
		if((pkbhs->flags & LLKHF_INJECTED))
			return ::CallNextHookEx(NULL, nCode, wParam, lParam);
		
		SYSTEMTIME sys;
		GetLocalTime( &sys );
		std::string strTime = GetSysLocalTimeStr(sys);

		HWND hwnd = GetForegroundWindow();
		std::string windowname = GetWindowNameStr(hwnd);
		ReplaceAll(windowname,"\n","\\n");
		ReplaceAll(windowname,"\t","\\t");

		DWORD processId;
		std::string processName = GetProcessNameStr(hwnd,&processId);

		std::string parentWindowName = GetNotNullParentNameStr(hwnd);

		FILE* fKeyBoardLog;
		errno_t err = _tfopen_s(&fKeyBoardLog,_T("log/keyboard.txt"),_T("a"));

		if(!IsNeedProcess(processName, windowname,parentWindowName))
		{
			fprintf_s(fKeyBoardLog,"%s\n", strTime.c_str());
			fprintf_s(fKeyBoardLog,"NOT LOGGED APPLICATION\n");
			fclose(fKeyBoardLog);
			return CallNextHookEx(NULL, nCode, wParam, lParam);
		}

		DWORD dwMsg = 1;
		dwMsg += pkbhs->scanCode << 16;
		dwMsg += pkbhs->flags << 24;

		CHAR strKey[80];
		GetKeyNameTextA(dwMsg,strKey,80);

		
		POINT point;
		GUITHREADINFO pg;
		pg.cbSize=48;
		::GetGUIThreadInfo(NULL,&pg);
		//HWND temphwnd=pg.hwndCaret;
		if (pg.hwndCaret)
		{
			point.x=pg.rcCaret.right;
			point.y=pg.rcCaret.bottom;
			::ClientToScreen(pg.hwndCaret,&point);
		}
		else
		{
			point.x = point.y = -1;
		}
		

		//FILE* fKeyBoardLog;
		//errno_t err = _tfopen_s(&fKeyBoardLog,_T("log/keyboard.txt"),_T("a"));
		if(err == 0)
		{
			printf_s("%s %s %s\n", windowname.c_str(), processName.c_str(), strKey);
			fprintf_s(fKeyBoardLog, "%s\n", strTime.c_str());
			fprintf_s(fKeyBoardLog, "%s\n", strKey);
			fprintf_s(fKeyBoardLog, "%d %d\n", point.x, point.y);
			fprintf_s(fKeyBoardLog, "%s\n", windowname.c_str());
			fprintf_s(fKeyBoardLog, "%s\n", processName.c_str());
			fprintf_s(fKeyBoardLog, "%s\n", parentWindowName.c_str());
			fclose(fKeyBoardLog);
		}
		else
		{
			printf_s("Open Key Log File Error\n");
		}

		//double interval =  difftime(timer,preKeyTimer);
		double interval = GetTimeDifference(preKeyTime,sys);	
		if(interval<0 || interval > 1)
		{
			preKeyTime = sys;
			/*
			RECT winRect;
			GetWindowRect(hwnd,&winRect);
			if(winRect.left<0)
			{
				winRect.right = winRect.right + winRect.left;
				winRect.left = 0;
			}
			if(winRect.top<0)
			{
				winRect.bottom = winRect.bottom + winRect.top;
				winRect.top = 0;
			}
			*/
			
			std::string img = "log/screen/" + strTime +  ".png";
			GetScreeny(SCREEN_RECT,from_string(img).c_str(),100);
		}
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK LLMouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode < 0)
	{ 
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}
	
	//time_t timer;
	//time(&timer);

	PMSLLHOOKSTRUCT pHookStruct = (PMSLLHOOKSTRUCT) lParam;
	POINT point = pHookStruct->pt;
	
	HWND hwnd = WindowFromPoint(point);
	if(hwnd == NULL)
	{
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}

	std::string windowName = GetWindowNameStr(hwnd);
	ReplaceAll(windowName,"\n","\\n");
	ReplaceAll(windowName,"\t","\\t");

	DWORD processId;
	std::string processName = GetProcessNameStr(hwnd, &processId);

	std::string parentWindowName = GetNotNullParentNameStr(hwnd);
	ReplaceAll(parentWindowName,"\n","\\n");
	ReplaceAll(parentWindowName,"\t","\\t");

	if(g_fMouseFile == NULL)
	{
		errno_t err = _tfopen_s(&g_fMouseFile,_T("log/mouse.txt"),_T("a"));
		if(err != 0)
		{
			printf_s("Open Mouse File Error\n");
			return 1;
		}
		_tfopen_s(&g_fAccTimeOut,_T("log/acc_timeout.txt"),_T("a"));
	}

	SYSTEMTIME sys;
	GetLocalTime( &sys );
	std::string strTime = GetSysLocalTimeStr(sys);

	if(wParam == WM_MOUSEMOVE)
	{
		double interval = GetTimeDifference(preMouseMoveTime,sys);	
		if(processId == preProcessId && hwnd == preWindow && interval<0.25)
		{
			//printf_s("MOUSE MOVE NOT LOGGED %d %f %d\n", timer, interval,preMouseMoveTimer);
			preMouseMoveTime = sys;
			return CallNextHookEx(NULL, nCode, wParam, lParam);
		}
		preMouseMoveTime = sys;
	}

	if(!IsNeedProcess(processName, windowName,parentWindowName))
	{
		printf_s("NOT Logged application %d %d\n", processId,preProcessId);
		if(processId != preProcessId || hwnd != preWindow)
		{
			preProcessId = processId;
			preWindow = hwnd;
			fprintf_s(g_fMouseFile,"%s\n", strTime.c_str());
			fprintf_s(g_fMouseFile,"NOT LOGGED APPLICATION\n");
			fflush(g_fMouseFile);
		}
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}

	
	RECT winRect;
	GetWindowRect(hwnd,&winRect);
	if(winRect.left<0)
	{
		winRect.right = winRect.right + winRect.left;
		winRect.left = 0;
	}
	if(winRect.top<0)
	{
		winRect.bottom = winRect.bottom + winRect.top;
		winRect.top = 0;
	}

	std::string msgName = GetMouseEventNameStr(wParam);

	fprintf_s(g_fMouseFile,"%s\n", strTime.c_str());
	fprintf_s(g_fMouseFile,"%s %d %d\n", msgName.c_str(), point.x, point.y);
	fprintf_s(g_fMouseFile,"%x,%s (%d %d %d %d)\n", (int)hwnd, windowName.c_str(), winRect.left, winRect.top,winRect.right,winRect.bottom);
	fprintf_s(g_fMouseFile,"%s %d\n", processName.c_str(), processId);
	fprintf_s(g_fMouseFile,"%s\n", parentWindowName.c_str());
	fflush(g_fMouseFile);
	
	if(wParam == WM_LBUTTONDOWN)
	{
		double interval = GetTimeDifference(preLClickTime,sys);
		printf_s("Window: %s, %s, %f\r\n", msgName.c_str(),windowName.c_str(), interval);
		
		if(interval > 0.5 || preLeftDownWindow != hwnd)
		{
			std::string img = "log/screen/" + strTime +  ".png";
			GetScreeny(SCREEN_RECT,from_string(img).c_str(),100);

			ParamData p;
			p.pt = point;
			p.sys = sys;

			HANDLE thread = CreateThread(NULL, 0, AccessUIThreadFunction, (LPVOID)&p, 0, NULL);  
			//threadList.push_back(thread);
				
			if(WAIT_TIMEOUT == WaitForSingleObject(thread,500))
			{
				fprintf_s(g_fAccTimeOut,"%s UIAutomation time out\n", strTime.c_str());
				fflush(g_fAccTimeOut);
				printf_s("thread time out\n");
			}
			
			CloseHandle(thread);
		}
		//preLClickTimer = timer;
		preLClickTime = sys;
		preLeftDownWindow = hwnd;

	}
	else if(wParam == WM_MOUSEWHEEL)
	{
		//double interval =  difftime(timer,preMouseWheelTimer);
		double interval = GetTimeDifference(preMouseWheelTime,sys);
		if(interval > 1 || preWheelWindow !=  hwnd)
		{
			std::string img = "log/screen/" + strTime +  ".png";
			GetScreeny(SCREEN_RECT,from_string(img).c_str(),100);
		}
		//preMouseWheelTimer = timer;
		preMouseWheelTime = sys;
		preWheelWindow = hwnd;
	}

	preProcessId = processId;
	preWindow = hwnd;

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

DWORD WINAPI AccessUIThreadFunction( LPVOID lpParam )
{
	try
    { 
	HRESULT hr;
	CoInitializeEx(NULL,COINIT_MULTITHREADED);

	PParamData data = (PParamData)lpParam;

	//WPARAM wParam = data->wParam;
	//LPARAM lParam = data->lParam;
	//time_t timer = data->timer;
	SYSTEMTIME sys = data->sys;
	//string processName = data->processName;
	//string windowName = data->windowName;
	POINT point = data->pt;
	string strTime =  GetSysLocalTimeStr(sys); 

	//EnterCriticalSection(&_critical);
	
	/*
	IUIAutomationCacheRequest* cacheRequest;
	g_pAutomation->CreateCacheRequest(&cacheRequest);
	cacheRequest->AddPattern(UIA_ValuePatternId);
	cacheRequest->AddProperty(UIA_LocalizedControlTypePropertyId);
	cacheRequest->AddProperty(UIA_RuntimeIdPropertyId);
	cacheRequest->AddProperty(UIA_NamePropertyId);
	cacheRequest->AddProperty(UIA_BoundingRectanglePropertyId);
	*/

	IUIAutomationElement* element = NULL;

	//hr = g_pAutomation->ElementFromPointBuildCache(point,cacheRequest,&element);
	hr = g_pAutomation->ElementFromPoint(point, &element);
	//printf_s("bad_alloc 3\n"); 

	if(element == NULL || S_OK != hr)
	{
		printf_s("Cann't get Element\n");
		
		//if(cacheRequest != NULL)
		//{
		//	cacheRequest->Release();
		//}
		
		if(element != NULL)
		{
			element->Release();
		}
		CoUninitialize();

		//LeaveCriticalSection(& _critical);
		return 1;
	}

	std::string elementDesc = GetElementDescStr(element);
	std::string elementName = GetElementNameStr(element);
	std::string elementValue = GetElementValueStr(element);

	string pname, ptype;
	GetElementParentNameWStr(g_pControlWalker,element, pname, ptype);

	ReplaceAll(elementValue,"\n","\\n");
	ReplaceAll(elementValue,"\t","\\t");
	//TODO: Parent name when name is empty, but not success

	SAFEARRAY* rumtimeId;
	element->GetRuntimeId(&rumtimeId);

	RECT bounding;
	//element->get_CachedBoundingRectangle(&bounding);
	element->get_CurrentBoundingRectangle(&bounding);
	std::string runtimeId = GetRuntimeIDStr(rumtimeId);
		
	//printf_s("bad_alloc 4\n"); 

	//LeaveCriticalSection(& _critical);
	//fwprintf_s(fMouseLog,_T("%s %d\n"), timestamp.c_str(), timer);
	//fwprintf_s(fMouseLog, _T("UIAutomation: %s, %s\n\n"),elementDesc.c_str(),elementName.c_str());	

	FILE* file;
	std::string filename = "log/detail/" + strTime + ".txt";
	std::wstring temp(filename.length(), L'#');
	mbstowcs(&temp[0],filename.c_str(),filename.length());
	_tfopen_s(&file,temp.c_str(),_T("w"));
	temp.clear();

	fprintf_s(file, "%s %d\n", strTime.c_str()); //timestamp
	fprintf_s(file, "%s\n", runtimeId.c_str()); //runtime id
	//fprintf_s(file, "Window: %s\n", windowName.c_str());
	fprintf_s(file, "%s\n", elementDesc.c_str()); //control type
	fprintf_s(file, "%s\n", elementName.c_str()); //contorl name
	fprintf_s(file, "%d %d %d %d\n", bounding.left, bounding.top, bounding.right, bounding.bottom); //bounding
	fprintf_s(file, "%s\n", pname.c_str()); //parent name
	fprintf_s(file, "%s\n", ptype.c_str()); //parent type
	fprintf_s(file, "%s", elementValue.c_str()); //control value

	fclose(file);

	//if(cacheRequest != NULL)
	//{
	//	cacheRequest->Release();	
	//}
		
	if(element != NULL)
	{
		element->Release();
	}

	SafeArrayDestroy(rumtimeId);

	CoUninitialize();
	}
	catch(std::exception &e)
	{
		printf_s("exception: %s\n", e.what()); 
	}
	return 0; 
}

DWORD WINAPI CaptureScreenThreadFunction( LPVOID lpParam )
{
	/*
	PParamData data = (PParamData)lpParam;
	SYSTEMTIME sys = data->systime;
	cv::Rect rect= data->rect;

	if(sys.wMonth > 12 || sys.wMonth < 0)
	{
		return -1;
	}

	string strTime = GetSysLocalTimeStr(sys);

	//int screenWidth =  GetSystemMetrics(SM_CXSCREEN);
	//int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	cv::Mat cap = GetScreenRect(rect); //cv::Rect(0,0,screenWidth,screenHeight));
	string img = "log/screen/" + strTime +  ".png";
	int ret = cv::imwrite(img,cap);

	return ret; 
	*/
	return 0;
}

/*
HRESULT	InitD3D(IDirect3DDevice9** g_pd3dDevice,int* screenW,int* screenH,HWND hWnd)
{
	D3DDISPLAYMODE	ddm;
	D3DPRESENT_PARAMETERS	d3dpp;

	IDirect3D9*	g_pD3D;
	//IDirect3DSurface9*	g_pSurface;

	if((g_pD3D=Direct3DCreate9(D3D_SDK_VERSION))==NULL)
	{
		printf("Unable to Create Direct3D ");
		return E_FAIL;
	}

	if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&ddm)))
	{
		printf("Unable to Get Adapter Display Mode");
		return E_FAIL;
	}

	ZeroMemory(&d3dpp,sizeof(D3DPRESENT_PARAMETERS));

	*screenW = ddm.Width;
	*screenH = ddm.Height;

	d3dpp.Windowed=TRUE;
	d3dpp.Flags=D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	d3dpp.BackBufferFormat=ddm.Format;
	d3dpp.BackBufferHeight =ddm.Height;
	d3dpp.BackBufferWidth =ddm.Width;
	d3dpp.MultiSampleType=D3DMULTISAMPLE_NONE;
	d3dpp.SwapEffect=D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow=hWnd;
	d3dpp.PresentationInterval=D3DPRESENT_INTERVAL_DEFAULT;
	d3dpp.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT;

	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING ,&d3dpp,g_pd3dDevice)))
	{
		printf("Unable to Create Device");
		return E_FAIL;
	}
	

	if(FAILED(g_pd3dDevice->CreateOffscreenPlainSurface(ddm.Width, ddm.Height, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &g_pSurface, NULL)))
	{
		printf("Unable to Create Surface");
		return E_FAIL;
	}
	

	return S_OK;
}

BOOL ScreenShot(LPDIRECT3DDEVICE9 lpDevice, std::wstring fileName,RECT rect)
{	
	//wprintf_s(_T("%s\n"),fileName);
	IDirect3DSurface9* pSurface;

    if(FAILED(lpDevice->CreateOffscreenPlainSurface(SCREEN_WIDTH, SCREEN_HEIGHT,
        D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &pSurface, NULL)))
	{
		printf("screenshot error: surface create error\n");
	}
    if(FAILED(lpDevice->GetFrontBufferData(0, pSurface)))
	{
		printf("screenshot error: can't get buffer data\n");
	}

	if(rect.left<0)
	{
		rect.right = rect.left + rect.right;
		rect.left = 0;
	}

	if(rect.top<0)
	{
		rect.bottom = rect.top + rect.bottom;
		rect.top = 0;
	}

    D3DXSaveSurfaceToFile(fileName.c_str(),D3DXIFF_BMP,pSurface,NULL,&rect);
    pSurface->Release(); 

	return true;
}
*/