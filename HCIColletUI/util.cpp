//#include "stdafx.h"
#include "util.h"
#include <assert.h>

wstring WINAPI GetMouseEventNameWStr(WPARAM wParam)
{	
	wstring eventName = _T("");
	switch( wParam  )
	{
		case WM_MOUSEMOVE:
		case WM_NCMOUSEMOVE:
			eventName = _T("MOUSE MOVE");
			break;
		case WM_LBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
			eventName = _T("MOUSE LEFT DOWN");
			break;
		case WM_RBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
			eventName = _T("MOUSE RIGHT DOWN");
			break;
		case WM_LBUTTONUP:
		case WM_NCLBUTTONUP:
			eventName = _T("MOUSE LEFT UP");
			break;
		case WM_RBUTTONUP:
		case WM_NCRBUTTONUP:
			eventName = _T("MOUSE RIGHT UP");
			break;
		case WM_LBUTTONDBLCLK:
			eventName = _T("MOUSE DOUBLE CLICK");
			break;
		case WM_MOUSEWHEEL:
			eventName = _T("MOUSE WHEEL");
			break;
		default:
			eventName = _T("MOUSE UNKNOWN");
   }

	return eventName;
}

string WINAPI GetMouseEventNameStr(WPARAM wParam)
{
	return to_utf8(GetMouseEventNameWStr(wParam));
}

wstring WINAPI GetWindowNameWStr(HWND hwnd)
{
  int textlen = GetWindowTextLength(hwnd);
 
  if(textlen == 0)
  {
	return _T("");
  }

  TCHAR *temp = new TCHAR[textlen+1]; 
  GetWindowText(hwnd, temp, textlen+1);
  
  wstring name(temp);

  delete[] temp;

  return name;
}

string WINAPI GetWindowNameStr(HWND hwnd)
{
	return to_utf8(GetWindowNameWStr(hwnd));
}

wstring WINAPI GetWindowClassWStr(HWND hwnd)
{
	const int MAXLEN = 255;
	TCHAR temp[MAXLEN];
	GetClassName(hwnd, temp, MAXLEN);
	wstring className(temp);

	return className;
}

string WINAPI GetWindowClassStr(HWND hwnd)
{
	return to_utf8(GetWindowClassWStr(hwnd));
}

wstring WINAPI GetProcessNameWStr(HWND hwnd, DWORD *processId)
{
	//TCHAR processName[255];
	const int MAXLEN = 255;

	GetWindowThreadProcessId(hwnd,processId);
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
							PROCESS_VM_READ, FALSE, *processId);
	if(hProcess == NULL)
	{
		CloseHandle(hProcess);
		return _T("");
	}

	TCHAR temp[MAXLEN];
	int res = GetProcessImageFileName(hProcess,temp,MAXLEN);

	CloseHandle(hProcess);

	if(res <= 0 || !temp)
	{
		return _T("");
	}

	wstring str(temp);

	int index = str.find_last_of(_T("\\"));
	if(index >= 0)
	{
		return str.substr(index+1);
	}
	else
	{
		return str;
	}
}

string WINAPI GetProcessNameStr(HWND hwnd, DWORD *processId)
{
	wstring temp = GetProcessNameWStr(hwnd, processId);

	return to_utf8(temp);
}

string GetSysLocalTimeStr()
{
	SYSTEMTIME sys;
    GetLocalTime( &sys );

	char timestr[30];

	sprintf_s(timestr, "%4d-%02d-%02d-%02d-%02d-%02d-%03d", sys.wYear, sys.wMonth, sys.wDay,
		sys.wHour, sys.wMinute,sys.wSecond, sys.wMilliseconds);

	return string(timestr);
}

string GetSysLocalTimeStr(SYSTEMTIME sys)
{
	char timestr[256];

	sprintf_s(timestr, "%4d-%02d-%02d-%02d-%02d-%02d-%03d", sys.wYear, sys.wMonth, sys.wDay,
		sys.wHour, sys.wMinute,sys.wSecond, sys.wMilliseconds);

	return string(timestr);
}

wstring GetSysLocalTimeWStr()
{
	SYSTEMTIME sys;
    GetLocalTime( &sys );

	TCHAR timestr[30];

	wsprintf(timestr, _T("%4d-%02d-%02d-%02d-%02d-%02d-%03d"), sys.wYear, sys.wMonth, sys.wDay,
		sys.wHour, sys.wMinute,sys.wSecond, sys.wMilliseconds);

	return wstring(timestr);
}

string WINAPI GetTimeStr(time_t timer)
{
	tm ptm;
	errno_t err = localtime_s(&ptm,&timer);
	char buffer[32];
	// Format: Mo, 15.06.2009 20:20:00
	//strftime(buffer, 32, "%a, %d.%m.%Y %H:%M:%S", ptm); 
	if(err != 0)
		return "";

	strftime(buffer, 32, "%Y-%m-%d-%H-%M-%S", &ptm); 

	return string(buffer);
}

wstring WINAPI GetTimeWStr(time_t timer)
{
	wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
	string temp = GetTimeStr(timer);
	return converter.from_bytes(temp);
}

wstring WINAPI GetNotNullParentNameWStr(HWND hwnd)
{
	HWND parent = GetParent(hwnd);

	if(parent != NULL)
	{
		wstring name = GetWindowNameWStr(parent);
		if(name == _T(""))
		{
			return GetNotNullParentNameWStr(parent);
		}
		else
		{
			return name;
		}
	}
	else
	{
		return _T("");
	}
}

string WINAPI GetNotNullParentNameStr(HWND hwnd)
{
	HWND parent = GetParent(hwnd);

	if(parent != NULL)
	{
		string name = GetWindowNameStr(parent);
		if(name == "")
		{
			return GetNotNullParentNameStr(parent);
		}
		else
		{
			return name;
		}
	}
	else
	{
		return "";
	}
}

string to_utf8(const wchar_t* buffer, int len)
{
    assert(buffer != NULL);  
  
	int nLen = ::WideCharToMultiByte(CP_ACP, 0, buffer, -1, NULL, 0, NULL, NULL);  
	if (nLen == 0)   
	{  
		return "";  
	}  
	
		string newbuffer;
		newbuffer.resize(nLen) ;
	::WideCharToMultiByte(CP_ACP, 0, buffer, -1, const_cast< char* >(newbuffer.c_str()), nLen, NULL, NULL);  

    return newbuffer;
}

string to_utf8(const wstring& str)
{
       return to_utf8(str.c_str(), (int)str.size());
}

void ReplaceAll(string& str,string from,string to)
{
	string::size_type start_pos = 0;
	while((start_pos = str.find(from, start_pos)) != std::string::npos) {
         str.replace(start_pos, from.length(), to);
         start_pos += to.length(); // ...
	}
}