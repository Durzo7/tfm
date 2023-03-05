#pragma once

#include "solution.hpp"
#include "neighbor.hpp"

long long local_search(solution& s, matrix& D);

neighbor initialize_matrix(solution& s, matrix& D);

neighbor update_matrix(solution& s, matrix& D, const neighbor& mv);