#pragma once
#include "directxgraphics.h"
#include "winkeyboard.h"
#include "winmouse.h"
#include "wincapturer.h"
#include "overlaycontext.h"

static_assert(ScreenRenderer<DirectXGraphics>, "DirectXGraphics does not satisfy the ScreenRenderer concept!");
static_assert(MouseManager<WinMouse>, "WinMouse does not satisfy the MouseManager concept!");
static_assert(KeyboardManager<WinKeyboard>, "WinKeyboard does not satisfy the KeyboardManager concept!");

class WinComposition {

public:

    DirectXGraphics create_renderer() {
        return DirectXGraphics(OverlayContext());
    }

    WinKeyboard create_listener() {
        return WinKeyboard();
    }

    WinMouse create_clicker() {
        return WinMouse();
    }

    WinCapturer create_capturer() {
        return WinCapturer();
    }
};

using NativeComposition = WinComposition;