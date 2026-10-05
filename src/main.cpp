#include <cassert>
#include <utility>
#include "concepts/keyboardmanager.h"
#include "concepts/screenrenderer.h"
#include "concepts/mousemanager.h"
#include "concepts/targetkeysmanager.h"
#include "concepts/keygenerator.h"
#include "entity.h"
#include "manager.h"
#include "overlaycontext.h"
#include "emptyconfig.h"
#include "directxgraphics.h"
#include "wincapturer.h"
#include "winmouse.h"
#include "winkeyboard.h"
#include "matchengine.h"
#include "keygen.h"
#include "strategies/cca.h"

//hide the console window
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup" )

int main(int argc, char** argv) {

    static_assert(ScreenRenderer<DirectXGraphics>, "DirectXGraphics does not satisfy the ScreenRenderer concept!");
    static_assert(MouseManager<WinMouse>, "WinMouse does not satisfy the MouseManager concept!");
    static_assert(KeyboardManager<WinKeyboard>, "WinKeyboard does not satisfy the KeyboardManager concept!");
    static_assert(KeyGenerator<KeyGen>, "Keygen does the satisfy the KeyGenerator concept!");
    static_assert(TargetKeysManager<MatchEngine<KeyGen>>, "MatchEngine<Keygen> does not satisfy the TargetKeysManager concept!");

    KeyGen keygen(ALL_COMBINATION);
    MatchEngine engine(std::move(keygen));

    Manager manager(
        EmptyConfig(),
        DirectXGraphics(OverlayContext()),
        WinKeyboard(),
        WinCapturer(),
        WinMouse(),
        std::move(engine),
        CCA()
    );

    manager.start();

    return 0;
}

