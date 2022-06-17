#include <World.hpp>
#include "../Entities/Cat.hpp"
#include "../Entities/Dog.hpp"

struct Global : World<Cat, Dog> {
    Cat* cat {};
    Global(void);
    void update(void);
};
