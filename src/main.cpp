
#include <vector>

#include <Game.hpp>
#include "Worlds/Global.hpp"

int main() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    window->setPos(1200, 100);
    runWorld<Global>();
    return 0;
}


