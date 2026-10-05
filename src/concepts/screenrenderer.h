#pragma once
#include <concepts>
#include <vector>
#include <utility>
#include "../entity.h"

template <typename T>
concept ScreenRenderer = requires (T obj,const std::vector<std::pair<float, float>>& coordinates, const std::vector<Key>& keys, const bool show) {
    { obj.init() } -> std::same_as<void>;
    { obj.render_frame(coordinates, keys) } -> std::same_as<void>;
    { obj.showOverlay(show) } -> std::same_as<void>;
    { obj.refresh() } -> std::same_as<void>;
    { obj.screenInfo() }->std::same_as<const ScreenInfo&>;
};


