#include <array>
#include <vector>
#include <algorithm>
#include <iostream>

#include "extended_reach.hpp"
#include "global.hpp"
#include "insert.hpp"
#include "shuffler.hpp"
#include "neighbor.hpp"
#include "matrix.hpp"

matrix difference_matrix(const solution& s) {
    matrix M(s.size());

    for (unsigned short i = 0; i<s.size(); ++i) {
        long long diff = 0;
        for (unsigned short j = i-1; j<i; --j) {
            diff += (*global::M_ptr)(s[i],s[j])-(*global::M_ptr)(s[j],s[i]);
            M(i,j) = diff;
        }

        diff = 0;
        for (unsigned short j = i+1; j<s.size(); ++j) {
            diff += (*global::M_ptr)(s[j],s[i])-(*global::M_ptr)(s[i],s[j]);
            M(i,j) = diff;
        }
    }

    return M;
}

long long explore_close_neighbors_obi(
    solution& s,
    std::vector<neighbor>& neighbors
) {
    long long best_diff = 0;
    unsigned short bi, bj;

    for (unsigned short i = 0; i<s.size(); ++i) {
        long long new_diff = 0;
        for (unsigned short j = i-1; j<i; --j, ++global::n_evals) {
            new_diff += (*global::M_ptr)(s[i],s[j]);
            new_diff -= (*global::M_ptr)(s[j],s[i]);

            if (new_diff>best_diff) {
                best_diff = new_diff;
                bi = i; bj = j;
            }
            else if (best_diff==0 and j+1!=i) neighbors.emplace_back(i,j,new_diff);
        }

        new_diff = 0;
        for (unsigned short j = i+1; j<s.size(); ++j, ++global::n_evals) {
            new_diff += (*global::M_ptr)(s[j],s[i]);
            new_diff -= (*global::M_ptr)(s[i],s[j]);

            if (new_diff>best_diff) {
                best_diff = new_diff;
                bi = i; bj = j;
            }
            else if (best_diff==0) neighbors.emplace_back(i,j,new_diff);
        }
    }

    if (best_diff>0) {
        global::prev_local_optima = false;
        insert::move(s,bi,bj);
    }
    else {
        ++global::n_local_optima;
        if (global::prev_local_optima) ++global::consec_local_optima;
        else global::prev_local_optima = true;
    }

    return best_diff;
}

double quality(const long long& diff) {
    double z = (double)(-diff)/10000.0;
    return (z*z)/2.0-z+0.5;
}


long long explore_far_neighbors_obi(
    solution& s,
    const std::vector<neighbor>& neighbors
) {
    unsigned short k_best = s.size();
    const matrix diff_M = std::move(difference_matrix(s));

    for (unsigned short k = 0; k<k_best and global::n_evals<global::max_evals; ++k) {
        long long best_diff = 0;
        unsigned short bi, bj;

        if (neighbors[k].i<neighbors[k].j) {
            for (unsigned short i = 0; i<neighbors[k].i; ++i) {
                for (unsigned short j = neighbors[k].i; j<neighbors[k].j; ++j, ++global::n_evals) {
                    long long new_diff = neighbors[k].d+diff_M(i,j+1);
                    new_diff += (*global::M_ptr)(s[i],s[neighbors[k].i]);
                    new_diff -= (*global::M_ptr)(s[neighbors[k].i],s[i]);

                    if (new_diff>best_diff) {
                        best_diff = new_diff;
                        bi = i; bj = j;
                    }
                }
            }

            for (unsigned short i = neighbors[k].i; i<neighbors[k].j; ++i) {
                for (unsigned short j = 0; j<neighbors[k].i; ++j, ++global::n_evals) {
                    long long new_diff = neighbors[k].d+diff_M(i+1,j);
                    new_diff += (*global::M_ptr)(s[neighbors[k].i],s[i+1]);
                    new_diff -= (*global::M_ptr)(s[i+1],s[neighbors[k].i]);

                    if (new_diff>best_diff) {
                        best_diff = new_diff;
                        bi = i; bj = j;
                    }
                }
                for (unsigned short j = neighbors[k].j; j<s.size(); ++j, ++global::n_evals) {
                    long long new_diff = neighbors[k].d+diff_M(i+1,j);
                    new_diff += (*global::M_ptr)(s[neighbors[k].i],s[i+1]);
                    new_diff -= (*global::M_ptr)(s[i+1],s[neighbors[k].i]);

                    if (new_diff>best_diff) {
                        best_diff = new_diff;
                        bi = i; bj = j;
                    }
                }
            }

            for (unsigned short i = neighbors[k].j+1; i<s.size(); ++i) {
                for (unsigned short j = neighbors[k].i+1; j<=neighbors[k].j; ++j, ++global::n_evals) {
                    long long new_diff = neighbors[k].d+diff_M(i,j+1);
                    new_diff += (*global::M_ptr)(s[i],s[neighbors[k].i]);
                    new_diff -= (*global::M_ptr)(s[neighbors[k].i],s[i]);

                    if (new_diff>best_diff) {
                        best_diff = new_diff;
                        bi = i; bj = j;
                    }
                }
            }
        }
        else {
            for (unsigned short i = 0; i<neighbors[k].j; ++i) {
                for (unsigned short j = neighbors[k].j; j<neighbors[k].i; ++j, ++global::n_evals) {
                    long long new_diff = neighbors[k].d+diff_M(i,j-1);
                    new_diff += (*global::M_ptr)(s[neighbors[k].i],s[i]);
                    new_diff -= (*global::M_ptr)(s[i],s[neighbors[k].i]);

                    if (new_diff>best_diff) {
                        best_diff = new_diff;
                        bi = i; bj = j;
                    }
                }
            }

            for (unsigned short i = neighbors[k].j+1; i<=neighbors[k].i; ++i) {
                for (unsigned short j = 0; j<=neighbors[k].j; ++j, ++global::n_evals) {
                    long long new_diff = neighbors[k].d+diff_M(i-1,j);
                    new_diff += (*global::M_ptr)(s[i-1],s[neighbors[k].i]);
                    new_diff -= (*global::M_ptr)(s[neighbors[k].i],s[i-1]);

                    if (new_diff>best_diff) {
                        best_diff = new_diff;
                        bi = i; bj = j;
                    }
                }
                for (unsigned short j = neighbors[k].i+1; j<s.size(); ++j, ++global::n_evals) {
                    long long new_diff = neighbors[k].d+diff_M(i-1,j);
                    new_diff += (*global::M_ptr)(s[i-1],s[neighbors[k].i]);
                    new_diff -= (*global::M_ptr)(s[neighbors[k].i],s[i-1]);

                    if (new_diff>best_diff) {
                        best_diff = new_diff;
                        bi = i; bj = j;
                    }
                }
            }

            for (unsigned short i = neighbors[k].i+1; i<s.size(); ++i) {
                for (unsigned short j = neighbors[k].j+1; j<=neighbors[k].i; ++j, ++global::n_evals) {
                    long long new_diff = neighbors[k].d+diff_M(i,j-1);
                    new_diff += (*global::M_ptr)(s[neighbors[k].i],s[i]);
                    new_diff -= (*global::M_ptr)(s[i],s[neighbors[k].i]);

                    if (new_diff>best_diff) {
                        best_diff = new_diff;
                        bi = i; bj = j;
                    }
                }
            }
        }

        if (neighbors[k].d==0) {
            ++global::n_plateaus;
            if (best_diff==0) ++global::n_local_optima;
        }

        if (best_diff>0) {
            //std::cout << k << ' ';
            //std::cout << neighbors[k].d << ' ';
            //std::cout << quality(neighbors[k].d) << std::endl;
            ++global::n_jumps;
            insert::move(s,neighbors[k].i,neighbors[k].j);
            insert::move(s,bi,bj);
            return best_diff;
        }
    }

    return 0;
}

long long extended_reach(solution& s, const unsigned int& max_starts) {
    long long best_cost = s.cost();
    global::n_jumps = 0;
    global::n_evals = 0;
    global::n_starts = 0;
    global::n_local_optima = 0;
    global::consec_local_optima = 0;
    global::n_plateaus = 0;

    for (; global::n_starts<max_starts and global::n_evals<global::max_evals; ++global::n_starts) {
        solution curr_solution;
        long long curr_cost = curr_solution.cost();
        global::prev_local_optima = false;

        while (global::n_evals<global::max_evals) {
            std::vector<neighbor> neighbors;
            long long new_cost = curr_cost+explore_close_neighbors_obi(curr_solution,neighbors);

            if (not (new_cost>curr_cost) and global::n_evals<global::max_evals) {
                std::sort(neighbors.begin(),neighbors.end());
                new_cost = curr_cost+explore_far_neighbors_obi(curr_solution,neighbors);
            }

            if (new_cost>curr_cost) curr_cost = new_cost;
            else break;
        }

        if (curr_cost>best_cost) {
            best_cost = curr_cost;
            s = std::move(curr_solution);
        }
    }

    return best_cost;
}
