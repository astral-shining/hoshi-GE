#pragma once
#include <tuple>

#include "Entity.hpp"
#include "Utility.hpp"
#include "Camera.hpp"

struct WorldBase {
    virtual void updateWorld() = 0;
    virtual ~WorldBase() = default;
};

template<typename... Ets>
class World : public WorldBase {
    EntityPool<Ets...> entity_pool {};
    
    virtual void init() {}
    virtual void update() {}
    virtual void destroy() {}

public:

    Camera camera;
    World() {
        init();
        camera.init();
    }

    template<typename T>
    uint32_t createEntity() {
        return entity_pool.template get<T>().createEntity().id;
    }

    template<typename T>
    void destroyEntity(uint32_t id) {
        entity_pool.template get<T>().destroyEntity(id);
    }

    // UPDATE
    void updateWorld() {
        entity_pool.update();
        update();
    }

};
