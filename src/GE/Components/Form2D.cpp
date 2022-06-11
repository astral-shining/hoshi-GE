#include <initializer_list>
#include <glm/vec2.hpp>
#include <vector>
#include <GL/glew.h>

#include "Form2D.hpp"
#include "Transform2D.hpp"
#include "../Render/Shader.hpp"
#include "Transform2D.hpp"

Shader triangle_shader {
R"(#version 300 es
precision mediump float;
in vec2 vert;
in vec2 pos;

void main() {
    gl_Position = vec4(vert.x + pos.x, vert.y + pos.y, -1.0, 1.0);
})",
R"(#version 300 es
precision mediump float;
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
})"
};

static constexpr std::initializer_list<float> triangle_vertices {
    -0.5f, -0.5f,
     0.5f, -0.5f,
     0.0f,  0.5f
};

template<Form f>
void Form2D<f>::staticInit() {
    triangle_shader.compile();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &vertexVBO);
    glGenBuffers(1, &instanceVBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, triangle_vertices.size()*sizeof(float), triangle_vertices.begin(), GL_DYNAMIC_DRAW);
    GLint vert = triangle_shader.getAttrib("vert");
    glVertexAttribPointer(vert, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(vert);


    //glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 2, example, GL_DYNAMIC_DRAW); // instances
    Transform2D::buffer.bind();
    GLint pos = triangle_shader.getAttrib("pos");
    glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(pos);
    glVertexAttribDivisor(pos, 1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind
    glDisableVertexAttribArray(vert);
    glDisableVertexAttribArray(pos);

    std::cout << "staticInit\n";
}

template<Form f>
void Form2D<f>::staticUpdate() {
    triangle_shader.use();
    glBindVertexArray(VAO);
    //glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * instances.size(), instances.data(), GL_DYNAMIC_DRAW); // instances

    //glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * instances.size(), nullptr, GL_DYNAMIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * instances.size(), instances.data(), GL_DYNAMIC_DRAW);
    //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * instances.size(), instances.data());
    //std::cout << instances[0].x << std::endl;
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, Transform2D::buffer.data.size());
}

template<Form f>
void Form2D<f>::staticDelete() {

}

template<Form f>
Form2D<f>::~Form2D() {
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
