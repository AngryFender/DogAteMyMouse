#pragma once
#include <concepts>
#include <vector>
#include <utility>

template <typename T>
concept KeyboardManager = requires(T obj, std::vector<char> && shortcuts) {
    { obj.set_overlay_callback(std::move(shortcuts), [](bool) {}) } -> std::same_as<void>;
    { obj.set_keypress_callback([](const char) {}) } ->std::same_as<void>;
    { obj.set_overlay_state_getter([]()->bool {return false; }) }-> std::same_as<void>;
    { obj.wait_message() }->std::same_as<void>;
    { obj.handle_message() }->std::same_as<void>;
};