#include "local_search.hpp"
#include "global.hpp"
#include "two_opt.hpp"


long long improved_neighbor(solution& s) {
    long long best_diff = 0;
    unsigned short bu, bv;

    for (unsigned short u = 1; u<s.size()-1; ++u) {
        for (unsigned short v = u+1; v<s.size()-(u==1); ++v) {
            long long new_diff = two_opt::difference(s,u,v);

            if (new_diff<best_diff) {
                best_diff = new_diff;
                bu = u; bv = v;
            }
        }
    }

    if (best_diff<0) two_opt::move(s,bu,bv);
    else ++global::n_local_optima;

    return best_diff;
}

long long local_search(solution& s) {
    long long diff = 0;

    while (global::n_evals<global::max_evals) {
        long long d = improved_neighbor(s);

        if (d<0) diff += d;
        else break;
    }

    return diff;
}