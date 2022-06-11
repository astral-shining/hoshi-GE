#pragma once
#include <glm/vec2.hpp>
#include <vector>

#include "Transform2D.hpp"

enum Form {
    Triangle
};

template<Form f>
struct Form2D {
    inline static std::vector<glm::vec2> instances {};
    inline static uint32_t VAO;
    inline static uint32_t vertexVBO;
    inline static uint32_t instanceVBO;

    std::vector<glm::vec2>::iterator pos_it;

    Form2D() {
        /*auto& pos = e.template getComponent<Transform2D>().position;
        instances.push_back(pos);
        pos_it = instances.end()-1;*/
    }

    static void staticInit(void);
    static void staticUpdate(void);
    static void staticDelete(void);

    void init(auto&) {}
    void update(auto& e) {
        /*auto& transform = e.template getComponent<Transform2D>();
        if (transform.changed) {
            *pos_it = transform.position;
        }*/
    }

    ~Form2D();
        /*Form2D*& last = *(formList.end()-1);
        Form2D*& current = formList[pos_it-positions.begin()];
        std::cout << this << std::endl;*/
        //std::swap(*last->pos_it, *pos_it);
        /*positions.pop_back();

        std::swap(last, current);*/
        //std::swap();
        
        /**pos_it = *(positions.end()-1); // Replace content
        positions.pop_back(); // Remove last item
        (*(formList.end()-1))->pos_it = pos_it; // Replace iterator
                                              //
        formList[positions.begin()-pos_it] = *formList.end();
        formList.pop_back();*/
};
