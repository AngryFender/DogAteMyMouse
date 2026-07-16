#pragma once
#include <cassert>
#include <array>

struct ScreenInfo
{
    int width;
    int height;
};

constexpr size_t BUFFER_SIZE = 2;
using Key = std::array<char, BUFFER_SIZE>;
struct KeyHasher
{
    std::size_t operator()(const Key& k) const
    {
        assert(BUFFER_SIZE == 2 && "KeyHasher is implemented only for 2 characters");

        return (static_cast<std::size_t>(k[0]) | static_cast<std::size_t>(k[1]));
    }
};

constexpr size_t COMBINATION = 30 * 30;
