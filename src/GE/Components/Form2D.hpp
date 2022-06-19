#pragma once
#include <glm/vec2.hpp>
#include <list>

#include "Transform2D.hpp"
#include "../RelativePtr.hpp"
#include "../SmartVector.hpp"

enum Form {
    Triangle
};

template<Form f>
struct Form2D {
    inline static SmartVector<glm::vec2, true> transform_buffer;
    inline static std::list<uint32_t> indexes;
    inline static uint32_t VAO;
    inline static uint32_t vertex_VBO;
    inline static uint32_t transform_VBO;
    
    std::list<Form2D*>::iterator formptr_list_it;

    std::list<uint32_t>::iterator index;
    RelativePtr<Transform2D> transform;
    
    Form2D(auto p) : transform(&p->template getComponent<Transform2D>()) {
        init();
    }

    ~Form2D();

    void init(void);
    void update(void);
    static void staticInit(void);
    static void staticUpdate(void);
    static void staticDestroy(void);
};
