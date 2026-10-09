#pragma once
#include "directxgraphics.h"
#include "winkeyboard.h"
#include "winmouse.h"
#include "wincapturer.h"
#include "overlaycontext.h"

class WinComposition {

public:

    DirectXGraphics get_renderer(OverlayContext&& context) {
        return DirectXGraphics(std::move(context));
    }

    WinKeyboard get_listener() {
        return WinKeyboard();
    }

    WinMouse get_clicker() {
        return WinMouse();
    }

    WinCapturer get_capturer() {
        return WinCapturer();
    }
};