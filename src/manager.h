#pragma once
#include "entity.h"
#include "overlaycontext.h"
#include <vector>
#include <concepts>
#include "./concepts/screenrenderer.h"
#include "./concepts/keyboardmanager.h"
#include "./concepts/targetkeysmanager.h"
#include "./concepts/mousemanager.h"
#include <utility>
#include <algorithm>

template <typename C>
using ImageReturnType = decltype(std::declval<C>().capture());

template <typename ObjectDetect, typename ImageType>
concept MatchStrat = requires(ObjectDetect d, ImageType t) {
    { d(std::move(t)) } -> std::same_as<std::vector<std::pair<float, float>>>;
};

template <
    typename Config,
    ScreenRenderer Renderer,
    KeyboardManager KeyboardListener,
    typename ScreenCapturer,
    MouseManager MouseClicker,
    TargetKeysManager MatchEngine,
    MatchStrat<ImageReturnType<ScreenCapturer>> DetectStrat
>
class Manager {

public:
    Manager(
        Config&& config,
        Renderer&& renderer,
        KeyboardListener&& listener,
        ScreenCapturer&& capturer,
        MouseClicker&& clicker,
        MatchEngine&& match_engine,
        DetectStrat&& detect_strat)
        :
        config_(std::move(config)),
        renderer_(std::move(renderer)),
        keyboard_listener_(std::move(listener)),
        screen_capturer_(std::move(capturer)),
        mouse_clicker_(std::move(clicker)),
        match_engine_(std::move(match_engine)),
        detect_strat_(std::move(detect_strat)),
        shutdown{ false },
        is_overlay_visible_{ false }
    {
        //init
        renderer_.init();

        coordinates_.reserve(TOTAL_COMBINATION);
        keys_.reserve(TOTAL_COMBINATION);
        
        keyboard_listener_.set_overlay_callback(
            std::vector<char>{},
            [this](bool show_overlay)
            {
                if (show_overlay) {

                    //take screenshot
                    auto screenshot = screen_capturer_.capture();

                    //run cv algorithm on the screenshot
                    coordinates_ = detect_strat_(std::move(screenshot));

                    //generate random keys and coordinates
                    keys_ = match_engine_.get_target_keys(coordinates_, renderer_.screenInfo());

                    //refresh is important for the render loop to get the 
                    renderer_.refresh();
                }
                else {

                    //reset 
                    coordinates_.clear();
                    keys_.clear();
                    match_engine_.clear();
                }

                renderer_.showOverlay(show_overlay);
                is_overlay_visible_ = show_overlay;
            }
        );

        keyboard_listener_.set_keypress_callback(
            [this](const char key) 
            {
                auto result = match_engine_.match_target_keys(key);
                if (result)
                {
                    auto pixel = result.value();
                    //hide window through renderer;

                    //emulate mouse press
                    mouse_clicker_.click(pixel);

                    //clear all states
                    renderer_.showOverlay(false);
                    is_overlay_visible_ = false;

                    coordinates_.clear();
                    keys_.clear();
                    match_engine_.clear();
                }
            }
        );

        keyboard_listener_.set_overlay_state_getter(
            [this]() {
                return is_overlay_visible_;
            }
        );

    }

    void start() {
        //control loop
        while (!shutdown) {

            if (!is_overlay_visible_) {
                keyboard_listener_.wait_message(); //get message, translate message & dispatch message
                continue;
            }

            //logic when window is visible
            keyboard_listener_.handle_message(); //peek message, translate message & dispatch message

            //render frames
            renderer_.render_frame(coordinates_, keys_);
        }
    }

    void stop() {

    }

    ~Manager() {
        shutdown = true;
    }

private:
    Config config_;
    Renderer renderer_;
    KeyboardListener keyboard_listener_;
    ScreenCapturer screen_capturer_;
    MouseClicker mouse_clicker_;
    MatchEngine match_engine_;
    DetectStrat detect_strat_;

    //local data
    bool shutdown;
    std::vector<std::pair<float, float>> coordinates_;
    std::vector<Key> keys_;
    bool is_overlay_visible_;
};
