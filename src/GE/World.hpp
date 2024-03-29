#pragma once
#include <tuple>
#include <memory>

#include "Entity.hpp"
#include "Utility.hpp"
#include "Camera.hpp"

struct WorldBase {
    uint64_t entity_counter {};
    virtual void updateWorld() = 0;
    virtual ~WorldBase() = default;
};

inline std::unique_ptr<WorldBase> world;
extern double delta_time;

template<typename... Ets>
class World : public WorldBase {
    virtual void update() {}
    virtual void destroy() {}

public:
    using Entities = std::tuple<Ets...>;
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
