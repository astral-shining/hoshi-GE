#pragma once
#include <glm/vec2.hpp>
#include <glm/gtx/string_cast.hpp>

#include "RelativePtr.hpp"

struct Transform2D {
    glm::vec2 position {};

    Transform2D(auto e) {
    }

    static void staticUpdate() {
        //buffer.update();
    }

    void update([[maybe_unused]] auto& e) {
        
    }

    ~Transform2D() {
    }
};

