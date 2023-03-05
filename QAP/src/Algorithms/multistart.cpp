#include <utility>

#include "multistart.hpp"
#include "global.hpp"
#include "local_search.hpp"

long long multistart(solution& s, const unsigned int& max_starts) {
    long long best_cost = s.cost();
    global::n_local_optima = 0;
    global::n_starts = 0;
    global::n_evals = 0;
    matrix D(s.size());

    for (; global::n_starts<max_starts and global::n_evals<global::max_evals; ++global::n_starts) {
        solution curr_solution;
        long long curr_cost = curr_solution.cost();
        if (global::n_evals<global::max_evals) curr_cost += local_search(curr_solution,D);

        if (curr_cost<best_cost) {
            best_cost = curr_cost;
            s = std::move(curr_solution);
        }
    }

    return best_cost;
}