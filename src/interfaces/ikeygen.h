#pragma once
#include "../entity.h"
#include <array>
#include <vector>

class IKeyGen
{
public:
    virtual ~IKeyGen() = default;
    virtual std::vector<Key> generate(const std::vector<std::pair<float, float>>& points, const ScreenInfo& screen) = 0;
    virtual void refresh() = 0;
};

