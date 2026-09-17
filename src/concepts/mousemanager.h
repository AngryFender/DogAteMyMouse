#include<concepts>

template <typename T>
concept MouseManager = requires(T obj, const std::pair<int,int> pixel) {
    { obj.click(pixel) } -> std::same_as<void>;
};