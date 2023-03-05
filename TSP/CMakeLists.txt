cmake_minimum_required(VERSION 3.10)

project(tfm_tsp)

include_directories(src src/Algorithms src/Classes src/Neighborhoods src/Parsers src/Utilities)

set(SOURCES
    src/main.cpp
    src/global.cpp
    src/Algorithms/local_search.cpp
    src/Algorithms/multistart.cpp
    src/Algorithms/extended_reach.cpp
    src/Classes/matrix.cpp
    src/Classes/solution.cpp
    src/Classes/neighbor.cpp
    src/Classes/shuffler.cpp
    src/Neighborhoods/two_opt.cpp
    src/Parsers/parser.cpp
    src/Utilities/rng.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
