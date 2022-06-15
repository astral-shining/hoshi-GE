#pragma once
#include <vector>
#include "Utility.hpp"
#include "SmartVector.hpp"

template<typename... Ts>
struct EntityManager {
    using ComponentsUsed = norepeated_tuple_t<join_tuple_t<typename Ts::Components...>>;
    std::tuple<SmartVector<Ts, true>...> pool {};
    EntityManager() {
        foreachVector([] (auto& vec) {
            vec.reserve(1024);
        });
    }

    template<auto i>
    auto& get() {
        return std::get<i>(pool);
    }

    template<typename T>
    auto& get() {
        return std::get<SmartVector<T, true>>(pool);
    }

    void foreachVector(const auto& fn) {
        (fn(std::get<SmartVector<Ts, true>>(pool)), ...);
    }

    void foreachEntity(const auto& fn) {
        foreachVector([&fn] (auto& vec) {
            for (auto& el : vec) {
                fn(el);
            }
        });
    }

    void staticForeachComponents(const auto& fn) {
        constexpr_for(auto i=0, i<std::tuple_size_v<ComponentsUsed>, i+1,
            using T = std::tuple_element_t<i AND ComponentsUsed>;
            fn.template operator()<T>();
        );
    }

    // create
    template<typename T, typename... Args>
    uint64_t createEntity(Args&&... args) {
        auto& e = get<T>().emplace_back(std::forward<Args>(args)...);
        return e.id;
    }

    // init 
    void staticInitComponents() {
        staticForeachComponents([] <typename T> () {
            evalif_validexpr(T::staticInit());
        });
    }

    // update
    void update() {
        foreachEntity([] (auto& e) {
            e.update();
            e.updateComponents();
        });
        staticUpdateComponents();
    }

    void staticUpdateComponents() {
        staticForeachComponents([] <typename T> () {
            evalif_validexpr(T::staticUpdate());
        });
    }

    // destroy
    void staticDestroyComponents() {
        staticForeachComponents([] <typename T> () {
            evalif_validexpr(T::staticDestroy());
        });
    }

    template<typename T>
    void destroyEntity(uint32_t id) {
        auto& vec = get<T>();
        for (auto it=vec.begin(); it != vec.end(); ++it) {
            if (it->id == id) {
                it->destroy();
                it->destroyComponents();
                vec.erase(it);
                break;
            }
        }
    }

    void clear() {
        foreachVector([] (auto& vec) {
            vec.clear();
        });
    }

    template<typename T>
    void clear() {
        get<T>().clear();
    }

    ~EntityManager() {
        staticDestroyComponents();
    }

    
};
static uint32_t entity_counter {};

template<typename... Cmps>
struct Entity {
    uint64_t id {entity_counter++};
    using Components = std::tuple<Cmps...>;
    Components components {
        ((Cmps*)0, this)...
    };

    Entity() {
    }

    void foreachComponents(const auto& fn) {
        constexpr_for(auto i=0, i<sizeof...(Cmps), i+1, 
            fn(std::get<i>(components));
        );
    }
    
    void destroyComponents() {
        foreachComponents([&] (auto& c) {
            evalif_validexpr(c.destroy(*this));
        });
    }

    void updateComponents() {
        foreachComponents([&] (auto& c) {
            evalif_validexpr(c.update(*this));
        });
    }

    template<typename T>
    [[nodiscard]] auto& getComponent() {
        return std::get<T>(components);
    }

    /*inline static std::vector<T> entities;

    template<typename... Ts>
    void create(Ts&&... args) {
        entities.emplace_back(std::forward<Ts>(args)...);
    }*/

    void update() {}
    void destroy() {}


    ~Entity() {
        std::cout << "~Entity\n";
    }
};
