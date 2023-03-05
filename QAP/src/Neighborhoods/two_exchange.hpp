#pragma once

#include "solution.hpp"

namespace two_exchange {
    long long difference(
        const solution& s,
        const unsigned short& i,
        const unsigned short& j
    );

    long long delta_difference(
        const solution& s,
        const unsigned short& i,
        const unsigned short& j,
        const unsigned short& k,
        const unsigned short& l
    );

    void move(
        solution& s,
        unsigned short i,
        unsigned short j
    );
}