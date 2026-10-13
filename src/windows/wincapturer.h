#pragma once
#include <Windows.h>

class WinCapturer {
public:
    HBITMAP capture()
    {
        //get primary screen dimension
        int x = 0, y = 0;
        int w = ::GetSystemMetrics(SM_CXSCREEN);
        int h = ::GetSystemMetrics(SM_CYSCREEN);

        //get the device context for the primary screen
        HDC hScreenDC = ::GetDC(nullptr);

        //create memory device context
        HDC hMemoryDC = ::CreateCompatibleDC(hScreenDC);

        //create Bitmap canvas
        HBITMAP hBitmap = ::CreateCompatibleBitmap(hScreenDC, w, h);

        //put the blank canvas into memory context
        //save the old canvas for later
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);

        //copy the pixels
        ::BitBlt(hMemoryDC, 0, 0, w, h, hScreenDC, x, y, SRCCOPY);

        //restore the old canvas
        SelectObject(hMemoryDC, hOldBitmap);

        //cleanup the memory device context
        DeleteDC(hMemoryDC);

        //release the device context
        ReleaseDC(nullptr, hScreenDC);

        return hBitmap;
    }
};