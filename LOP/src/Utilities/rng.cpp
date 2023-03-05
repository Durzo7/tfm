#include "rng.hpp"

std::mt19937 rng::engine(std::chrono::system_clock::now().time_since_epoch().count());

unsigned int rng::random_uint(const unsigned int& max) {
    return engine()%(max+1);
}

matrix rng::random_matrix(const unsigned short& n, const unsigned int& max) {
    matrix M(n);

    for (unsigned short i = 0; i<n-1; ++i) {
        for (unsigned short j = i+1; j<n; ++j) {
            M(i,j) = random_uint(max);
            M(j,i) = random_uint(max);
        }
    }

    return M;
}