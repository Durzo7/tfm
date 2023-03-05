#pragma once

#include "solution.hpp"

namespace two_opt {
    long long difference(
        const solution& s,
        const unsigned short& u,
        const unsigned short& v
    );

    void move(
        solution& s,
        unsigned short u,
        unsigned short v
    );
}