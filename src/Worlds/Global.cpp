#include "Global.hpp"

Global::Global() {
    auto id = createEntity<Cat>();
    createEntity<Dog>();

    destroyEntity<Cat>(id);
}

void Global::update() {
}
