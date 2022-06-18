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
        float velocity = delta_time * 15;

        if (input->keyIsPressed(KEY_LEFT)) {
            c.setPos({pos.x-velocity, pos.y});
        } 
        if (input->keyIsPressed(KEY_RIGHT)) {
            c.setPos({pos.x+velocity, pos.y});
        } 
        if (input->keyIsPressed(KEY_UP)) {
            c.setPos({pos.x, pos.y+velocity});
        } 
        if (input->keyIsPressed(KEY_DOWN)) {
            c.setPos({pos.x, pos.y-velocity});
        } 
        
    }
};
