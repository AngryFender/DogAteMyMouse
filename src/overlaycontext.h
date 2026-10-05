#pragma once

#if defined(_WIN32)
#include <Windows.h>
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#endif

#include "entity.h" 

struct OverlayContext {

#if defined(_WIN32)

    HWND hWnd;
    ScreenInfo screen;
    WNDCLASSEXW wc;

    OverlayContext() {
        ::ImGui_ImplWin32_EnableDpiAwareness();
        screen.width = ::GetSystemMetrics(SM_CXSCREEN);
        screen.height = ::GetSystemMetrics(SM_CYSCREEN);

        wc = {
            sizeof(wc), CS_CLASSDC,
            ::DefWindowProcW,       //Calls the default window procedure to provide default processing for any window messages that an application does not process. This function ensures that every message is processed. DefWindowProc is called with the same parameters received by the window procedure.
            0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
            L"Dog Ate My Mouse", nullptr
        };

        ::RegisterClassExW(&wc);
        hWnd = ::CreateWindowExW(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW, wc.lpszClassName, L"Dog ate my mouse", WS_POPUP, 0, 0, screen.width, screen.height, nullptr, nullptr, wc.hInstance, nullptr);
    }

    ~OverlayContext() {
        if (hWnd) {
            ::DestroyWindow(hWnd);
        }
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
    }

    OverlayContext(const OverlayContext&) = delete;
    OverlayContext& operator=(const OverlayContext&) = delete;

    OverlayContext(OverlayContext&& other) :
        hWnd(std::exchange(other.hWnd, nullptr)),
        screen(std::exchange(other.screen, { 0,0 })),
        wc(other.wc)
    {
        other.wc.lpszClassName = nullptr;
    }

    OverlayContext& operator=(OverlayContext&& other) {
        if (this != &other) {

            if (hWnd) {
                ::DestroyWindow(hWnd);
            }
            if (wc.lpszClassName) {
                ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
            }

            hWnd = std::exchange(other.hWnd, nullptr);
            screen = std::exchange(other.screen, { 0, 0 });
            wc = other.wc;

            other.wc.lpszClassName = nullptr;
        }
        return *this;
    }

#else



#endif

};




