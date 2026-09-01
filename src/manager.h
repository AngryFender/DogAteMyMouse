#pragma once

#include "entity.h"
#include <vector>

template <
    typename Renderer,
    typename KeyboardListener,
    typename ScreenCapturer,
    typename MouseClicker,
    typename MatchEngine
>
class Manager {

public:
    Manager(Renderer&& renderer,
        KeyboardListener&& listener,
        ScreenCapturer&& capturer,
        MouseClicker&& clicker,
        MatchEngine&& match_engine,
        )
        : renderer_(std::move(renderer),
            keyboard_listener_(std::move(listener)),
            screen_capturer_(std::move(capturer),
                mouse_clicker_(std::move(clicker)),
                match_engine_(std::move(match_engine)),
                shutdown{ false }
    {
        //init
        renderer_.init();
        capturer_.init();
        keyboard_listener_.init();

        coordinates_.reserve(TOTAL_COMBINATION);
        keys_.reserve(TOTAL_COMBINATION);
        
        //keyboard_listener_.set_callback()

    }

    void start() {
        while (renderer.shutdown() || shutdown) {
            //TODO logic inside the main loop

            if (!renderer.is_window_visible()) {
                keyboard_listener_.consume_message();
                continue;
            }

            //logic when window is visible
            keyboard_listener_.handle_message();

            //render frames
            renderer_.render_frame(coordinates_, keys);

        }
    }

    void stop() {

    }

    ~Manager() {
        shutdown = false;
        renderer_.clearup();
        capturer_.clearup();
        keyboard_listener.clearup();
    }

private:
    Renderer renderer_;
    KeyboardListener keyboard_listener_;
    ScreenCapturer screen_capturer_;
    MouseClicker mouse_clicker_;
    MatchEngine match_engine_;

    //local data
    bool shutdown;
    std::vector<std::pair<float, float>> coordinates_;
    std::vector<Key> keys_;
    ScreenInfo screen;
};
