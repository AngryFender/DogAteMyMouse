#pragma once
#include "../entity.h"
#include <array>
#include <vector>

class IKeyGen
{
public:
    virtual ~IKeyGen() = default;
    virtual std::vector<Key> generate(const std::vector<std::pair<float, float>>& points) = 0;
};

