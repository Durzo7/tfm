#include <array>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>

#include "extended_reach.hpp"
#include "global.hpp"
#include "shuffler.hpp"
#include "neighbor.hpp"
#include "two_opt.hpp"


long long explore_close_neighbors(
    solution& curr_solution,
    std::vector<neighbor>& neighbors
) {
    long long best_diff = 0;
    unsigned short bu, bv;

    for (unsigned short u = 1; u<curr_solution.size()-1; ++u) {
        for (unsigned short v = u+1; v<curr_solution.size()-(u==1); ++v) {
            long long new_diff = two_opt::difference(curr_solution,u,v);

            if (new_diff<best_diff) {
                best_diff = new_diff;
                bu = u; bv = v;
            }
            else if (best_diff==0) neighbors.emplace_back(u,v,new_diff);
        }
    }

    if (best_diff<0) {
        global::prev_local_optima = false;
        two_opt::move(curr_solution,bu,bv);
    }
    else {
        ++global::n_local_optima;
        if (global::prev_local_optima) ++global::consec_local_optima;
        else global::prev_local_optima = true;
    }

    return best_diff;
}

long long explore_far_neighbors(
    solution& curr_solution,
    const std::vector<neighbor>& neighbors
) {
    unsigned short k_best = 2*curr_solution.size();

    for (unsigned short k = 0; k<k_best and global::n_evals<global::max_evals; ++k) {
        solution curr_neig = curr_solution;
        two_opt::move(curr_neig,neighbors[k].u,neighbors[k].v);
        long long best_diff = 0;
        unsigned short bu, bv;

        for (unsigned short u = 1; u<curr_solution.size()-1; ++u) {
            for (unsigned short v = u+1; v<curr_solution.size()-(u==1); ++v) {
                long long new_diff = neighbors[k].d+two_opt::difference(curr_neig,u,v);

                if (new_diff<best_diff) {
                    best_diff = new_diff;
                    bu = u; bv = v;
                }
            }
        }

        if (neighbors[k].d==0) {
            ++global::n_plateaus;
            if (best_diff==0) ++global::n_local_optima;
        }

        if (best_diff<0) {
            //std::cout << k << std::endl;
            ++global::n_jumps;
            two_opt::move(curr_neig,bu,bv);
            curr_solution = std::move(curr_neig);
            return best_diff;
        }
    }

    return 0;
}

double prob(double x) {
    if (x<-1) return std::pow(x+2.0,4)/24;
    else {
        double x3 = std::pow(x,3);
        double x4 = x3*x;
        return -x4/8-x3/3+2*x/3+0.5; 
    }
}

long long explore_far_neighbors_opt(
    solution& curr_solution,
    const std::vector<neighbor>& neighbors
) {
    unsigned short k_best = 2*curr_solution.size();

    for (unsigned short k = 0; k<k_best and global::n_evals<global::max_evals; ++k) {
        solution curr_neig = curr_solution;
        two_opt::move(curr_neig,neighbors[k].u,neighbors[k].v);
        long long best_diff = 0;
        unsigned short bu, bv;

        for (unsigned short u = 1; u<neighbors[k].u; ++u) {
            for (unsigned short v = neighbors[k].u-1+(u==neighbors[k].u-1); v<=neighbors[k].v; ++v) {
                long long new_diff = neighbors[k].d+two_opt::difference(curr_neig,u,v);

                if (new_diff<best_diff) {
                    best_diff = new_diff;
                    bu = u; bv = v;
                }
            }
        }

        for (unsigned short v = neighbors[k].u+1; v<curr_solution.size(); ++v) {
            long long new_diff = neighbors[k].d+two_opt::difference(curr_neig,neighbors[k].u,v);

            if (new_diff<best_diff) {
                best_diff = new_diff;
                bu = neighbors[k].u; bv = v;
            }
        }

        for (unsigned short u = neighbors[k].u+1; u<=neighbors[k].v; ++u) {
            for (unsigned short v = neighbors[k].v+(u==neighbors[k].v); v<curr_solution.size(); ++v) {
                long long new_diff = neighbors[k].d+two_opt::difference(curr_neig,u,v);

                if (new_diff<best_diff) {
                    best_diff = new_diff;
                    bu = u; bv = v;
                }
            }
        }

        for (unsigned short v = neighbors[k].v+2; v<curr_solution.size(); ++v) {
            long long new_diff = neighbors[k].d+two_opt::difference(curr_neig,neighbors[k].v+1,v);

            if (new_diff<best_diff) {
                best_diff = new_diff;
                bu = neighbors[k].v+1; bv = v;
            }
        }

        if (neighbors[k].d==0) {
            ++global::n_plateaus;
            if (best_diff==0) ++global::n_local_optima;
        }

        if (best_diff<0) {
            /*std::cout << k << ' ';
            std::cout << neighbors[k].d << ' ';
            std::cout << prob((double)neighbors[k].d/-10000.0) << std::endl;*/
            ++global::n_jumps;
            two_opt::move(curr_neig,bu,bv);
            curr_solution = std::move(curr_neig);
            return best_diff;
        }
    }

    return 0;
}

double newton_rapson_approx(const double& prob) {
    double x = -0.5;
    double x2 = x*x;
    double x3 = x2*x;
    double x4 = x3*x;
    double f = -x4/8.0-x3/3.0+2.0*x/3.0+0.5-prob;
    double fp = -x3/2.0-x2+2.0/3.0;
    double h = f/fp;

    while (std::abs(h)>0.000001) {
        x -= h;
        x2 = x*x;
        x3 = x2*x;
        x4 = x3*x;
        f = -x4/8.0-x3/3.0+2.0*x/3.0+0.5-prob;
        fp = -x3/2.0-x2+2.0/3.0;
        h = f/fp;
    }

    return x-h;
}

long long explore_far_neighbors_opt2(
    solution& curr_solution,
    const std::vector<neighbor>& neighbors
) {
    long long max_diff = newton_rapson_approx(0.25)*-10000;
    unsigned short k = -1;

    while (neighbors[++k].d<=max_diff) {
        solution curr_neig = curr_solution;
        two_opt::move(curr_neig,neighbors[k].u,neighbors[k].v);
        long long best_diff = 0;
        unsigned short bu, bv;

        for (unsigned short u = 1; u<neighbors[k].u; ++u) {
            for (unsigned short v = neighbors[k].u-1+(u==neighbors[k].u-1); v<=neighbors[k].v; ++v) {
                long long new_diff = neighbors[k].d+two_opt::difference(curr_neig,u,v);

                if (new_diff<best_diff) {
                    best_diff = new_diff;
                    bu = u; bv = v;
                }
            }
        }

        for (unsigned short v = neighbors[k].u+1; v<curr_solution.size(); ++v) {
            long long new_diff = neighbors[k].d+two_opt::difference(curr_neig,neighbors[k].u,v);

            if (new_diff<best_diff) {
                best_diff = new_diff;
                bu = neighbors[k].u; bv = v;
            }
        }

        for (unsigned short u = neighbors[k].u+1; u<=neighbors[k].v; ++u) {
            for (unsigned short v = neighbors[k].v+(u==neighbors[k].v); v<curr_solution.size(); ++v) {
                long long new_diff = neighbors[k].d+two_opt::difference(curr_neig,u,v);

                if (new_diff<best_diff) {
                    best_diff = new_diff;
                    bu = u; bv = v;
                }
            }
        }

        for (unsigned short v = neighbors[k].v+2; v<curr_solution.size(); ++v) {
            long long new_diff = neighbors[k].d+two_opt::difference(curr_neig,neighbors[k].v+1,v);

            if (new_diff<best_diff) {
                best_diff = new_diff;
                bu = neighbors[k].v+1; bv = v;
            }
        }

        if (neighbors[k].d==0) {
            ++global::n_plateaus;
            if (best_diff==0) ++global::n_local_optima;
        }

        if (best_diff<0) {
            /*std::cout << k << ' ';
            std::cout << neighbors[k].d << ' ';
            std::cout << prob((double)neighbors[k].d/-10000.0) << std::endl;*/
            ++global::n_jumps;
            two_opt::move(curr_neig,bu,bv);
            curr_solution = std::move(curr_neig);
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
            long long new_cost = curr_cost+explore_close_neighbors(curr_solution,neighbors);

            if (not (new_cost<curr_cost) and global::n_evals<global::max_evals) {
                std::sort(neighbors.begin(),neighbors.end());
                new_cost = curr_cost+explore_far_neighbors_opt(curr_solution,neighbors);
            }

            if (new_cost<curr_cost) curr_cost = new_cost;
            else break;
        }

        if (curr_cost<best_cost) {
            best_cost = curr_cost;
            s = std::move(curr_solution);
        }
    }

    return best_cost;
}