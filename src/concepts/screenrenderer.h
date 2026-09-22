#include <concepts>
#include <vector>
#include "../entity.h"

template <typename T>
concept ScreenRenderer = requires (T obj,const std::vector<std::pair<float, float>>& coordinates, const std::vector<Key>& keys) {
    { obj.init() } -> std::same_as<void>;
    { obj.is_shutdown() } -> std::same_as<bool>;
    { obj.render_frame(coordinates, keys) } -> std::same_as<void>;
};


