#pragma once
#include <memory>

#include "Input.hpp"
#include "Window.hpp"
#include "World.hpp"


template<typename T>
using UPtr = std::unique_ptr<T>;

void G_loop(void);
void G_init(void);

extern UPtr<Window> window;
extern UPtr<Input> input;
extern UPtr<WorldBase> world;

template<typename T>
void runWorld(void) {
    world = std::make_unique<T>();
    G_loop();
}
