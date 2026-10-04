#pragma once
#include <concepts>
#include <vector>
#include <utility>
#include <functional>

template <typename T>
concept KeyboardManager = requires(
    T obj,
    std::vector<char> && shortcuts,
    std::function<void(bool)> overlay_callback,
    std::function<void(const char)> keypress_callback,
    std::function<bool()> overlay_state_getter
) {
    { obj.set_overlay_callback(std::move(shortcuts), overlay_callback) } -> std::same_as<void>;
    { obj.set_keypress_callback(keypress_callback) } ->std::same_as<void>;
    { obj.set_overlay_state_getter(overlay_state_getter) }-> std::same_as<void>;
    { obj.wait_message() }->std::same_as<void>;
    { obj.handle_message() }->std::same_as<void>;
};