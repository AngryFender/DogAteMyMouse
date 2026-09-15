
#include <optional>
#include <vector>
#include "../entity.h"

template <typename T>
concept TargetKeysManager = requires(T obj, const char key, std::vector<std::pair<float, float>>&coordinates, const ScreenInfo & screen) {
    { obj.match_target_keys(key) } -> std::same_as<std::optional<std::pair<float, float>>>;
    { obj.get_target_keys(coordinates, screen) } -> std::same_as<std::vector<std::pair<float, float>>>;
    { obj.clear() } -> std::same_as<void>;
};

