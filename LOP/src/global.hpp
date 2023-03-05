#pragma once

#include "matrix.hpp"

namespace global {
    extern const matrix* M_ptr;
    extern unsigned long long n_evals;
    extern unsigned long long max_evals;
    extern unsigned long long n_local_optima;
    extern unsigned long long consec_local_optima;
    extern unsigned long long n_jumps;
    extern unsigned long long n_plateaus;
    extern unsigned long long n_starts;
    extern bool prev_local_optima;
}