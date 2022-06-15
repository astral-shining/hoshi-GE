#pragma once
#include <cstdint>

struct WorldBase {
    uint64_t entity_counter {};
    virtual void updateWorld() = 0;
    virtual ~WorldBase() = default;
};