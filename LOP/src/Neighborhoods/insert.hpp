#pragma once

#include "solution.hpp"

namespace insert {
    long long difference(
        const solution& s,
        const unsigned short& i,
        const unsigned short& j
    );

    void move(
        solution& s,
        const unsigned short& i,
        const unsigned short& j
    );
}