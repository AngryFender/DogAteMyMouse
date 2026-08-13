#pragma once
#include "../entity.h"
#include <array>
#include <vector>

class IKeyGen
{
public:
    virtual ~IKeyGen() = default;
    virtual std::vector<Key> generate(const std::vector<std::pair<float, float>>& coordinates, const ScreenInfo& screen) = 0;
};

