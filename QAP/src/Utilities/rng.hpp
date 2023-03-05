#pragma once

#include <random>
#include <chrono>

namespace rng {
    extern std::mt19937 engine;

    unsigned int random_uint(const unsigned int& max);
}