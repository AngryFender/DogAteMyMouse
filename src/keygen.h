#pragma once

#include "./interfaces/ikeygen.h"
#include "entity.h"
#include <unordered_set>
#include <iostream>

class KeyGen final : public IKeyGen
{
public:
    ~KeyGen() override = default;

    KeyGen(): combos_(ALL_COMBINATION)
    {};

    std::vector<Key> generate(const std::vector<std::pair<float, float>>& points, const ScreenInfo& screen) override 
    {
        std::vector<Key> keys;
        keys.reserve(points.size());

        int random_index = 0;
        for (const auto& point : points)
        {
            random_index = rand() % (TOTAL_COMBINATION-1) ;
            keys.push_back(std::move(combos_[random_index]));
        }

        return keys;
    }

    void refresh()override 
    {
        combos_ = ALL_COMBINATION;
    };

private:
    PossibleCombo combos_;

};