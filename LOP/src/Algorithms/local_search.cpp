#include <vector>

#include "local_search.hpp"
#include "global.hpp"
#include "shuffler.hpp"
#include "insert.hpp"
#include "neighbor.hpp"
#include "rng.hpp"


long long improved_neighbor_obi(solution& curr_solution) {
    long long best_diff = 0;
    unsigned short bi, bj;

    for (unsigned short i = 0; i<curr_solution.size(); ++i) {
        long long new_diff = 0;
        for (unsigned short j = i-1; j<i; --j, ++global::n_evals) {
            new_diff += (*global::M_ptr)(curr_solution[i],curr_solution[j]);
            new_diff -= (*global::M_ptr)(curr_solution[j],curr_solution[i]);

            if (new_diff>best_diff) {
                best_diff = new_diff;
                bi = i; bj = j;
            }
        }

        new_diff = 0;
        for (unsigned short j = i+1; j<curr_solution.size(); ++j, ++global::n_evals) {
            new_diff += (*global::M_ptr)(curr_solution[j],curr_solution[i]);
            new_diff -= (*global::M_ptr)(curr_solution[i],curr_solution[j]);

            if (new_diff>best_diff) {
                best_diff = new_diff;
                bi = i; bj = j;
            }
        }
    }

    if (best_diff>0) insert::move(curr_solution,bi,bj);
    else ++global::n_local_optima;

    return best_diff;
}

long long improved_neighbor_rfi(solution& curr_solution) {
    std::vector<neighbor> improvements;

    for (shuffler is(curr_solution.size()); not is.empty() and global::n_evals<global::max_evals;) {
        const unsigned short& i = is.next();
        
        long long new_diff = 0;
        for (unsigned short j = i-1; j<i and global::n_evals<global::max_evals; --j, ++global::n_evals) {
            new_diff += (*global::M_ptr)(curr_solution[i],curr_solution[j]);
            new_diff -= (*global::M_ptr)(curr_solution[j],curr_solution[i]);

            if (new_diff>0) improvements.emplace_back(i,j,new_diff);
        }

        new_diff = 0;
        for (unsigned short j = i+1; j<curr_solution.size() and global::n_evals<global::max_evals; ++j, ++global::n_evals) {
            new_diff += (*global::M_ptr)(curr_solution[j],curr_solution[i]);
            new_diff -= (*global::M_ptr)(curr_solution[i],curr_solution[j]);

            if (new_diff>0) improvements.emplace_back(i,j,new_diff);
        }

        if (not improvements.empty()) {
            unsigned short k = rng::random_uint(improvements.size()-1);
            insert::move(curr_solution,improvements[k].i,improvements[k].j);
            return improvements[k].d;
        }
    }
    if (global::n_evals<global::max_evals) ++global::n_local_optima;

    return 0;
}

long long improved_neighbor_LSf(solution& curr_solution) {
    for (unsigned short i = 0; i<curr_solution.size(); ++i) {
        long long best_diff = 0;
        unsigned short bi, bj;

        long long new_diff = 0;
        for (unsigned short j = i-1; j<i; --j, ++global::n_evals) {
            new_diff += (*global::M_ptr)(curr_solution[i],curr_solution[j]);
            new_diff -= (*global::M_ptr)(curr_solution[j],curr_solution[i]);

            if (new_diff>best_diff) {
                best_diff = new_diff;
                bi = i; bj = j;
            }
        }

        new_diff = 0;
        for (unsigned short j = i+1; j<curr_solution.size(); ++j, ++global::n_evals) {
            new_diff += (*global::M_ptr)(curr_solution[j],curr_solution[i]);
            new_diff -= (*global::M_ptr)(curr_solution[i],curr_solution[j]);

            if (new_diff>best_diff) {
                best_diff = new_diff;
                bi = i; bj = j;
            }
        }

        if (best_diff>0) {
            insert::move(curr_solution,bi,bj);
            return best_diff;
        }
    }
    ++global::n_local_optima;

    return 0;
}

long long local_search(solution& s) {
    long long diff = 0;

    while (global::n_evals<global::max_evals) {
        long long d = improved_neighbor_obi(s);

        if (d>0) diff += d;
        else break;
    }

    return diff;
}