#include<concepts>
#include<vector>

template <typename T>
concept KeyboardManager = requires(T obj, const std::vector<char>&shortcut) {
    { obj.set_overlay_callback(shortcut, [](bool) {}) } -> std::same_as<void>;
    { obj.set_keypress_callback([](const char) {}) } ->std::same_as<void>;
    { obj.wait_message() }->std::same_as<void>;
    { obj.handle_message() }->std::same_as<void>;
};