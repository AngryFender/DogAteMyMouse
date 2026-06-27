#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <d3d11.h>
#include <tchar.h>
#include <Windows.h>

inline IDXGISwapChain* g_pSwapChain = nullptr;

LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int main() 
{
    //make process DPI aware and obtain main monitor scale
    ImGui_ImplWin32_EnableDpiAwareness();
    float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(
        ::MonitorFromPoint(POINT{ 0,0 }, MONITOR_DEFAULTTOPRIMARY)
    );

    //using win32 api to create a handle for a new window
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WinProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"Dog Ate My Mouse", nullptr };
    ::RegisterClassExW(&wc);
    HWND hWnd = ::CreateWindowW(wc.lpszClassName, L"Dog ate my mouse", WS_OVERLAPPEDWINDOW, 0, 0, (int)(1280 * main_scale), (int)(800 * main_scale), nullptr, nullptr, wc.hInstance, nullptr);

    //show the window
    ::ShowWindow(hWnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hWnd);
    
    bool shutdown = false;

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

        //Enable VSync
        //HRESULT hr = g_pSwapChain

    }

    return 0;
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
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
