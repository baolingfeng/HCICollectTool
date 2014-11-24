#include "stdafx.h"
#include "windows.h"
#include "gdiplus.h"
using namespace Gdiplus;
using namespace Gdiplus::DllExports;

#pragma comment(lib, "gdiplus.lib")

int GetEncoderClsid(WCHAR *format, CLSID *pClsid)
{
        unsigned int num = 0,  size = 0;
        GetImageEncodersSize(&num, &size);
        if(size == 0) return -1;
        ImageCodecInfo *pImageCodecInfo = (ImageCodecInfo *)(malloc(size));
        if(pImageCodecInfo == NULL) return -1;
        GetImageEncoders(num, size, pImageCodecInfo);
        for(unsigned int j = 0; j < num; ++j)
        {
                if(wcscmp(pImageCodecInfo[j].MimeType, format) == 0){
                        *pClsid = pImageCodecInfo[j].Clsid;
                        free(pImageCodecInfo);
                        return j;
                }    
        }
        free(pImageCodecInfo);
        return -1;
}

int GetScreeny(HWND hMyWnd,LPWSTR lpszFilename, ULONG uQuality) // by Napalm
{
        ULONG_PTR gdiplusToken;
        GdiplusStartupInput gdiplusStartupInput;
        GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
        //HWND hMyWnd = GetDesktopWindow(); // get my own window
        RECT  r;             // the area we are going to capture 
        int w, h;            // the width and height of the area
        HDC dc;              // the container for the area
        int nBPP;
        HDC hdcCapture;
        LPBYTE lpCapture;
        int nCapture;
        int iRes;
        CLSID imageCLSID;
        Bitmap *pScreenShot;
        HGLOBAL hMem;
        int result;

        // get the area of my application's window      
        //GetClientRect(hMyWnd, &r);
        GetWindowRect(hMyWnd, &r);
        //dc = GetWindowDC(hMyWnd);//   GetDC(hMyWnd) ;
        dc = GetDC(NULL);
		
		if(r.left<0)
		{
			r.right = r.right + r.left;
			r.left = 0;
		}
		if(r.top<0)
		{
			r.bottom = r.bottom + r.top;
			r.top = 0;
		}
		
		w = r.right - r.left;
        h = r.bottom - r.top;
        nBPP = GetDeviceCaps(dc, BITSPIXEL);
        hdcCapture = CreateCompatibleDC(dc);


        // create the buffer for the screenshot
        BITMAPINFO bmiCapture = {
                  sizeof(BITMAPINFOHEADER), w, -h, 1, nBPP, BI_RGB, 0, 0, 0, 0, 0,
        };

        // create a container and take the screenshot
        HBITMAP hbmCapture = CreateDIBSection(dc, &bmiCapture,
                DIB_PAL_COLORS, (LPVOID *)&lpCapture, NULL, 0);

        // failed to take it
        if(!hbmCapture)
        {
                DeleteDC(hdcCapture);
                DeleteDC(dc);
                GdiplusShutdown(gdiplusToken);
                printf("failed to take the screenshot. err: %d\n", GetLastError());
                return 0;
        }

        // copy the screenshot buffer
        nCapture = SaveDC(hdcCapture);
        SelectObject(hdcCapture, hbmCapture);
		StretchBlt(hdcCapture, 0,0, w, h, dc, r.left, r.top,w,h, SRCCOPY);
		//BitBlt(hdcCapture, 0, 0, w, h, dc, 0, 0, SRCCOPY);
        RestoreDC(hdcCapture, nCapture);
        DeleteDC(hdcCapture);
        DeleteDC(dc);

        GpImage *bob;
        IStream *ssStr;

        // save the buffer to a file    
        pScreenShot = new Bitmap(hbmCapture, (HPALETTE)NULL);
        EncoderParameters encoderParams;
        encoderParams.Count = 1;
        encoderParams.Parameter[0].NumberOfValues = 1;
        encoderParams.Parameter[0].Guid  = EncoderQuality;
        encoderParams.Parameter[0].Type  = EncoderParameterValueTypeLong;
        encoderParams.Parameter[0].Value = &uQuality;
        GetEncoderClsid(L"image/png", &imageCLSID);
        iRes = (pScreenShot->Save(lpszFilename, &imageCLSID, &encoderParams) == Ok);

        delete pScreenShot;
        DeleteObject(hbmCapture);
        GdiplusShutdown(gdiplusToken);
        return iRes;

}

int GetScreeny2(HWND hMyWnd,LPWSTR lpszFilename, ULONG uQuality)
{
	ULONG_PTR gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	HDC hwindowDC,hwindowCompatibleDC;

    int srcheight,srcwidth;
    HBITMAP hbwindow;
   
    BITMAPINFOHEADER  bi;

    hwindowDC = GetDC(NULL); //screen HDC
    hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC,COLORONCOLOR);  

	srcwidth = GetSystemMetrics(SM_CXSCREEN);
	srcheight = GetSystemMetrics(SM_CYSCREEN);

	RECT rect;
	GetWindowRect(hMyWnd, &rect);
	int w = rect.right - rect.left;
    int h = rect.bottom - rect.top;

    // create a bitmap
    hbwindow = CreateCompatibleBitmap( hwindowDC, w, h);
    bi.biSize = sizeof(BITMAPINFOHEADER);   
    bi.biWidth = w;    
    bi.biHeight = -h;  //this is the line that makes it draw upside down or not
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
	StretchBlt( hwindowCompatibleDC, 0,0, w, h, hwindowDC, rect.left, rect.top,w,h, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
    //GetDIBits(hwindowCompatibleDC,hbwindow,0,rect.height,src.data,(BITMAPINFO *)&bi,DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

	CLSID imageCLSID;
    Bitmap *pScreenShot;

	pScreenShot = new Bitmap(hbwindow, (HPALETTE)NULL);
    EncoderParameters encoderParams;
    encoderParams.Count = 1;
    encoderParams.Parameter[0].NumberOfValues = 1;
    encoderParams.Parameter[0].Guid  = EncoderQuality;
    encoderParams.Parameter[0].Type  = EncoderParameterValueTypeLong;
    encoderParams.Parameter[0].Value = &uQuality;
    GetEncoderClsid(L"image/png", &imageCLSID);
    int iRes = (pScreenShot->Save(lpszFilename, &imageCLSID, &encoderParams) == Ok);

    delete pScreenShot;
    DeleteObject(hbwindow);
	ReleaseDC(NULL, hwindowDC);
	DeleteDC(hwindowCompatibleDC);

    GdiplusShutdown(gdiplusToken);

	return iRes;
}

int _tmain_1(int argc, _TCHAR* argv[])
{
	HWND hMyWnd = (HWND)0x002904B0;//GetDesktopWindow(); 
    GetScreeny(hMyWnd,L"test.png", 100);
    GetScreeny(GetDesktopWindow(),L"test2.png", 100);
	GetScreeny2(hMyWnd,L"test3.png", 100);
	return 0;
}