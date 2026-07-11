#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <d3d11.h>
#include <tchar.h>
#include <Windows.h>
#include <dwmapi.h>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include <fstream>

//hide the console window
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup" )

//allocate memory and create resources in the gpu
inline ID3D11Device* g_pd3dDevice = nullptr;

//issues rendering commands
inline ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;

//holds the front and back bufferes and swaps them to the monitor
inline IDXGISwapChain* g_pSwapChain = nullptr;

//canvas; memory address on the gpu where the final image is painted 
inline ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

inline bool is_left_shift_down = false;
inline bool is_right_shift_down = false;
inline bool is_windows_visible = false;

HWND hWnd;
void showHideWindow(bool show);

void SetupKeyboardHooks();
void CleanupKeyboardHooks();

int main() 
{
    SetupKeyboardHooks();
    SetProcessDPIAware();

    //make process DPI aware and obtain main monitor scale
    ImGui_ImplWin32_EnableDpiAwareness();
    float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(
        ::MonitorFromPoint(POINT{ 0,0 }, MONITOR_DEFAULTTOPRIMARY)
    );

    auto win_x = ::GetSystemMetrics(SM_CXSCREEN);
    auto win_y = ::GetSystemMetrics(SM_CYSCREEN);

    //using win32 api to create a handle for a new window
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WinProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"Dog Ate My Mouse", nullptr };
    ::RegisterClassExW(&wc);
    hWnd = ::CreateWindowExW(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW, wc.lpszClassName, L"Dog ate my mouse", WS_POPUP, 0, 0, win_x, win_y, nullptr, nullptr, wc.hInstance, nullptr);

    MARGINS margins = { -1, -1, -1,-1 };
    ::DwmExtendFrameIntoClientArea(hWnd, &margins);
    ::SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);

    if (!CreateDeviceD3D(hWnd)) {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    //hide the window
    showHideWindow(false);

    //setup Dear ImGui contexnt
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    //setup Dear ImGui style
    ImGui::StyleColorsLight();

    //setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    style.FontScaleDpi = main_scale;

    //setup platform/renderer backends
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
    
    bool shutdown = false;

    //state
    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.00f);

    while (!shutdown) 
    {
        //check message queue & dispatch any new messages 
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);

            if (msg.message == WM_QUIT) 
                shutdown = true;
        }
        if (shutdown)
            break;

        //start new frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        //Bypass the ImGUI windows system and get the background canvas
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

        draw_list->AddText(
            ImVec2(1200.0f, 1200.0f),
            IM_COL32(255,0,0,255),
            "Tests"
        );
        
        //rendering
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };

        //point the directX commander at the canvas
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        
        //wipe the canvas clean with the clean color
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);

        //pain the ImGui vertex data onto the canvas
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        
        //Enable VSync
        HRESULT hr = g_pSwapChain->Present(1, 0);

    }

    //cleanup 
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
    
    CleanupKeyboardHooks();
    return 0;
}

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam )) 
    {
        return true;
    }

    switch (msg)
    {
        case WM_SIZE:
            if (wParam == SIZE_MINIMIZED) {
            }
            break;
        case WM_DESTROY:
        {
            ::PostQuitMessage(0);
            return 0;
        }
    }

    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

void showHideWindow(bool show)
{
    int mode = show ? SW_SHOWDEFAULT : SW_HIDE;
    ::ShowWindow(hWnd, mode);
    ::UpdateWindow(hWnd);

    is_windows_visible = show;
}

HBITMAP TakeScreenshot();
HHOOK hKeyboardHook = nullptr;
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
        {
            KBDLLHOOKSTRUCT* pKey = (KBDLLHOOKSTRUCT*)lParam;
            switch (pKey->vkCode)
            {
            case VK_LSHIFT:
            {
                is_left_shift_down = true;
                break;
            }
            case VK_RSHIFT:
            {
                is_right_shift_down = true;
                break;
            }
            case VK_ESCAPE:
            {
                if (is_windows_visible) {
                    is_left_shift_down = false;
                    is_right_shift_down = false;
                    showHideWindow(false);
                }
            }
            default:break;
            }
        }
    }
   
    if (is_left_shift_down && is_right_shift_down && !is_windows_visible)
    {
        HBITMAP screenshot = TakeScreenshot();
        points = Detect(HBITMAPToMat(screenshot));
        assert(screenshot && "Screenshot failure");
        showHideWindow(true);
    }

    //TODO add logic for external keyboard presses 
    return ::CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

void SetupKeyboardHooks()
{
    HINSTANCE hInstance = ::GetModuleHandle(NULL);
    hKeyboardHook = ::SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInstance, 0);
    assert(hKeyboardHook && "Keyboard low level hook proc couldn't be set");
}

void CleanupKeyboardHooks()
{
    if (hKeyboardHook != nullptr)
    {
        ::UnhookWindowsHookEx(hKeyboardHook);
        hKeyboardHook = nullptr;
    }
}

HBITMAP TakeScreenshot()
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


std::vector<std::pair<float, float>> Detect(cv::Mat image)
{
    std::cout << "Detecting\n";
    std::vector<std::pair<float, float>> result;
    result.reserve(100);

    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    cv::Ptr<cv::MSER> mser = cv::MSER::create(
        2,
        100,
        14400,
        .5,
        .0002,
        5,
        1.01,
        0.003,
        5
    );

    std::vector<std::vector<cv::Point>> regions;
    std::vector<cv::Rect> rects;

    mser->detectRegions(gray, regions, rects);

    int validPoint = 0;
    for (size_t i = 0; i < rects.size(); ++i) 
    {
        const cv::Rect& rect = rects[i];
        double aspectRatio = (double)rect.width / (double)rect.height;
        bool isNotTooThin = (aspectRatio > 0.5 && aspectRatio < 3.0);
        bool isNotTooBig = rect.area() < (image.cols * image.rows * 0.5);

        if (isNotTooThin && isNotTooBig)
        {
            validPoint++;
            result.emplace_back(
                std::pair<float, float>(
                    (float)rect.x + (float)(rect.width)/2, 
                    (float)rect.y + (float)(rect.height)/2
                )
            );
            std::cout
                << " x = " << ((float)rect.x + (float)(rect.width) / 2)
                << " y = " << ((float)rect.y + (float)(rect.height) / 2)
                << std::endl;
        }
    }
 
    std::cout << "Found " << result.size() << " valid elements.\n";
    return result;
}


cv::Mat HBITMAPToMat(HBITMAP hBitmap) 
{
    BITMAP bmp;
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
