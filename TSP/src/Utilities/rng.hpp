#pragma once

#include <random>
#include <chrono>

#include "matrix.hpp"

namespace rng {
    extern std::mt19937 engine;

    unsigned int random_uint(const unsigned int& max);

    matrix random_matrix(const unsigned short& n, const unsigned int& max);
}