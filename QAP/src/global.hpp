#pragma once

#include "matrix.hpp"

namespace global {
    extern const matrix* A_ptr;
    extern const matrix* B_ptr;
    extern unsigned int n_evals;
    extern unsigned int max_evals;
    extern unsigned int n_local_optima;
    extern unsigned int consec_local_optima;
    extern unsigned int n_jumps;
    extern unsigned int n_plateaus;
    extern unsigned int n_starts;
    extern bool prev_local_optima;
    extern bool flag;
}