#pragma once
#include <gmock/gmock.h>
#include "entity.h"
#include <vector>

class MockKeyboard {
public:
    MOCK_METHOD(
        void, 
        set_overlay_callback,
        ((const std::vector<char>&) shortcuts, std::function<void(bool)> overlay_callback)
	);

    MOCK_METHOD(
        void,
        set_keypress_callback,
        (std::function<void(const char)> keypress_callback)
    );

    MOCK_METHOD(
        void,
        set_overlay_state_getter,
        (std::function<bool()> overlay_state_getter)
    );

    MOCK_METHOD(
        void,
        wait_message,
        ()
    );

    MOCK_METHOD(
        void,
        handle_message,
        ()
    );
};
