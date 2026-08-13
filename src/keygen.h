#pragma once

#include "./interfaces/ikeygen.h"
#include "entity.h"
#include <unordered_set>
#include <iostream>

class KeyGen final : public IKeyGen
{
public:
    ~KeyGen() override = default;

    KeyGen(const PossibleCombo& combination) :qcombos_{}
    {
        for (const Key& key : combination)
        {
            qcombos_.push(&key);
        }
    };

    std::vector<Key> generate(const std::vector<std::pair<float, float>>& coordinates, const ScreenInfo& screen) override
    {
        std::queue<const Key*> temp{ qcombos_ };

        std::vector<Key> keys;
        keys.reserve(coordinates.size());

        for (const auto& point : coordinates)
        {
            keys.push_back(*(temp.front()));
            temp.pop();
        }
        return keys;
    }

private:
    std::queue<const Key*> qcombos_;
};