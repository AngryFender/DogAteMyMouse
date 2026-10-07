#pragma once
#include <cmath>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/geometry/2d.hpp>
#include <opencv2/objdetect.hpp>
#include <fstream>
#include <Windows.h>
#include <opencv2/core/hal/interface.h>

cv::Mat ImageToMat(const HBITMAP& hBitmap)
    {
        BITMAP bmp{};
        GetObject(hBitmap, sizeof(BITMAP), &bmp);

        BITMAPINFOHEADER bi;
        bi.biSize = sizeof(BITMAPINFOHEADER);
        bi.biWidth = bmp.bmWidth;
        bi.biHeight = -bmp.bmHeight;
        bi.biPlanes = 1;
        bi.biBitCount = 32;
        bi.biCompression = BI_RGB;
        bi.biSizeImage = 0;
        bi.biXPelsPerMeter = 0;
        bi.biYPelsPerMeter = 0;
        bi.biClrUsed = 0;
        bi.biClrImportant = 0;

        cv::Mat mat(bmp.bmHeight, bmp.bmWidth, CV_8UC4);

        HDC hdc = GetDC(NULL);

        GetDIBits(hdc, hBitmap, 0, bmp.bmHeight, mat.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

        ReleaseDC(NULL, hdc);

        return mat;
    }
