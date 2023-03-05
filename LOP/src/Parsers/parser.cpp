#include <fstream>
#include <sstream>

#include "parser.hpp"

matrix load(std::string filepath) {
    std::ifstream file_stream(filepath);
    std::string line; std::getline(file_stream,line);
    std::istringstream line_stream(line);
    unsigned short n; line_stream >> n;
    matrix M(n);

    for (unsigned short i = 0; i<n; ++i) {
        std::getline(file_stream,line);
        line_stream = std::istringstream(line);

        for (unsigned short j = 0; j<n; ++j) line_stream >> M(i,j);
    }

    return M;
}