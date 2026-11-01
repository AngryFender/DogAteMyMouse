#pragma once
#include <gmock/gmock.h>
#include "entity.h"
#include <vector>

class FakeImage {

};

class MockScreenCapturer {
public:
    MOCK_METHOD(
        FakeImage,
        capture,
        ()
    );
};

class MockStrategy {
public:
    MOCK_METHOD(
		(std::vector<std::pair<float, float>>),
		mocked_operator,
        (FakeImage& image)
    );

    std::vector<std::pair<float, float>> operator()(auto&& image) {
        return mocked_operator(image);
    }

};
