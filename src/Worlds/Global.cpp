#include "Global.hpp"

void Global::init() {
    auto id = createEntity<Cat>();
    createEntity<Dog>();

    destroyEntity<Cat>(id);
}

void Global::update() {
}
