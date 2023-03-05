#include "shake.hpp"
#include "rng.hpp"
#include "swap.hpp"

long long shake_swap(solution& s, const unsigned short& n_moves) {
    long long diff = 0;

    for (unsigned short m = 0; m<n_moves; ++m) {
        unsigned short i = rng::random_uint(s.size()-1);
        unsigned short j = rng::random_uint(s.size()-1);

        while (i==j or i+1==j or i==j+1) {
            i = rng::random_uint(s.size()-1);
            j = rng::random_uint(s.size()-1);
        }

        diff += swap::difference(s,i,j);
        swap::move(s,i,j);
    }

    return diff;
}