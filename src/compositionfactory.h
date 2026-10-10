#pragma once
#include <utility>
#include <algorithm>
#include "entity.h"
#include "manager.h"
#include "overlaycontext.h"
#include <vector>
#include <concepts>
#include "./concepts/screenrenderer.h"
#include "./concepts/keyboardmanager.h"
#include "./concepts/targetkeysmanager.h"
#include "./concepts/mousemanager.h"
#if defined(_WIN32)
#include "wincomposition.h"
#endif

template < typename Factory >
class CompositionFactory {
public:
    auto get_renderer() {
        return factory_.get_renderer();
    }

    auto get_listener() {
        return factory_.get_listener();
    }

    auto get_clicker() {
        return factory_.get_clicker();
    }

    auto get_capturer() {
        return factory_.get_capturer();
    }

private:
    Factory factory_;

};