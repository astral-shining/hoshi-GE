#include <initializer_list>
#include <glm/vec2.hpp>
#include <vector>
#include <glad/glad.h>

#include "Form2D.hpp"
#include "Transform2D.hpp"
#include "../Render/Shader.hpp"
#include "Transform2D.hpp"
#include "../Utility.hpp"

Shader triangle_shader {
R"(#version 300 es
precision mediump float;
in vec2 vert;
in vec2 pos;

void main() {
    vec2 poss = (vert+pos) * 0.1f;
    gl_Position = vec4(poss, 0.0, 1.0);
})",
R"(#version 300 es
precision mediump float;
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
})"
};

static constexpr std::initializer_list<float> triangle_vertices {
    -0.5f, -0.5f,
     0.5f, -0.5f,
     0.0f,  0.5f
};

template<Form f>
void Form2D<f>::init() {
    indexes.push_back(transform_buffer.size());
    index = --indexes.end();
    transform_buffer.push_back(transform->position);
}

template<Form f>
void Form2D<f>::staticInit() {
    triangle_shader.compile();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &vertex_VBO);
    glGenBuffers(1, &transform_VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glBufferData(GL_ARRAY_BUFFER, triangle_vertices.size()*sizeof(float), triangle_vertices.begin(), GL_STATIC_DRAW);
    GLint vert = triangle_shader.getAttrib("vert");
    glVertexAttribPointer(vert, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(vert);


    glBindBuffer(GL_ARRAY_BUFFER, transform_VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 2, example, GL_DYNAMIC_DRAW); // instances
    GLint pos = triangle_shader.getAttrib("pos");
    glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(pos);
    glVertexAttribDivisor(pos, 1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind
    glDisableVertexAttribArray(vert);
    glDisableVertexAttribArray(pos);
}

template<Form f>
void Form2D<f>::staticUpdate() {
    // Update transform
    glBindBuffer(GL_ARRAY_BUFFER, transform_VBO);
    glBufferData(GL_ARRAY_BUFFER, transform_buffer.size()*sizeof(glm::vec2), transform_buffer.data(), GL_DYNAMIC_DRAW);
    triangle_shader.use();
    glBindVertexArray(VAO);
    
    //Transform2D::buffer.bind();
    //glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * instances.size(), instances.data(), GL_DYNAMIC_DRAW); // instances

    //glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * instances.size(), nullptr, GL_DYNAMIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * instances.size(), instances.data(), GL_DYNAMIC_DRAW);
    //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * instances.size(), instances.data());
    //std::cout << instances[0].x << std::endl;
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, transform_buffer.size());
}

template<Form f>
void Form2D<f>::staticDestroy() {
}

template<Form f>
void Form2D<f>::update() {
    transform_buffer[*index] = transform->position;
}

template<Form f>
Form2D<f>::~Form2D() {
    if (transform_buffer.size() > 0) {
        transform_buffer.erase(transform_buffer.begin() + *index);
        *--indexes.end() = *index;
        indexes.splice(index, indexes, --indexes.end());
        indexes.erase(index);
    }

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
}


template struct Form2D<Triangle>;
