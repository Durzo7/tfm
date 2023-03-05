#include <vector>
#include <algorithm>
#include <iostream>

#include "extended_reach.hpp"
#include "local_search.hpp"
#include "global.hpp"
#include "shuffler.hpp"
#include "neighbor.hpp"
#include "two_exchange.hpp"


long long local_search_er(solution& s, matrix& D, neighbor last_move) {
    long long diff = 0;

    while (global::n_evals<global::max_evals) {
        last_move = update_matrix(s,D,last_move);

        if (last_move.d<0) diff += last_move.d;
        else break;
    }
    if (diff==0) ++global::consec_local_optima;
  
    return diff;
}

void initialize_neighbors(std::vector<neighbor>& neighbors, const matrix& D) {
    for (unsigned short i = 0; i<D.size()-1; ++i) {
        for (unsigned short j = i+1; j<D.size(); ++j) neighbors.emplace_back(i,j,D(i,j));
    }
    std::sort(neighbors.begin(),neighbors.end());
}

neighbor explore_far_neighbors(solution& s, matrix& D) {
    matrix aux_D(D.size());
    unsigned short k_best = s.size();
    std::vector<neighbor> neighbors; initialize_neighbors(neighbors,D);

    for (unsigned short k = 0; k<k_best and global::n_evals<global::max_evals; ++k) {
        solution curr_neig = s;
        two_exchange::move(curr_neig,neighbors[k].i,neighbors[k].j);
        neighbor best_nn(0,0,0);

        for (unsigned short i = 0; i<s.size()-1; ++i) {
            for (unsigned short j = i+1; j<s.size(); ++ j) {
                if (i==neighbors[k].i or i==neighbors[k].j or j==neighbors[k].i or j==neighbors[k].j) aux_D(i,j) = two_exchange::difference(curr_neig,i,j);
                else aux_D(i,j) = D(i,j)+two_exchange::delta_difference(curr_neig,i,j,neighbors[k].i,neighbors[k].j);
                long long new_diff = neighbors[k].d+aux_D(i,j);

                if (new_diff<best_nn.d) best_nn = neighbor(i,j,new_diff);
            }
        }

        if (neighbors[k].d==0) {
            ++global::n_plateaus;
            if (best_nn.d==0) ++global::n_local_optima;
        }

        if (best_nn.d<0) {
            //std::cout << k << std::endl;
            ++global::n_jumps;
            two_exchange::move(curr_neig,best_nn.i,best_nn.j);
            s = std::move(curr_neig);
            D = std::move(aux_D);
            return best_nn;
        }
    }

    return neighbor(0,0,0);
}

long long extended_reach(solution& s, const unsigned int& max_starts) {
    long long best_cost = s.cost();
    global::n_jumps = 0;
    global::n_evals = 0;
    global::n_starts = 0;
    global::n_local_optima = 0;
    global::consec_local_optima = 0;
    global::n_plateaus = 0;
    matrix D(s.size());

    for (; global::n_starts<max_starts and global::n_evals<global::max_evals; ++global::n_starts) {
        solution curr_solution;
        long long curr_cost = curr_solution.cost();

        if (global::n_evals<global::max_evals) {
            neighbor last_move = initialize_matrix(curr_solution,D);
            curr_cost += last_move.d;

            while (global::n_evals<global::max_evals) {
                if (last_move.d<0) curr_cost += local_search_er(curr_solution,D,last_move);
                last_move = global::n_evals<global::max_evals ? explore_far_neighbors(curr_solution,D) : neighbor(0,0,0);
                if (last_move.d<0) curr_cost += last_move.d;
                else break;
            }
        }

        if (curr_cost<best_cost) {
            best_cost = curr_cost;
            s = std::move(curr_solution);
        }
    }

    return best_cost;
}