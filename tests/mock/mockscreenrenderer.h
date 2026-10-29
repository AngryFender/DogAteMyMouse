#pragma once
#include <gmock/gmock.h>
#include "entity.h"
#include <vector>

class MockScreenRenderer {
public:
    MOCK_METHOD(
        void,
        init,
        ()
    );

    MOCK_METHOD(
        void,
        render_frame,
        ((const std::vector<std::pair<float, float>>&) coordinates, const std::vector<Key>& keys)
    );

    MOCK_METHOD(
        void,
        show_overlay,
        (const bool show)
    );

    MOCK_METHOD(
        void,
        refresh,
        ()
	);
    
    MOCK_METHOD(
        const ScreenInfo&,
        screen_info,
        ()
    );

};
