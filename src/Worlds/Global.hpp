#include <World.hpp>
#include <variant>
#include <Utility.hpp>

#include "../Entities/Cat.hpp"
#include "../Entities/Dog.hpp"


struct Global : World<Cat, Dog> {
    std::variant<EntityRef<Cat>, EntityRef<Dog>> current_tool;

    Global() {
        current_tool = em.createEntity<Cat>();
    }

    void changeTool() {
        std::visit([&] <typename T> (T& v) {
            using V = decltype(current_tool);
            using newType = typename std::variant_alternative_t<(find_tuple_v<T, template_to_t<V, std::tuple>>+1) % std::variant_size_v<V>, V>::type;
            auto& e = em.createEntity<newType>();
            if (v.exists()) {
                e.template getComponent<Transform2D>().position = v->template getComponent<Transform2D>().position;
                v->destroy();
            }
            current_tool = e;
        }, current_tool);
    }

    void update() {
        auto& vec = em.get<Cat>();
        if (input->getKey(KEY_C)) {
            std::visit([&] <typename T> (T& e) {
                glm::vec p = e->template getComponent<Transform2D>().position;
                em.createEntity<T>()->template getComponent<Transform2D>().position = p;
            }, current_tool);
        }
        if (input->getKeyDown(KEY_X)) {
            changeTool();
        }
        if (input->getKey(KEY_Z)) {
            if (vec.size() > 1) {
                vec.pop_back();
            }
        }
        std::visit([] (auto& e) {
            if (e.exists()) {
                e->move();
            }
        }, current_tool);
    }
};
