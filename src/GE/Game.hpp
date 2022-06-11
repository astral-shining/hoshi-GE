#pragma once
#include <memory>

#include "Window.hpp"
#include "World.hpp"

class Game {
    std::unique_ptr<WorldBase> world;
    Window window {"hakugame"};
    uint32_t fps {};

    void loop(void); 
    void init(void);
public:

    Game() { init(); }

    template<typename T>
    void runWorld(void) {
        world = std::make_unique<T>();
        loop();
    }
};

extern Game game;
