#pragma once
#include <gmock/gmock.h>
#include "entity.h"
#include <vector>

class MockKeygen {
public:
    MOCK_METHOD(
        std::vector<Key>, 
        generate,
        ((const std::vector<std::pair<float, float>>&) coordinates, const ScreenInfo& screen)
	);
};