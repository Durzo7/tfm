#include <utility>

#include "VNS.hpp"
#include "swap.hpp"
#include "shake.hpp"
#include "global.hpp"
#include "matrix.hpp"
#include "local_search.hpp"

long long swap_step(solution& s) {
    long long best_diff = 0;
    unsigned short bi, bj;

    for (unsigned short i = 0; i<s.size()-1; ++i) {
        for (unsigned short j = i+1; j<s.size(); ++j) {
            long long new_diff = swap::difference(s,i,j);

            if (new_diff>best_diff) {
                best_diff = new_diff;
                bi = i; bj = j;
            }
        }
    }
    
    if (best_diff>0) swap::move(s,bi,bj);
    else ++global::n_local_optima;

    return best_diff;
}

long long swap_step_fast(solution& s) {
    long long best_diff = 0;
    unsigned short bi, bj;
    matrix M(s.size());

    for (unsigned short i = 0; i<s.size(); ++i) {
        long long val = 0;
        for (unsigned short j = i-1; j<i; --j, ++global::n_evals) {
            val += (*global::M_ptr)(s[i],s[j])-(*global::M_ptr)(s[j],s[i]);
            M(i,j) = val;
        }

        val = 0;
        for (unsigned short j = i+1; j<s.size()-1; ++global::n_evals) {
            val += (*global::M_ptr)(s[j],s[i])-(*global::M_ptr)(s[i],s[j]);
            M(i,++j) = val;
        }
    }

    for (unsigned short i = 0; i<s.size()-1; ++i) {
        for (unsigned short j = i+1; j<s.size(); ++j) {
            long long new_diff = M(i,j)+M(j,i);

            if (new_diff>best_diff) {
                best_diff = new_diff;
                bi = i; bj = j;
            }
        }
    }

    if (best_diff>0) swap::move(s,bi,bj);
    else ++global::n_local_optima;

    return best_diff;
}

long long VNS(solution& s) {
    solution curr_solution = s;
    long long best_cost = s.cost();
    long long curr_cost = best_cost;
    global::n_evals = 0;
    global::n_local_optima = 0;

    while (global::n_evals<global::max_evals) {
        curr_cost += local_search(curr_solution);
        long long diff = global::n_evals<global::max_evals ? swap_step_fast(curr_solution) : 0;
        
        if (diff>0) curr_cost += diff;
        else {
            if (curr_cost>best_cost) {
                best_cost = curr_cost;
                s = std::move(solution(curr_solution));
            }
            if (global::n_evals<global::max_evals)
                curr_cost += shake_swap(curr_solution,curr_solution.size()/10);
        }
    }

    return best_cost;
}