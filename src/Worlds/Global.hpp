#include <World.hpp>
#include "../Entities/Cat.hpp"
#include "../Entities/Dog.hpp"

struct Global : World<Cat, Dog> {
    EntityRef<Cat> current_cat;
    EntityRef<Cat> example;
    Global(void);
    void update(void);
};
