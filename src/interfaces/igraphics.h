#pragma once
#include <vector>
#include "../entity.h"

class IGraphics {
public:
    virtual ~IGraphics() = default;
    virtual void start(const std::vector<std::pair<float, float>>& coordinates, const std::vector<Key>& keys) = 0;
    virtual void stop() = 0;
};


