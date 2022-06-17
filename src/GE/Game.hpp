#pragma once
#include <memory>

#include "Window.hpp"
#include "Input.hpp"
#include "World.hpp"

void G_loop(void);
void G_init(void);

template<typename T>
void runWorld(void) {
    world = std::make_unique<T>();
    G_loop();
}
