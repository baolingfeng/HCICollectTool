#include "stdafx.h"
#include<Psapi.h>
#include<string>
#include<ctime>
#include <codecvt>
#include <Oleacc.h>
#include <OleAuto.h>
#include <UIAutomation.h>
#include <UIAutomationClient.h>

//#include "opencv2/core/core.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp" 


#include <GdiPlus.h>

using namespace std;



std::string __declspec(dllexport) WINAPI GetTimeStr(time_t timer);
std::wstring __declspec(dllexport) WINAPI GetTimeWStr(time_t timer);

std::wstring __declspec(dllexport) WINAPI GetWindowNameWStr(HWND hwnd);
std::string __declspec(dllexport) WINAPI GetWindowNameStr(HWND hwnd);

std::wstring __declspec(dllexport) WINAPI GetWindowClassWStr(HWND hwnd);
std::string __declspec(dllexport) WINAPI GetWindowClassStr(HWND hwnd);

std::wstring __declspec(dllexport) WINAPI GetProcessNameWStr(HWND hwnd, DWORD *processId);
std::string __declspec(dllexport) WINAPI GetProcessNameStr(HWND hwnd, DWORD *processId);

std::wstring __declspec(dllexport) WINAPI GetMouseEventNameWStr(WPARAM wParam);
std::string __declspec(dllexport) WINAPI GetMouseEventNameStr(WPARAM wParam);

std::wstring __declspec(dllexport) WINAPI GetNotNullParentNameWStr(HWND hwnd);
std::string __declspec(dllexport) WINAPI GetNotNullParentNameStr(HWND hwnd);

std::wstring __declspec(dllexport) GetSysLocalTimeWStr();
std::string __declspec(dllexport) GetSysLocalTimeStr();

std::string __declspec(dllexport) GetSysLocalTimeStr(SYSTEMTIME sys);

std::string to_utf8(const wchar_t* buffer, int len);
std::string to_utf8(const std::wstring& str);

std::wstring from_string(const std::string& str);

//cv::Mat GetScreenRect(cv::Rect rect);

void ReplaceAll(std::string& str, std::string from,std::string to);

std::wstring __declspec(dllexport) WINAPI GetElementDescWStr(IUIAutomationElement* element);
std::string __declspec(dllexport) WINAPI GetElementDescStr(IUIAutomationElement* element);

std::wstring __declspec(dllexport) WINAPI GetElementNameWStr(IUIAutomationElement* element);
std::string __declspec(dllexport) WINAPI GetElementNameStr(IUIAutomationElement* element);

std::wstring __declspec(dllexport) WINAPI GetElementValueWStr(IUIAutomationElement* element);
std::string __declspec(dllexport) WINAPI GetElementValueStr(IUIAutomationElement* element);

std::wstring __declspec(dllexport) WINAPI GetRuntimeIDWStr(SAFEARRAY* runtimeId);
std::string __declspec(dllexport) WINAPI GetRuntimeIDStr(SAFEARRAY* runtimeId);

void __declspec(dllexport) WINAPI GetElementParentNameWStr(IUIAutomationTreeWalker* walker, IUIAutomationElement* element, string& pname, string& ptype);
//std::string __declspec(dllexport) WINAPI GetElementParentNameStr(IUIAutomationTreeWalker* walker,IUIAutomationElement* element);


double GetTimeDifference( SYSTEMTIME &st1, SYSTEMTIME &st2 );
void trim(std::string& str);

 int GetScreeny(RECT rect,LPCWSTR lpszFilename, ULONG uQuality);