#pragma once

#include "./interfaces/ikeygen.h"
#include "entity.h"
#include <unordered_set>
#include <iostream>

class KeyGen final : public IKeyGen
{
public:
    ~KeyGen() override = default;

    KeyGen(const PossibleCombo& combination): combos_(ALL_COMBINATION), qcombos_{}
    {
        for (const Key& key : combos_) 
        {
            qcombos_.push(&key);
        }
    };

    std::vector<Key> generate(const std::vector<std::pair<float, float>>& points, const ScreenInfo& screen) override 
    {
        std::queue<const Key*> temp{qcombos_};

        std::vector<Key> keys;
        keys.reserve(points.size());

        int random_index = 0;
        for (const auto& point : points)
        {
            const Key& key = *temp.front();
            keys.push_back(key);
            temp.pop();
        }
        return keys;
    }

    void refresh()override 
    {
        //combos_ = ALL_COMBINATION;
    };

private:
    const PossibleCombo& combos_;
    std::queue<const Key*> qcombos_;
};