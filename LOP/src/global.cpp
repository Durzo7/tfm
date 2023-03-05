#include <limits>

#include "global.hpp"

const matrix* global::M_ptr = nullptr;
unsigned long long global::n_evals = 0;
unsigned long long global::max_evals = std::numeric_limits<unsigned long long>::max();
unsigned long long global::n_local_optima = 0;
unsigned long long global::consec_local_optima = 0;
unsigned long long global::n_jumps = 0;
unsigned long long global::n_plateaus = 0;
unsigned long long global::n_starts = 0;
bool global::prev_local_optima = false;