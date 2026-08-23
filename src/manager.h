#pragma once

template <
    typename Renderer,
    typename ScreenCapturer,
    typename MouseClicker
>
class Manager {

public:
    Manager(Renderer&& renderer, ScreenCapturer&& capturer, MouseClicker&& clicker)
        : renderer_(std::move(renderer),
          screen_capturer_(std::move(capturer),
          mouse_clicker_(std::move(clicker)) {
    }

    ~Manager() {

    }

private:
    Renderer renderer_;
    ScreenCapturer screen_capturer_;
    MouseClicker mouse_clicker_;

};
