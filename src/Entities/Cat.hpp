#include <Entity.hpp>
#include <Input.hpp>
#include <Components/Transform2D.hpp>
#include <Components/Form2D.hpp>

struct Cat : Entity<Cat, Transform2D, Form2D<Triangle>> {

    void update() {
    }
    void move() {
        auto& c = getComponent<Transform2D>();
        float velocity = 10.f * delta_time;
        if (input->keyIsPressed(KEY_LEFT)) {
            c.position.x -= velocity;
        }
        if (input->keyIsPressed(KEY_RIGHT)) {
            c.position.x += velocity;
        }
        if (input->keyIsPressed(KEY_UP)) {
            c.position.y += velocity;
        }
        if (input->keyIsPressed(KEY_DOWN)) {
            c.position.y -= velocity;
        }
    }
};
