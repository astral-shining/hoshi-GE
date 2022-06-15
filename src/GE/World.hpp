#pragma once
#include <tuple>

#include "Entity.hpp"
#include "Utility.hpp"
#include "Camera.hpp"

struct WorldBase {
    uint64_t entity_counter {};
    virtual void updateWorld() = 0;
    virtual ~WorldBase() = default;
};

template<typename... Ets>
class World : public WorldBase {
    virtual void update() {}
    virtual void destroy() {}

public:
    EntityManager<Ets...> em {};
    Camera camera;
    World() {
        camera.init();
    }

    // UPDATE
    void updateWorld() {
        em.update(); // Update all entities
        update();
    }

};
