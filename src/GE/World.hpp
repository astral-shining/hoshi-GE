#pragma once
#include <tuple>

#include "Entity.hpp"
#include "Utility.hpp"
#include "Camera.hpp"
#include "WorldBase.hpp"

template<typename... Ets>
class World : public WorldBase {
    EntityPool<Ets...> entity_manager {};
    
    virtual void update() {}
    virtual void destroy() {}

public:

    Camera camera;
    World() {
        camera.init();
    }

    // UPDATE
    void updateWorld() {
        entity_pool.update();
        update();
    }

};
