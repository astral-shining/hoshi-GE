#pragma once
#include <glm/vec2.hpp>
#include <glm/gtx/string_cast.hpp>

#include "RelativePtr.hpp"
#include "../Render/InstancedVertexBuffer.hpp"

class Transform2D {
    VertexElement<glm::vec2> position_ve { buffer };
public:
    static InstancedVertexBuffer<glm::vec2> buffer;

    Transform2D(auto p) {
    }

    [[nodiscard]] glm::vec2& getPos() const {
        return position_ve.getValue();
    }

    void setPos(const glm::vec2& v) {
        position_ve.setValue(v);
    }

    static void staticUpdate() {
        buffer.update();
    }

    void update([[maybe_unused]] auto& e) {
        
    }

    ~Transform2D() {
    }
};

