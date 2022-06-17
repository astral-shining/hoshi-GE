#pragma once
#include <vector>
#include <memory>

#include "Utility.hpp"
#include "SmartVector.hpp"

static uint32_t l_index {};
static uint32_t l_count {};

template<typename... Ts>
struct EntityManager {
    using ComponentsUsed = norepeated_tuple_t<join_tuple_t<typename Ts::Components...>>;
    std::tuple<SmartVector<Ts, true>...> pool {};
    EntityManager() {
        foreachVector([] (auto& vec) {
            vec.reserve(1024);
        });
        staticInitComponents();
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

    template<typename T>
    void foreachEntity(const auto& fn) {
        for (auto& el : get<T>()) {
            fn(el);
        }
    }

    void staticForeachComponents(const auto& fn) {
        constexpr_for(auto i=0, i<std::tuple_size_v<ComponentsUsed>, i+1,
            using T = std::tuple_element_t<i AND ComponentsUsed>;
            fn.template operator()<T>();
        );
    }

    // create
    template<typename T, typename... Args>
    auto& createEntity(Args&&... args) {
        l_index = get<T>().size();
        return get<T>().emplace_back(std::forward<Args>(args)...);
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


    // Create reference
    template<typename T>
    void ref(T& e) {
            //EntityRef<T> r = static_cast<EntityRef<T>>(e.reference);
        
        //return static_cast<EntityRef<T>>(e.reference);
        /*
        if (!r.ptr.get()) {
            r.ptr = std::make_shared<typename EntityRef<T>::Content>(e.index, &get<T>());
        }
        return r;*/
        /*if (!e.reference.ptr.get()) {
            e.reference.ptr = std::make_shared<EntityRef<T>>(e.index, get<T>());
        }
        return static_cast<std::shared_ptr<EntityRef<T>>>(e.reference.ptr);*/
    }

    ~EntityManager() {
        staticDestroyComponents();
    }
};

template<typename... Cmps>
struct Entity {
    //SmartVector<Entity, true>* vec { reinterpret_cast<SmartVector<Entity, true>*>(eparams->vec) };
    uint32_t index { l_index };
    using Components = std::tuple<Cmps...>;
    uint32_t id { l_count++ };
    Components components {
        ((Cmps*)0, this)...
    };

    Entity& operator=(Entity& e) {
        components = e.components;
        id = e.id;
        index = e.index;
        updateRef();
        return *this;
    }

    /*auto& ref() {
        if (!reference.get()) {
            reference = std::make_shared<EntityRef<Entity>::Content>(index, vec);
        }
        return reference;
    }*/

    void updateRef() {
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
        //updateRef();
    }
};
