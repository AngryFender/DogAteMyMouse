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

constexpr size_t TOTAL_CHAR = 40;
constexpr size_t TOTAL_COMBINATION = TOTAL_CHAR * TOTAL_CHAR;

constexpr std::array<char, 40> CHAR_POOL{ 
    'j','f','d','k','l', 's','a','h','g',';', 
    'q','w','e','r','t', 'y','u','i','p','o', 
    'z','x','c','v','b', 'n','m',',','.','/', 
    '1','2','3','4','5', '6','7','8','9','0', 
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
