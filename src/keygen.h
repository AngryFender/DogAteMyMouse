#pragma once

#include "./interfaces/ikeygen.h"

class KeyGen final : public IKeyGen
{
public:
    ~KeyGen() override = default;
    std::vector<Key> generate(const std::vector<std::pair<float, float>>& points) override 
    {
        std::vector<Key> keys;
        keys.reserve(points.size());

        
        return keys;
    }
};