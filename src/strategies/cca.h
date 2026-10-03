#pragma once
#include <vector>
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

class CCA{
public:
    std::vector<std::pair<float, float>> operator()(HBITMAP&& image) {
        auto result = detect(HBITMAPToMat(image));
        ::DeleteObject(image);
        return result;
    }

private:
    static cv::Mat HBITMAPToMat(const HBITMAP& hBitmap)
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

    static std::vector<std::pair<float, float>> detect(const cv::Mat& image)
    {
        std::vector<std::pair<float, float>> result;

        cv::Mat original = image.clone();
        cv::Mat gray, binary;
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

        cv::adaptiveThreshold(gray, binary, 255,
            cv::ADAPTIVE_THRESH_GAUSSIAN_C,
            cv::THRESH_BINARY_INV, 3, 1);

        //if (debugMode)
            //SaveImage(gray, "c:\\temp\\1.bmp");

        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
        cv::morphologyEx(binary, binary, cv::MORPH_CLOSE, kernel);

        //if (debugMode)
            //SaveImage(binary, "c:\\temp\\2.bmp");

        cv::Mat labels, stats, centroids;
        int numLabels = cv::connectedComponentsWithStats(binary, labels, stats, centroids, 8, CV_32S);

        // Define how close (in pixels) two elements can be before we consider them the "same" object
        const float MIN_DISTANCE = 15.0f;

        for (int i = 1; i < numLabels; i++)
        {
            int x = stats.at<int>(i, cv::CC_STAT_LEFT);
            int y = stats.at<int>(i, cv::CC_STAT_TOP);
            int width = stats.at<int>(i, cv::CC_STAT_WIDTH);
            int height = stats.at<int>(i, cv::CC_STAT_HEIGHT);
            int area = stats.at<int>(i, cv::CC_STAT_AREA);

            double aspectRatio = (double)width / height;
            bool isNotTooThin = (aspectRatio > 0.5 && aspectRatio < 6.0);
            bool isRightSize = (area > 100 && area < 10000);

            if (isRightSize)
            {
                float centerX = centroids.at<double>(i, 0);
                float centerY = centroids.at<double>(i, 1);

                // Assume this new point is far away from everything
                bool isTooClose = false;

                // Compare it against every point we already saved
                for (const auto& savedPoint : result)
                {
                    // std::hypot calculates the 2D distance between two points
                    float distance = std::hypot(centerX - savedPoint.first, centerY - savedPoint.second);

                    if (distance < MIN_DISTANCE)
                    {
                        isTooClose = true;
                        break; // Stop checking, we already know it's too close
                    }
                }

                // Only save and draw if it passed the distance test
                if (!isTooClose)
                {
                    result.emplace_back(centerX, centerY);
                    cv::rectangle(original, cv::Rect(x, y, width, height), cv::Scalar(0, 0, 255, 255), 5);
                }
            }
        }

        //if (debugMode)
            //SaveImage(original, "c:\\temp\\3.bmp");

        return result;
    }
};