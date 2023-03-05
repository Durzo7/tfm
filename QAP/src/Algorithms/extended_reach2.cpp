#include <array>
#include <vector>
#include <algorithm>
#include <iostream>

#include "extended_reach2.hpp"
#include "global.hpp"
#include "two_exchange.hpp"
#include "shuffler.hpp"
#include "neighbor.hpp"
#include "matrix.hpp"
#include "local_search.hpp"

long long local_search_er(solution& s, matrix& D, neighbor& mv) {
    long long diff = 0;

    while (global::n_evals<global::max_evals) {
        mv = update_matrix(s,D,mv);

        if (mv.d<0) diff += mv.d;
        else break;
    }

    return diff;
}

std::vector<neighbor> best_neighbors(const matrix& D, const long long& threshold) {
    std::vector<neighbor> neighbors;

    for (unsigned short i = 0; i<D.size()-1; ++i) {
        for (unsigned short j = i+1; j<D.size(); ++j) {
            if (D(i,j)<=threshold) neighbors.emplace_back(i,j,D(i,j));
        }
    }
    std::sort(neighbors.begin(),neighbors.end());

    return neighbors;
}

long long adjustment(
    const solution& s,
    const unsigned short& i,
    const unsigned short& j,
    const unsigned short& k,
    const unsigned short& l
) {
    ++global::n_evals;
    const matrix& A = *global::A_ptr;
    const matrix& B = *global::B_ptr;
    long long dd = 0;

    dd += (B(i,k)-B(i,l))*(A(s[j],s[l])-A(s[i],s[l])-A(s[j],s[k])+A(s[i],s[k]));
    dd += (B(k,i)-B(l,i))*(A(s[l],s[j])-A(s[l],s[i])-A(s[k],s[j])+A(s[k],s[i]));

    dd += (B(j,k)-B(j,l))*(A(s[i],s[l])-A(s[j],s[l])-A(s[i],s[k])+A(s[j],s[k]));
    dd += (B(k,j)-B(l,j))*(A(s[l],s[i])-A(s[l],s[j])-A(s[k],s[i])+A(s[k],s[j]));

    return dd;
}

void explore_nn(solution& s, matrix& D, neighbor& mv) {
    const long long threshold = 40000;
    const std::vector<neighbor> neighbors = best_neighbors(D,threshold);
    long long counter = 0;

    for (std::size_t n1 = 0; n1<neighbors.size()-1 and neighbors[n1].d+neighbors[n1+1].d<=threshold and global::n_evals<global::max_evals; ++n1) {
        const unsigned short i = neighbors[n1].i, j = neighbors[n1].j;

        for (std::size_t n2 = n1+1; n2<neighbors.size() and global::n_evals<global::max_evals; ++n2) {
            long long diff_sum = neighbors[n1].d+neighbors[n2].d;
            if (diff_sum>threshold) break;
            ++counter;
            const unsigned short k = neighbors[n2].i, l = neighbors[n2].j;

            if (i!=k and i!=l and j!=k and j!=l) {
                long long adj = adjustment(s,i,j,k,l);

                if (adj<-diff_sum) {
                    ++global::n_jumps;
                    two_exchange::move(s,i,j);
                    mv.i = i; mv.j = j;
                    mv = update_matrix(s,D,mv);
                    mv.d += neighbors[n1].d;
                    return;
                }
            }
        }
    }
    //std::cout << neighbors.size() << ' ' << counter << std::endl;
}

long long extended_reach2(solution& s, const unsigned int& max_starts) {
    long long best_cost = s.cost();
    global::n_jumps = 0;
    global::n_evals = 0;
    global::n_starts = 0;
    global::n_local_optima = 0;
    global::consec_local_optima = 0;
    global::n_plateaus = 0;

    matrix D(s.size());

    for (;global::n_starts<max_starts and global::n_evals<global::max_evals; ++global::n_starts) {
        solution curr_solution;
        neighbor mv = initialize_matrix(curr_solution,D);
        long long curr_cost = curr_solution.cost();

        while (global::n_evals<global::max_evals) {
            if (mv.d<0) curr_cost += local_search_er(curr_solution,D,mv);
            if (global::n_evals<global::max_evals) explore_nn(curr_solution,D,mv);
            else mv = {0,0,0};

            if (mv.d<0) curr_cost += mv.d;
            else break;
        }

        if (curr_cost<best_cost) {
            best_cost = curr_cost;
            s = std::move(curr_solution);
        }
    }

    return best_cost;
}