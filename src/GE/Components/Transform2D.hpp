#pragma once
#include <glm/vec2.hpp>
#include <glm/gtx/string_cast.hpp>

#include "../Render/InstancedVertexBuffer.hpp"

class Transform2D {
    VertexElement<glm::vec2> position_ve { buffer };

public:
    inline static InstancedVertexBuffer<glm::vec2> buffer;

    static void staticInit() {
        buffer.create();
    }

    void init(auto& e) {
        position_ve.create();
        //a.push_back(10);
    }

    [[nodiscard]] glm::vec2& getPos() const {
        return position_ve.getValue();
    }

    void setPos(const glm::vec2& v) {
        position_ve.setValue(v);
    }

    void destroy(auto& e) {
        position_ve.destroy();
    }

    static void staticUpdate() {
        //buffer.update();
    }

    

    Transform2D() {
        
    }

    void update([[maybe_unused]] auto& e) {
        
    }
};
