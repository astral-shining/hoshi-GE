#include "Global.hpp"
#include "Input.hpp"

Global::Global() {
    current_cat = em.createEntity<Cat>();
}

void Global::update() {
    auto& vec = em.get<Cat>();
    if (input->keyIsPressed(KEY_C)) {
        glm::vec p = current_cat->getComponent<Transform2D>().position;
        em.createEntity<Cat>().getComponent<Transform2D>().position = p;
    }
    if (input->keyIsPressed(KEY_X)) {
        example = vec.back();
    }
    if (input->keyIsPressed(KEY_Z)) {
        if (vec.size() > 1) {
            vec.pop_back();
        }
    }
    if (current_cat.exists()) {
        current_cat->move();
    }
}
