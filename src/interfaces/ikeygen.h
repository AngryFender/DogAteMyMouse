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


class KeyGenBase {
public:
    template <typename Self>
    auto generate(this const Self& self, const std::vector<std::pair<float,float>>& coordinates, const ScreenInfo& screen) {
        return self.generate_impl(coordinates, screen);
    }
};

