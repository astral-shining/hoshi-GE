#include "Global.hpp"
#include "Input.hpp"

Global::Global() {
    cat = &em.createEntity<Cat>(glm::vec2{});
}

void Global::update() {
    if (input->keyIsPressed(KEY_SPACE)) {
        if (cat) {
            cat = &em.createEntity<Cat>(cat->getComponent<Transform2D>().getPos());
        } else {
            cat = &em.createEntity<Cat>(glm::vec2{});
        }
    }
    if (input->keyIsPressed(KEY_F5)) {
        em.clear();
        cat = &em.createEntity<Cat>(glm::vec2{});
    }

    cat->move();
}
