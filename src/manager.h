#pragma once

#include "entity.h"
#include <vector>

template <
    typename Renderer,
    typename KeyboardListener,
    typename ScreenCapturer,
    typename MouseClicker,
    typename MatchEngine,
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

    }

    void start() {
        while (renderer.shutdown() || shutdown) {
            //TODO logic inside the main loop
            


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
    bool shutdown;
};
