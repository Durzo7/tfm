#include <iostream>
#include <string>

#include "global.hpp"
#include "parser.hpp"
#include "solution.hpp"
#include "rng.hpp"
#include "multistart.hpp"
#include "extended_reach.hpp"
#include "shuffler.hpp"


int main(int argc, char* argv[]) {
    const matrix M = std::move(load(argv[1]));
    solution::set_matrix(M);
    global::M_ptr = &M;
    global::max_evals = std::stoul(argv[2]);
    rng::engine.seed(std::stoul(argv[3]));

    solution s;
    long long c = extended_reach(s,global::max_evals);

    for (unsigned short i = 0; i<s.size(); ++i)
        std::cout << s[i] << ' ';
    std::cout << std::endl;

    std::cout << "Cost:\t\t" << c << std::endl;
    std::cout << "Starts:\t\t" << global::n_starts << std::endl;
    std::cout << "LO:\t\t" << global::n_local_optima << std::endl;
    std::cout << "Consec LO:\t" << global::consec_local_optima << std::endl;
    std::cout << "Plateaus:\t" << global::n_plateaus << std::endl;
    std::cout << "Jumps:\t\t" << global::n_jumps << std::endl;

    return 0;
}