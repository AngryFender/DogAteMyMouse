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

template < typename Factory >
class CompositionFactory {
public:
    auto create_renderer() {
        return factory_.create_renderer();
    }

    auto create_listener() {
        return factory_.create_listener();
    }

    auto create_clicker() {
        return factory_.create_clicker();
    }

    auto create_capturer() {
        return factory_.create_capturer();
    }

private:
    Factory factory_;
};