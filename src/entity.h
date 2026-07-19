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

constexpr size_t TOTAL_CHAR = 30;
constexpr size_t TOTAL_COMBINATION = TOTAL_CHAR * TOTAL_CHAR;

constexpr std::array<char, 30> CHAR_POOL{ 
    'q','w','e','r','t', 'y','u','i','p','o', 
    'a','s','d','f','g', 'h','j','k','l',';', 
    'z','x','c','v','b', 'n','m',',','.','/' 
};

using PossibleCombo = std::array<Key, TOTAL_COMBINATION>;

constexpr PossibleCombo generate_all_combinations()
{
    PossibleCombo combo{};
    int index = 0;
    for (char first : CHAR_POOL)
    {
        for (char second : CHAR_POOL)
        {
            combo[index] = { first, second };
            ++index;
        }
    }

    return combo;
};

constexpr PossibleCombo ALL_COMBINATION = generate_all_combinations();
