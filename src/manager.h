#pragma once

#include "entity.h"

template <
    typename Renderer,
    typename KeyboardListener,
    typename ScreenCapturer,
    typename MouseClicker
>
class Manager {

public:
    Manager(Renderer&& renderer, KeyboardListener&& listener, ScreenCapturer&& capturer, MouseClicker&& clicker)
        : renderer_(std::move(renderer),
          keyboard_listener_(std::move(listener)),
          screen_capturer_(std::move(capturer),
          mouse_clicker_(std::move(clicker)),
          shutdown{ false }
    {
        //init
        coordinates_.reserve(TOTAL_COMBINATION);
        keys_.reserve(TOTAL_COMBINATION);
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
    }

private:
    Renderer renderer_;
    KeyboardListener keyboard_listener_;
    ScreenCapturer screen_capturer_;
    MouseClicker mouse_clicker_;
    bool shutdown;

    std::vector<std::pair<float, float>> coordinates_;
    std::vector<Key> keys_;
    ScreenInfo screen_;

};
