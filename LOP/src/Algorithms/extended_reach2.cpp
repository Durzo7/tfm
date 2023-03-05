#include <array>
#include <vector>
#include <algorithm>
#include <iostream>

#include "extended_reach2.hpp"
#include "global.hpp"
#include "insert.hpp"
#include "shuffler.hpp"
#include "neighbor.hpp"
#include "matrix.hpp"

long long best_improvement(solution& s, matrix& M_diff) {
    long long best_diff = 0;
    unsigned short bi, bj;

    for (unsigned short i = 0; i<s.size(); ++i) {
        for (unsigned short j = i-1; j<i; --j, ++global::n_evals) {
            M_diff(i,j) = M_diff(i,j+1)+(*global::M_ptr)(s[i],s[j])-(*global::M_ptr)(s[j],s[i]);

            if (M_diff(i,j)>best_diff) {
                best_diff = M_diff(i,j);
                bi = i; bj = j;
            }
        }
        for (unsigned short j = i+1; j<s.size(); ++j, ++global::n_evals) {
            M_diff(i,j) = M_diff(i,j-1)+(*global::M_ptr)(s[j],s[i])-(*global::M_ptr)(s[i],s[j]);

            if (M_diff(i,j)>best_diff) {
                best_diff = M_diff(i,j);
                bi = i; bj = j;
            }
        }
    }

    if (best_diff>0) insert::move(s,bi,bj);
    return best_diff;
}

long long local_search_er(solution& s, matrix& M_diff) {
    long long diff = 0;

    while (global::n_evals<global::max_evals) {
        long long d = best_improvement(s,M_diff);

        if (d>0) diff += d;
        else break;
    }

    return diff;
}

std::vector<neighbor> best_neighbors(const matrix& M_diff, const long long& threshold) {
    std::vector<neighbor> neighbors;

    for (unsigned short i = 0; i<M_diff.size(); ++i) {
        for (unsigned short j = i-2; j<i; --j) {
            if (M_diff(i,j)>=threshold)
                neighbors.emplace_back(i,j,M_diff(i,j));
        }
        for (unsigned short j = i+1; j<M_diff.size(); ++j) {
            if (M_diff(i,j)>=threshold)
                neighbors.emplace_back(i,j,M_diff(i,j));
        }
    }
    std::sort(neighbors.begin(),neighbors.end());

    return neighbors;
}

neighbor adjustment(
    const solution& s,
    const unsigned short& i,
    const unsigned short& j,
    const unsigned short& k,
    const unsigned short& l
) {
    ++global::n_evals;

    if (k<l) {
        if (i<j) {
            if (i<k and k<j and j<=l)
                return {i,(unsigned short)(j-1),(*global::M_ptr)(s[i],s[k])-(*global::M_ptr)(s[k],s[i])};
            if (k<i and i<=l and l<=j)
                return {(unsigned short)(i-1),j,(*global::M_ptr)(s[k],s[i])-(*global::M_ptr)(s[i],s[k])};
        }
        else {
            if (j<k and k<i and i<=l)
                return {(unsigned short)(i-1),j,(*global::M_ptr)(s[k],s[i])-(*global::M_ptr)(s[i],s[k])};
            if (k<j-1 and j-1<=l and l<i)
                return {i,(unsigned short)(j-1),(*global::M_ptr)(s[i],s[k])-(*global::M_ptr)(s[k],s[i])};
        }
    }
    else {
        if (i<j) {
            if (i<l and l<=j+1 and j+1<k)
                return {i,(unsigned short)(j+1),(*global::M_ptr)(s[k],s[i])-(*global::M_ptr)(s[i],s[k])};
            if (l<=i and i<k and k<j)
                return {(unsigned short)(i+1),j,(*global::M_ptr)(s[i],s[k])-(*global::M_ptr)(s[k],s[i])};
        }
        else {
            if (j<=l and l<=i and i<k)
                return {(unsigned short)(i+1),j,(*global::M_ptr)(s[i],s[k])-(*global::M_ptr)(s[k],s[i])};
            if (l<=j and j<k and k<i)
                return {i,(unsigned short)(j+1),(*global::M_ptr)(s[k],s[i])-(*global::M_ptr)(s[i],s[k])};
        }
    }

    return {0,0,0};
}

long long explore_nn(solution& s, const matrix& M_diff) {
    const long long threshold = -10000;
    const std::vector<neighbor> neighbors = best_neighbors(M_diff,threshold);

    for (std::size_t n1 = 0; n1<neighbors.size()-1 and neighbors[n1].d+neighbors[n1+1].d>=threshold and global::n_evals<global::max_evals; ++n1) {
        for (std::size_t n2 = n1+1; n2<neighbors.size() and global::n_evals<global::max_evals; ++n2) {
            long long diff_sum = neighbors[n1].d+neighbors[n2].d;
            if (diff_sum<threshold) break;

            neighbor adj = adjustment(s,neighbors[n1].i,neighbors[n1].j,neighbors[n2].i,neighbors[n2].j);

            if (adj.d>-diff_sum) {
                insert::move(s,neighbors[n2].i,neighbors[n2].j);
                insert::move(s,adj.i,adj.j);
                return diff_sum+adj.d;
            }

            /*adj = adjustment(s,neighbors[n2].i,neighbors[n2].j,neighbors[n1].i,neighbors[n1].j);

            if (adj.d>-diff_sum) {
                insert::move(s,neighbors[n1].i,neighbors[n1].j);
                insert::move(s,adj.i,adj.j);
                return diff_sum+adj.d;
            }*/
        }
    }

    return 0;
}

long long extended_reach2(solution& s, const unsigned int& max_starts) {
    long long best_cost = s.cost();
    global::n_jumps = 0;
    global::n_evals = 0;
    global::n_starts = 0;
    global::n_local_optima = 0;
    global::consec_local_optima = 0;
    global::n_plateaus = 0;

    matrix M_diff(s.size());

    for (;global::n_starts<max_starts and global::n_evals<global::max_evals; ++global::n_starts) {
        solution curr_solution;
        long long curr_cost = curr_solution.cost();

        while (global::n_evals<global::max_evals) {
            curr_cost += local_search_er(curr_solution,M_diff);
            long long diff = explore_nn(curr_solution,M_diff);

            if (diff>0) curr_cost += diff;
            else break;
        }

        if (curr_cost>best_cost) {
            best_cost = curr_cost;
            s = std::move(curr_solution);
        }
    }

    return best_cost;
}