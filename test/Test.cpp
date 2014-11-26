// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <ctime>
#include <string>
#include <sstream>
#include <Oleacc.h>
#include <OleAuto.h>
#include <UIAutomation.h>
#include <UIAutomationClient.h>
#include <locale.h>
#include <fstream>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp" 

//#include "d3d9.h"
//#include "D3dx9tex.h"

#define _CRTDBG_MAP_ALLOC
#include <iostream>
#include <crtdbg.h>

#pragma comment (lib,"Ole32.lib")
#pragma comment (lib,"Psapi.lib")
#pragma comment (lib,"OleAcc.lib")

//#pragma comment (lib,"d3d9.lib")
//#pragma comment (lib,"d3dx9.lib")

#ifdef _DEBUG
#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

using namespace std;

//IDirect3DDevice9*	g_pd3dDevice;

string to_utf8(const wchar_t* buffer, int len)
{
	assert(buffer != NULL);  
  
	int nLen = ::WideCharToMultiByte(CP_ACP, 0, buffer, -1, NULL, 0, NULL, NULL);  
	if (nLen == 0)   
	{  
		return "";  
	}  
	
	string newbuffer;
	newbuffer.resize(nLen);
	::WideCharToMultiByte(CP_ACP, 0, buffer, -1, const_cast< char* >(newbuffer.c_str()), nLen, NULL, NULL);  

    return newbuffer;
}

string ws2s(const wstring& ws)
{
	string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";

	setlocale(LC_ALL, "C");

	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest,0,_Dsize);
	wcstombs(_Dest,_Source,_Dsize);
	string result = _Dest;
	delete []_Dest;

	setlocale(LC_ALL, curLocale.c_str());

	return result;
}

string to_utf8(const wstring& str)
{
       return to_utf8(str.c_str(), (int)str.size());
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

BOOL SaveBitmap(HDC hDC,HBITMAP hBitmap,char* szPath)
{
	OutputDebugString(L"Start SaveBitmap() ");
	DWORD error;
	FILE * fp= NULL;
	fp = fopen(szPath,"wb");
	if(fp == NULL)
	{
		OutputDebugString(L"Error Unable to Create File ");
		return false;
	}
	BITMAP Bm;
	BITMAPINFO BitInfo;
	ZeroMemory(&BitInfo, sizeof(BITMAPINFO));
	BitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BitInfo.bmiHeader.biBitCount = 0;

	if(!::GetDIBits(hDC, hBitmap, 0, 0, NULL, &BitInfo, DIB_RGB_COLORS))
	{
		OutputDebugString(L"Error GetDIBits Fail");

		return (false);
	}
	Bm.bmHeight = BitInfo.bmiHeader.biHeight;
	Bm.bmWidth  = BitInfo.bmiHeader.biWidth;

	BITMAPFILEHEADER    BmHdr;

	BmHdr.bfType        = 0x4d42;   // 'BM' WINDOWS_BITMAP_SIGNATURE
	BmHdr.bfSize        = (((3 * Bm.bmWidth + 3) & ~3) * Bm.bmHeight) 
		+ sizeof(BITMAPFILEHEADER) 
		+ sizeof(BITMAPINFOHEADER);
	BmHdr.bfReserved1    = BmHdr.bfReserved2 = 0;
	BmHdr.bfOffBits      = (DWORD) sizeof(BITMAPFILEHEADER) 
		+ sizeof(BITMAPINFOHEADER);

	BitInfo.bmiHeader.biCompression = 0;
	// Writing Bitmap File Header ////
	size_t size = fwrite(&BmHdr,sizeof(BITMAPFILEHEADER),1,fp);
	if(size < 1)
	{
		OutputDebugString(L"Error  Header Write");
		error = GetLastError();
	}
	size = fwrite(&BitInfo.bmiHeader,sizeof(BITMAPINFOHEADER),1,fp);
	if(size < 1)
	{		
		OutputDebugString(L"Error  Write");
		error = GetLastError();
	}
	BYTE *pData = new BYTE[BitInfo.bmiHeader.biSizeImage + 5];
	if(!::GetDIBits(hDC, hBitmap, 0, Bm.bmHeight, 
		pData, &BitInfo, DIB_RGB_COLORS))
		return (false);
	if(pData != NULL)
		fwrite(pData,1,BitInfo.bmiHeader.biSizeImage,fp);

	fclose(fp);
	delete (pData);

	return (true);
}

BOOL SavePNG(HDC hDC,HBITMAP hBitmap,char* szPath)
{
	OutputDebugString(L"Start SavePNG() ");
	cv::Mat img;
	DWORD error;
	
	//BITMAP Bm;
	
	BITMAPINFO BitInfo;
	ZeroMemory(&BitInfo, sizeof(BITMAPINFO));
	BitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BitInfo.bmiHeader.biBitCount = 0;

	if(!::GetDIBits(hDC, hBitmap, 0, 0, NULL, &BitInfo, DIB_RGB_COLORS))
	{
		OutputDebugString(L"Error GetDIBits Fail");

		return (false);
	}

	img.create(BitInfo.bmiHeader.biHeight,BitInfo.bmiHeader.biWidth,CV_8UC4);
	//Bm.bmHeight = BitInfo.bmiHeader.biHeight;
	//Bm.bmWidth  = BitInfo.bmiHeader.biWidth;
	int nchanel = img.channels();
	
	
	BitInfo.bmiHeader.biCompression = 0;
	// Writing Bitmap File Header ////
	
	//BYTE *pData = new BYTE[BitInfo.bmiHeader.biSizeImage + 5];
	if(!::GetDIBits(hDC, hBitmap, 0, BitInfo.bmiHeader.biHeight, 
		img.data, &BitInfo, DIB_RGB_COLORS))
		return (false);

	//delete(pData);

	cv::imwrite("screen.png",img);

	return (true);
}

cv::Mat GetScreenRect(cv::Rect rect)
{
	HDC hwindowDC,hwindowCompatibleDC;

    int srcheight,srcwidth;
    HBITMAP hbwindow;
    cv::Mat src;
    BITMAPINFOHEADER  bi;

    hwindowDC = GetDC(NULL); //screen HDC
    hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC,COLORONCOLOR);  

	srcwidth = GetSystemMetrics(SM_CXSCREEN);
	srcheight = GetSystemMetrics(SM_CYSCREEN);

    src.create(rect.height,rect.width,CV_8UC4);

    // create a bitmap
    hbwindow = CreateCompatibleBitmap( hwindowDC, rect.width, rect.height);
    bi.biSize = sizeof(BITMAPINFOHEADER);   
    bi.biWidth = rect.width;    
    bi.biHeight = -rect.height;  //this is the line that makes it draw upside down or not
    bi.biPlanes = 1;    
    bi.biBitCount = 32;    
    bi.biCompression = BI_RGB;    
    bi.biSizeImage = 0;  
    bi.biXPelsPerMeter = 0;    
    bi.biYPelsPerMeter = 0;    
    bi.biClrUsed = 0;    
    bi.biClrImportant = 0;


    // use the previously created device context with the bitmap
    SelectObject(hwindowCompatibleDC, hbwindow);
    // copy from the window device context to the bitmap device context
	//BitBlt(hwindowCompatibleDC,0,0, rect.width, rect.height, hwindowDC, rect.x, rect.y, SRCCOPY);
	StretchBlt( hwindowCompatibleDC, 0,0, rect.width, rect.height, hwindowDC, rect.x, rect.y,rect.width,rect.height, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
    
	SaveBitmap(hwindowCompatibleDC,hbwindow,"screen.bmp");
	
	SavePNG(hwindowCompatibleDC,hbwindow,"screen.png");
	//Bm.bmHeight = BitInfo.bmiHeader.biHeight;
	//Bm.bmWidth  = BitInfo.bmiHeader.biWidth;
	GetDIBits(hwindowCompatibleDC,hbwindow,0,rect.height,src.data,(BITMAPINFO *)&bi,DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

	DeleteObject(hbwindow);

	ReleaseDC(NULL, hwindowDC);
	DeleteDC(hwindowCompatibleDC);

	return src;
}

int UnicodeToANSI(char * pDes, const wchar_t * pSrc)  
{  
    assert(pDes != NULL);  
    assert(pSrc != NULL);  
  
    int nLen = ::WideCharToMultiByte(CP_ACP, 0, pSrc, -1, NULL, 0, NULL, NULL);  
    if (nLen == 0)   
    {  
        return -1;  
    }  
  
    return ::WideCharToMultiByte(CP_ACP, 0, pSrc, -1, pDes, nLen, NULL, NULL);  
} 

void trim(string& str)
{
  string::size_type pos = str.find_last_not_of(' ');
  if(pos != string::npos) {
    str.erase(pos + 1);
    pos = str.find_first_not_of(' ');
    if(pos != string::npos) str.erase(0, pos);
  }
  else str.erase(str.begin(), str.end());

  pos = str.find_last_not_of('\0');
  if(pos != string::npos) {
    str.erase(pos + 1);
    pos = str.find_first_not_of(' ');
    if(pos != string::npos) str.erase(0, pos);
  }
  else str.erase(str.begin(), str.end());
}

string WINAPI GetNotNullParentNameStr(HWND hwnd)
{
	HWND parent = GetParent(hwnd);

	if(parent != NULL)
	{
		string name = GetWindowNameStr(parent);
		trim(name);
		string temp("");
		if(name.empty())
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

double GetTimeDifference( SYSTEMTIME &st1, SYSTEMTIME &st2 )
{
    FILETIME        ft1, ft2;
    LARGE_INTEGER   li1, li2, liDiff;
    DWORD dwDiff;

    SystemTimeToFileTime( &st1, &ft1 );
    SystemTimeToFileTime( &st2, &ft2 );

	ULARGE_INTEGER ul1;
    ul1.LowPart = ft1.dwLowDateTime;
    ul1.HighPart = ft1.dwHighDateTime;

	ULARGE_INTEGER ul2;
    ul2.LowPart = ft2.dwLowDateTime;
    ul2.HighPart = ft2.dwHighDateTime;

	ULARGE_INTEGER uliRetValue;
    uliRetValue.QuadPart = 0;

	if(ul2.QuadPart >  ul1.QuadPart)
	{
		ul2.QuadPart -= ul1.QuadPart;
		uliRetValue = ul2;
		return (double)uliRetValue.QuadPart / 10 / 1000 / 1000;
	}
	else
	{
		ul1.QuadPart -= ul2.QuadPart;
		uliRetValue = ul1;
		return 0-(double)uliRetValue.QuadPart / 10 / 1000 / 1000;
	}

}

wstring WINAPI GetElementNameWStr(IUIAutomationElement* element)
{
	BSTR name = NULL;
	element->get_CachedName(&name);
	//element->get_CurrentName(&name);

	if(name)
	{
		wstring strName(name);
		SysFreeString(name);
		return strName;
	}
	else
	{
		return _T("");
	}
	
}

string WINAPI GetElementNameStr(IUIAutomationElement* element)
{
	return to_utf8(GetElementNameWStr(element));
}

/*
BOOL ScreenShot(LPDIRECT3DDEVICE9 lpDevice, HWND hWnd, TCHAR* fileName)
{
	IDirect3DSurface9* pSurface;

    lpDevice->CreateOffscreenPlainSurface(1280, 1024,
        D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &pSurface, NULL);
    lpDevice->GetFrontBufferData(0, pSurface);

    //D3DXSaveSurfaceToFile(_T("Desktop.png"),D3DXIFF_BMP,pSurface,NULL,NULL);
    pSurface->Release(); 

	return true;
}


HRESULT	InitD3D(HWND hWnd)
{
	D3DDISPLAYMODE	ddm;
	D3DPRESENT_PARAMETERS	d3dpp;

	IDirect3D9*	g_pD3D;
	IDirect3DSurface9*	g_pSurface;

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

	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING ,&d3dpp,&g_pd3dDevice)))
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
*/

cv::Mat addImageOnFront(cv::Point pt, cv::Mat& bg, cv::Mat fg, cv::Mat mask)
{
	//cv::Mat res;
	//bg.copyTo(res);

	cv::Mat roi = bg(cv::Rect(pt.x, pt.y, fg.cols, fg.rows));
	//cv::Mat copyRoi;
	//roi.copyTo(copyRoi);

	cv::Mat mask2;
	cv::bitwise_not(mask, mask2);

	cv::Mat m1, m2, dst;
	cv::bitwise_and(roi, roi, m1,  mask2);
	cv::bitwise_and(fg, fg, m2,  mask);
	cv::add(m1, m2, dst);

	dst.copyTo(roi);

	return fg;
}

void addimagewithmask()
{
	cv::Mat m = cv::imread( "../icons/1.png");
	cv::Mat mask = cv::imread( "../icons/1_msk.png",0);
	cv::Mat img = cv::imread("../HCICollectApp/log/screen/2014-11-01-14-57-41-015.png");

	addImageOnFront(cv::Point(100,100), img, m, mask);
	//cv::Mat m_gray, mask, mask_inv;
	//cv::cvtColor(m, m_gray, cv::COLOR_BGR2GRAY);
	//cv::threshold(m_gray,mask, 10,255,CV_THRESH_BINARY);
	//cv::bitwise_not(mask, mask_inv);
	//cv::Mat mask2 = mask > 128;

	//cv::Mat mask2;
	//cv::bitwise_not(mask, mask2);

	//cv::Mat mouse = cv::imread( "../icons/1_msk.png");
	//cv::Mat roi = img(cv::Rect(100, 100, m.cols, m.rows));
	//cv::imshow("roi", roi);

	//cv::Mat mask2;
	//cv::bitwise_not(mask, mask2);
	//cv::Mat m1, m2;
	//cv::bitwise_and(roi, mask, m1);
	//cv::bitwise_or(m1, mouse, m2);


	//cv::Mat m1, m2, dst;
	//cv::bitwise_and(roi, roi, m1,  mask2);
	//cv::bitwise_and(m, m, m2,  mask);
	//cv::add(m1, m2, dst);

	//cv::rectangle(img, cv::Rect(100, 100, m.cols, m.rows), cv::Scalar(0,255,0));
	
	//dst.copyTo(roi);
	//roi = dst;

	
	//cv::imshow("m1", m1);
	//cv::imshow("m2", m2);
	//cv::imshow("mask", mask2);
	//cv::imshow("dst", dst);
	//cv::imshow("img", img);
	//cv::waitKey(0);
}

int main0(int argc, _TCHAR* argv[])
{
	ifstream is("keyboard.txt");
	//is.imbue(std::locale());
	//is.imbue(std::locale("UTF8"));

	string line;
	getline(is, line);
	cout<<line<<endl;
	is.close();

	wstring wline;
	wifstream wis("keyboard.txt");
	wis.imbue(std::locale());
	getline(wis, wline);
	wis.close();
	

	//std::locale utf8_locale(std::locale(), new gel::stdx::utf8cvt<true>);
/*	
	HWND hwnd = (HWND)0x0005073E;

	wstring wwname = GetWindowNameWStr(hwnd);

	string wname = GetWindowNameStr(hwnd);
	string wname2 = ws2s(wwname);

	FILE* file;
	_tfopen_s(&file,_T("test.txt"),_T("a"));

	fprintf_s(file, "%s\n", wname.c_str());
	//fwprintf_s(file, _T("%s\n"), wwname.c_str());

	fclose(file);

	wprintf(_T("%s\n"), wwname.c_str());
	printf("%s\n", wname.c_str());
	*/
	//addimagewithmask();

	//LPDIRECT3DDEVICE9 lpDevice = Direct3DCreate9(D3D_SDK_VERSION);

	/*
	IUIAutomation *g_pAutomation=NULL;
	CoInitializeEx(NULL,COINIT_MULTITHREADED);
	HRESULT hr = CoCreateInstance(__uuidof(CUIAutomation), NULL,
		CLSCTX_INPROC_SERVER, __uuidof(IUIAutomation), (void**)&g_pAutomation);

	IUIAutomationCacheRequest* cacheRequest;
	g_pAutomation->CreateCacheRequest(&cacheRequest);
	cacheRequest->AddPattern(UIA_ValuePatternId);
	cacheRequest->AddProperty(UIA_LocalizedControlTypePropertyId);
	cacheRequest->AddProperty(UIA_RuntimeIdPropertyId);
	cacheRequest->AddProperty(UIA_NamePropertyId);
	cacheRequest->AddProperty(UIA_BoundingRectanglePropertyId);

	HWND hwnd = (HWND)0x00060922;

	IUIAutomationElement* element = NULL;
	g_pAutomation->ElementFromHandleBuildCache(hwnd,cacheRequest,&element);

	string name = GetElementNameStr(element);
	
	SAFEARRAY* rumtimeId;
	element->GetRuntimeId(&rumtimeId);

	element->Release();
	cacheRequest->Release();
	*/
	//HWND hwnd = (HWND)0x405b0;
	//string name = GetNotNullParentNameStr(hwnd);
	/*
	TCHAR temp2[] = _T("中文");
	char dst[256];
	UnicodeToANSI(dst,temp2);
	string s1(dst);

	FILE* file = _tfopen(_T("test.txt"),_T("r"));
	char temp[256];
	fscanf(file, "%s", temp);
	string s2(temp);

	int index = s2.find(s1);

	fclose(file);
	*/
	
	/*
	HWND hwnd = (HWND)0x00DF0642;

	wstring wwname = GetWindowNameWStr(hwnd);

	string wname = GetWindowNameStr(hwnd);
	
	wprintf(_T("%s\n"), wwname.c_str());
	printf("%s\n", wname.c_str());

	int index = wwname.find(_T("文本"));

	index = wname.find("文本");
	*/
	
	
	//time_t timer1;
	//time(&timer1);

	
	/*
	for(int i=0; i<10; i++)
	{
		cv::Mat cap = GetScreenRect(cv::Rect(0,0,1024,1280));
		stringstream ss;
		ss<<i<<".png";
		string name = ss.str();
		imwrite(name,cap);
		cap.release();
	}
	*/

	//time_t timer2;
	//time(&timer2);

	//double temp = difftime(timer1,timer2);

	//printf("%f", temp);
	

	return 0;
}

int main_0()
{
	//char* regHookTime = "reg add \"HKEY_CURRENT_USER\Control Panel\Desktop\" /v LowLevelHooksTimeout /t reg_dword /d 888888 /f";
	int res = system("reg add \"HKEY_CURRENT_USER\\Control Panel\\Desktop\" /v LowLevelHooksTimeout /t reg_dword /d 888888 /f");
	cout<<res<<endl;
	if(res < 0)
	{
		cout<<"Reg Hook Time Fail!"<<endl;
	}

	return 1;
}
