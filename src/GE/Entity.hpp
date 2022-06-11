#pragma once
#include <vector>
#include "Utility.hpp"
#include "SmartVector.hpp"

template<typename>
struct EntityManager;

template<typename... Ts>
struct EntityPool {
    using ComponentsUsed = norepeated_tuple_t<join_tuple_t<typename Ts::Components...>>;
    std::tuple<EntityManager<Ts>...> pool {};
    template<auto i>
    auto& get() {
        return std::get<i>(pool);
    }
    template<typename T>
    auto& get() {
        return std::get<EntityManager<T>>(pool);
    }

    void init() {

    }

    void foreachEntityManager(const auto& fn) {
        constexpr_for(auto i=0, i<sizeof...(Ts), i+1, 
            fn(get<i>());
        );
    }

    void staticForeachComponents(const auto& fn) {
        constexpr_for(auto i=0, i<std::tuple_size_v<ComponentsUsed>, i+1,
            using T = std::tuple_element_t<i AND ComponentsUsed>;
            fn.template operator()<T>();
        );
    }

    // init 
    void staticInitComponents() {
        staticForeachComponents([] <typename T> () {
            evalif_validexpr(T::staticInit());
        });
    }

    // update
    void update() {
        foreachEntityManager([] (auto& em) {
            em.update();
        });
        staticUpdateComponents();
    }

    void staticUpdateComponents() {
        staticForeachComponents([] <typename T> () {
            evalif_validexpr(T::staticUpdate());
        });
    }

    // destroy
    ~EntityPool() {
        staticDestroyComponents();
    }

    void staticDestroyComponents() {
        staticForeachComponents([] <typename T> () {
            evalif_validexpr(T::staticDestroy());
        });
    }
};

template<typename T>
struct EntityManager {
    SmartVector<T, true> entities;
    EntityManager() {
        entities.reserve(1024);
    }

    template<typename... Ts>
    auto& createEntity(Ts&&... args) {
        auto& e = entities.emplace_back(std::forward<Ts>(args)...);
        e.id = entities.size()-1;
        e.initComponents();
        e.init();
        return e;
    }

    void destroyEntity(uint32_t id) {
        for (auto it=entities.begin(); it != entities.end(); ++it) {
            if (it->id == id) {
                it->destroy();
                it->destroyComponents();
                entities.erase(it);
                break;
            }
        }
    }

    void update() {
        for (auto& e : entities) {
            e.updateComponents();
            e.update();
        }
    }

    void clear() {
        for (auto& e : entities) {
            e.destroyComponents();
            e.destroy();
        }
        entities.clear();
    }

    ~EntityManager() {
        clear();
    }

};

template<typename... Cmps>
struct Entity {
    uint32_t id;
    using Components = std::tuple<Cmps...>;
    Components components;

    void foreachComponents(const auto& fn) {
        constexpr_for(auto i=0, i<sizeof...(Cmps), i+1, 
            fn(std::get<i>(components));
        );
    }
    
    void initComponents() {
        foreachComponents([&] (auto& c) {
            evalif_validexpr(c.init(*this));
        });
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
};
