#include <Windows.h>

class WinMouse {

public:

    WinMouse() = default;

    void click(const std::pair<int, int> pixel)const {
        //move the cursor to the target pixel
        ::SetCursorPos(pixel.first, pixel.second);

        //create an array of 2 INPUT structures and initialize them to 0
        INPUT inputs[2] = { 0 };

        //configure the first event (Mouse Left Button DOWN)
        inputs[0].type = INPUT_MOUSE;
        inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

        //configure the second event (Mouse Left Button UP)
        inputs[1].type = INPUT_MOUSE;
        inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

        //send the events to the operating system
        ::SendInput(2, inputs, sizeof(INPUT));
    }

};
