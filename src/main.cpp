#include "concepts/targetkeysmanager.h"
#include "concepts/keygenerator.h"
#include "entity.h"
#include "manager.h"
#include "emptyconfig.h"
#include "matchengine.h"
#include "keygen.h"
#include "strategies/cca.h"
#include "compositionfactory.h"

//hide the console window
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup" )

int main(int argc, char** argv) {

    static_assert(KeyGenerator<KeyGen>, "Keygen does the satisfy the KeyGenerator concept!");
    static_assert(TargetKeysManager<MatchEngine<KeyGen>>, "MatchEngine<Keygen> does not satisfy the TargetKeysManager concept!");

    CompositionFactory<NativeComposition> factory{};

    Manager manager(
        EmptyConfig(),
        factory.get_renderer(),
        factory.get_listener(),
        factory.get_capturer(),
        factory.get_clicker(),
        MatchEngine(KeyGen(ALL_COMBINATION)),
        CCA()
    );

    manager.start();

    return 0;
}

