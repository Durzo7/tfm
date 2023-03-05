#include "local_search.hpp"
#include "global.hpp"
#include "two_exchange.hpp"


neighbor initialize_matrix(solution& s, matrix& D) {
    neighbor best_neig(0,0,0);

    for (unsigned short i = 0; i<s.size()-1; ++i) {
        for (unsigned short j = i+1; j<s.size(); ++j) {
            D(i,j) = two_exchange::difference(s,i,j);
            
            if (D(i,j)<best_neig.d) best_neig = neighbor(i,j,D(i,j));
        }
    }

    if (best_neig.d<0) two_exchange::move(s,best_neig.i,best_neig.j);
    else ++global::n_local_optima;

    return best_neig;
}

neighbor update_matrix(solution& s, matrix& D, const neighbor& mv) {
    neighbor best_neig(0,0,0);

    for (unsigned short i = 0; i<s.size()-1; ++i) {
        for (unsigned short j = i+1; j<s.size(); ++j) {
            if (i==mv.i or i==mv.j or j==mv.i or j==mv.j) D(i,j) = two_exchange::difference(s,i,j);
            else D(i,j) += two_exchange::delta_difference(s,i,j,mv.i,mv.j);

            if (D(i,j)<best_neig.d) best_neig = neighbor(i,j,D(i,j));
        }
    }

    if (best_neig.d<0) two_exchange::move(s,best_neig.i,best_neig.j);
    else ++global::n_local_optima;

    return best_neig;
}

long long local_search(solution& s, matrix& D) {
    neighbor last_move = initialize_matrix(s,D);
    long long diff = last_move.d;

    if (diff<0) {
        while (global::n_evals<global::max_evals) {
            last_move = update_matrix(s,D,last_move);

            if (last_move.d<0) diff += last_move.d;
            else break;
        }
    }
  
    return diff;
}