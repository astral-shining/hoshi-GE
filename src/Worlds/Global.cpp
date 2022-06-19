#include "Global.hpp"
#include "Input.hpp"

Global::Global() {
    cat = &em.createEntity<Cat>();
}

void Global::update() {
    if (input->keyIsPressed(KEY_SPACE)) {
        glm::vec p = cat->getComponent<Transform2D>().position;
        cat = &em.createEntity<Cat>();
        cat->getComponent<Transform2D>().position = p;
    }
    if (input->keyIsPressed(KEY_F5)) {
        auto& l = em.get<Cat>();
        if (l.size() > 1) {
            l.erase(l.begin());
        } 
        cat = l.begin()+l.size()-1;
    }
    if (cat) {
        cat->move();
    }
        
}
