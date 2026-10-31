#pragma once
#include <gmock/gmock.h>
#include "entity.h"
#include <vector>

class MockTargetKeyManager {
public:
    MOCK_METHOD(
		(std::optional<std::pair<float, float>>),
		match_target_keys,
        (const char key)
    );

    MOCK_METHOD(
        (std::vector<Key>),
        get_target_keys,
        ((const std::vector<std::pair<float, float>>&) coordinates, const std::vector<Key>& keys)
    );

    MOCK_METHOD(
        void,
        clear,
        ()
    );

};
