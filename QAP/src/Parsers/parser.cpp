#include <fstream>
#include <sstream>

#include "parser.hpp"

std::pair<matrix,matrix> load(std::string filepath) {
    std::ifstream file_stream(filepath);
    unsigned short n; file_stream >> n;
    matrix A(n), B(n);

    for (unsigned short i = 0; i<n; ++i) {
        for (unsigned short j = 0; j<n; ++j) file_stream >> A(i,j);
    }

    for (unsigned short i = 0; i<n; ++i) {
        for (unsigned short j = 0; j<n; ++j) file_stream >> B(i,j);
    }

    return std::make_pair(std::move(A),std::move(B));
}