#pragma once
#include "../entity.h"
#include <array>
#include <vector>

template <typename T>
concept KeyGenerator = requires(T obj, const std::vector<std::pair<float, float>>&coordinates, const ScreenInfo & screen) {
    { obj.generate(coordinates, screen) } -> std::same_as<std::vector<Key>>;
};

