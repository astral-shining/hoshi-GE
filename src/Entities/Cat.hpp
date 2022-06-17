#include <Entity.hpp>
#include <Input.hpp>
#include <Components/Transform2D.hpp>
#include <Components/Form2D.hpp>

struct Cat : Entity<Transform2D, Form2D<Triangle>> {
    bool disabled {};
    Cat(glm::vec2 pos) {
        getComponent<Transform2D>().setPos(pos);
    }
    void move() {
        auto& c = getComponent<Transform2D>();
        auto& pos = c.getPos();

        if (input->keyIsPressed(KEY_LEFT)) {
            c.setPos({pos.x-0.01f, pos.y});
        } 
        if (input->keyIsPressed(KEY_RIGHT)) {
            c.setPos({pos.x+0.01f, pos.y});
        } 
        if (input->keyIsPressed(KEY_UP)) {
            c.setPos({pos.x, pos.y+0.01f});
        } 
        if (input->keyIsPressed(KEY_DOWN)) {
            c.setPos({pos.x, pos.y-0.01f});
        } 
        
    }
};
