#pragma once
#include <gmock/gmock.h>
#include "entity.h"
#include <vector>

class MockMouse {
public:
    MOCK_METHOD(
        void,
        click,
        ((const std::pair<int, int>) pixel)
    );
};
