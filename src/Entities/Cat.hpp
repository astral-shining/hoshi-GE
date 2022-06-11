#include <Entity.hpp>
#include <Components/Transform2D.hpp>
#include <Components/Form2D.hpp>

struct Cat : Entity<Transform2D, Form2D<Triangle>> {
    void update() {
        auto& c = getComponent<Transform2D>();
        auto& pos = c.getPos();
        c.setPos({pos.x+0.001f, pos.y});
    }
};
