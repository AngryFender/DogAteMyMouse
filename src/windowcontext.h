#pragma once

#include <Windows.h>
#include "entity.h" 

struct WindowContext {

#if defined(_WIN32)
    HWND hWnd;
    ScreenInfo screen;
    WindowContext() {

        screen.width = ::GetSystemMetrics(SM_CXSCREEN);
        screen.height = ::GetSystemMetrics(SM_CYSCREEN);

        WNDCLASSEXW wc = {
            sizeof(wc), CS_CLASSDC,
            ::DefWindowProcW,       //Calls the default window procedure to provide default processing for any window messages that an application does not process. This function ensures that every message is processed. DefWindowProc is called with the same parameters received by the window procedure.
            0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
            L"Dog Ate My Mouse", nullptr
        };
        ::RegisterClassExW(&wc);
        hWnd = ::CreateWindowExW(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW, wc.lpszClassName, L"Dog ate my mouse", WS_POPUP, 0, 0, screen.width, screen.height, nullptr, nullptr, wc.hInstance, nullptr);
    }

#else



#endif

};




