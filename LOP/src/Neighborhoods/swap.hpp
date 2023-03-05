#pragma once

#include "solution.hpp"

namespace swap {
    long long difference(
        const solution& s,
        unsigned short i,
        unsigned short j
    );

    void move(
        solution& s,
        const unsigned short& i,
        const unsigned short& j
    );
}