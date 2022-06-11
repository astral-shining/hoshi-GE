#include <initializer_list>
#include <stdint.h>
#include <GL/glew.h>
#include <tuple>
#include <vector>
#include <list>
#include <iostream>

template<typename T>
struct VertexElement;

template<typename T>
class InstancedVertexBuffer {
    uint32_t VBO;
    uint32_t draw_type;
public:
    using type = T;
    std::vector<T> data;
    std::vector<VertexElement<T>*> elements;

    InstancedVertexBuffer() {
    }

    void create() {
        glGenBuffers(1, &VBO);
        data.reserve(1000);
    }

    void update() {
        bind();
        glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(T), data.data(), GL_DYNAMIC_DRAW);
        unbind();
    }

    void updateIndex(uint32_t index) {
        bind();
        glBufferSubData(GL_ARRAY_BUFFER, index*sizeof(T), sizeof(T), data.data()+index);
        unbind();
    }

    void bind() {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
    }

    void unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void del() {
        glDeleteBuffers(1, &VBO);
        unbind();
    }
};

template<typename T>
struct VertexElement {
    InstancedVertexBuffer<T>* buffer;
    uint32_t index {};

    VertexElement(InstancedVertexBuffer<T>& buffer) : buffer(&buffer) {}

    [[nodiscard]] T& getValue() const {
        return buffer->data[index];
    }

    void setValue(const T& v) {
        buffer->data[index] = v;
        buffer->updateIndex(index);
    }

    void create() {
        index = buffer->data.size();
        buffer->data.emplace_back();
        buffer->update();
        buffer->elements.push_back(this);
        //it_value = buffer->data.end()-1;
    }
    /*VertexElement& operator=(const VertexElement& e) {
        return *this;
    }*/

    void destroy() {
        buffer->data[index] = buffer->data[buffer->data.size()-1];
        buffer->elements[index] = buffer->elements[buffer->data.size()-1];
        buffer->elements[index]->index = index;
        buffer->data.pop_back();
        buffer->elements.pop_back();
    }
};
