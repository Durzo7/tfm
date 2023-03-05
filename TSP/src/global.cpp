#include <limits>

#include "global.hpp"

const matrix* global::M_ptr = nullptr;
unsigned int global::n_evals = 0;
unsigned int global::max_evals = std::numeric_limits<unsigned int>::max();
unsigned int global::n_local_optima = 0;
unsigned int global::consec_local_optima = 0;
unsigned int global::n_jumps = 0;
unsigned int global::n_plateaus = 0;
unsigned int global::n_starts = 0;
bool global::prev_local_optima = false;