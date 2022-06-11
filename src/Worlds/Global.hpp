#include <World.hpp>
#include "../Entities/Cat.hpp"
#include "../Entities/Dog.hpp"

struct Global : World<Cat, Dog> {
    void init(void);
    void update(void);
};
