#include "Global.hpp"

Global::Global() {
    auto id = em.createEntity<Cat>();
    em.createEntity<Dog>();
    em.destroyEntity<Cat>(id);
}

void Global::update() {
}
