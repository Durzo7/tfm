#include "rng.hpp"

std::mt19937 rng::engine(std::chrono::system_clock::now().time_since_epoch().count());

unsigned int rng::random_uint(const unsigned int& max) {
    return engine()%(max+1);
}
