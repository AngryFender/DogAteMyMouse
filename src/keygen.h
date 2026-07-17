#pragma once

#include "./interfaces/ikeygen.h"
#include "entity.h"

class KeyGen final : public IKeyGen
{
public:
    ~KeyGen() override = default;
    std::vector<Key> generate(const std::vector<std::pair<float, float>>& points, const ScreenInfo& screen) override 
    {
        std::vector<Key> keys;
        keys.reserve(points.size());

        
        return keys;
    }

    void refresh()override 
    {
    };
};