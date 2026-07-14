#pragma once
#include <array>
#include <vector>

constexpr size_t BUFFER_SIZE = 2;
using Key = std::array<char, BUFFER_SIZE>;

class IKeyGen 
{
public:
    virtual ~IKeyGen() = default;
    virtual std::vector<Key> generate(const int size) = 0;
};

