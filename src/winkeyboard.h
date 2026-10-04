#pragma once
#include <Windows.h>
#include <vector>
#include <functional>
#include <utility>
#include <cassert>
#include <map>
#include <algorithm>

class WinKeyboard {
public:
    WinKeyboard() {

        //setting global low level keyboard hook
        HINSTANCE hInstance = ::GetModuleHandle(NULL);
        hook_ = ::SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInstance, 0);

        //default shortcut of left and right shifts keys 
        shortcuts_[VK_LSHIFT] = false;
        shortcuts_[VK_RSHIFT] = false;
    }

    ~WinKeyboard() {
        if (hook_) {
            ::UnhookWindowsHookEx(hook_);
        }
    }

    WinKeyboard(WinKeyboard& other) = delete;
    WinKeyboard& operator=(WinKeyboard& other) = delete;

    WinKeyboard(WinKeyboard&& other) {
        hook_ = std::exchange(other.hook_, nullptr);
    }

    WinKeyboard& operator=(WinKeyboard&& other) {
        if (this != &other) {
            hook_ = std::exchange(other.hook_, nullptr);
        }
        return *this;
    }

    void set_overlay_callback(std::vector<char>&& shortcuts, std::function<void(bool)> callback) {
        if (!shortcuts.empty()) {
            //TODO convert keypress from agnostic format to windows format
            assert(false && "Conversion from Config keyboard shortcut to Windows keyboard is still in developement");
        }
        overlay_callback_ = std::move(callback);
    }

    void set_keypress_callback(std::function<void(const char)> callback) {
        keypress_callback_ = std::move(callback);
    }

    void set_overlay_state_getter(std::function<bool()> getter) {
        is_overlay_visible_ = std::move(getter);
    }

    //blocking until a key is pressed
    void wait_message() {
        MSG msg;
        if (::GetMessage(&msg, nullptr, 0, 0))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }

    void handle_message() {
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
    inline static std::map<DWORD, bool> shortcuts_;
    inline static std::function<void(bool)> overlay_callback_;
    inline static std::function<void(const char)> keypress_callback_;
    inline static std::function<bool()> is_overlay_visible_;

    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
    {
        if (nCode == HC_ACTION)
        {
            if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
            {
                KBDLLHOOKSTRUCT* pKey = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

                const auto& key_unpressed = pKey->vkCode;
                if (shortcuts_.contains(key_unpressed)) {
                    shortcuts_[key_unpressed] = false;
                }

            }

            if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
            {
                KBDLLHOOKSTRUCT* pKey = (KBDLLHOOKSTRUCT*)lParam;

                const auto& key_pressed = pKey->vkCode;
                if (shortcuts_.contains(key_pressed)) {
                    shortcuts_[key_pressed] = true;
                }
                else {
                    if (key_pressed == VK_ESCAPE) {

                        //if ESC is pressed then reset all the keypresses
                        for (auto& [_, state] : shortcuts_) {
                            state = false;
                        }

                        //if display is visible then hide it
                        if (is_overlay_visible_ && is_overlay_visible_()) {
                            overlay_callback_(false);
                        }

                    }
                    else {
                        if (is_overlay_visible_ && is_overlay_visible_()) {
                            BYTE keyboardState[256];
                            ::GetKeyboardState(keyboardState);

                            keyboardState[VK_SHIFT] = ::GetKeyState(VK_SHIFT) & 0x8000;
                            keyboardState[VK_CAPITAL] = ::GetKeyState(VK_CAPITAL) & 0x8000;
                            keyboardState[VK_CONTROL] = ::GetKeyState(VK_CONTROL) & 0x8000;
                            keyboardState[VK_MENU] = ::GetKeyState(VK_MENU) & 0x8000;

                            HKL keyboardLayout = ::GetKeyboardLayout(GetWindowThreadProcessId(GetForegroundWindow(), NULL));

                            WORD asciiChar = 0;

                            int result = ::ToAsciiEx(
                                pKey->vkCode,
                                pKey->scanCode,
                                keyboardState,
                                &asciiChar,
                                0,
                                keyboardLayout
                            );
                            if (result > 0)
                            {
                                keypress_callback_(asciiChar);
                            }
                            return 1;
                        }
                    }
                }
            }
        }

        const bool shortcut_triggered = std::all_of(shortcuts_.cbegin(), shortcuts_.cend(), [](const auto& pair) {
            return pair.second;
            }
        );

        if (is_overlay_visible_ && !is_overlay_visible_() && shortcut_triggered) {
            // if overlay is hidden and all shortcut keys are pressed then show overlay 
            overlay_callback_(true);

            //reset all shortcut presses
            for (auto& [_, state] : shortcuts_) {
                state = false;
            };
        }

        return ::CallNextHookEx(nullptr, nCode, wParam, lParam);
    }
};