#include <utility>

#include "ILS.hpp"
#include "local_search.hpp"
#include "global.hpp"
#include "shake.hpp"

long long ILS_LOP(solution& s) {
    long long best_cost = s.cost();
    global::n_evals = 0;
    global::n_local_optima = 0;
    best_cost += local_search(s);
    long long curr_cost = best_cost;
    solution curr_solution = s;

    while (global::n_evals<global::max_evals) {
        solution new_solution = curr_solution;
        long long new_cost = curr_cost+shake_swap(new_solution,7);
        if (global::n_evals<global::max_evals) new_cost += local_search(new_solution);

        if (new_cost>(1.0-0.0001)*curr_cost) {
            curr_cost = new_cost;
            curr_solution = std::move(new_solution);

            if (curr_cost>best_cost) {
                best_cost = curr_cost;
                s = std::move(solution(curr_solution));
            }
        }
    }

    return best_cost;
}