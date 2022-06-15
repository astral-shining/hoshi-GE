#pragma once
#include <memory>

#include "Window.hpp"
#include "WorldBase.hpp"

class Game {
    uint32_t fps {};

    void loop(void); 
    void init(void);
public:
    std::unique_ptr<WorldBase> world;
    Window window {"hakugame"};

    Game() { init(); }

    template<typename T>
    void runWorld(void) {
        world = std::make_unique<T>();
        loop();
    }
};

extern Game game;