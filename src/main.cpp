
#include <vector>

#include <Game.hpp>
#include "Worlds/Global.hpp"

int main() {
    game.window.setPos(1200, 100);
    game.runWorld<Global>();
    return 0;
}


