#include <Windows.h>
#include <vector>
#include <functional>
#include "windowcontext.h"

class WinKeyboard {
public:
    void init() {
        //register win message handler here using context.hWnd
        HINSTANCE hInstance = ::GetModuleHandle(NULL);
        hook_ = ::SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInstance, 0);
    }

    void set_overlay_callback(const std::vector<char>& shortcut, std::function<void(bool)> callback) {
        overlay_callback_ = callback;
    }

    void set_keypress_callback(std::function<void(const char)>callback) {
        keypress_callback_ = callback;
    }

    //blocking until a key is pressed
    void wait_message(){
        MSG msg;
        if (::GetMessage(&msg, nullptr, 0, 0))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }

    void handle_message(){
        MSG msg;

        //loops until all messages are dispatched
        //exits the loop when there are no more messages left in the queue
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }

private:
    HHOOK hook_ = nullptr;
    std::function<void(bool)> overlay_callback_;
    std::function<void(const char)> keypress_callback_;

    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
    {
        //TODO add key detection logic here
        return ::CallNextHookEx(nullptr, nCode, wParam, lParam);
    }
};