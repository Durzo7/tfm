cmake_minimum_required(VERSION 3.10)

project(tfm_lop)

include_directories(src src/Algorithms src/Classes src/Neighborhoods src/Parsers src/Utilities)

set(SOURCES
    src/main.cpp
    src/global.cpp
    src/Algorithms/local_search.cpp
    src/Algorithms/multistart.cpp
    src/Algorithms/extended_reach.cpp
    src/Algorithms/extended_reach2.cpp
    src/Algorithms/ILS.cpp
    src/Algorithms/VNS.cpp
    src/Classes/matrix.cpp
    src/Classes/solution.cpp
    src/Classes/neighbor.cpp
    src/Classes/shuffler.cpp
    src/Neighborhoods/insert.cpp
    src/Neighborhoods/swap.cpp
    src/Parsers/parser.cpp
    src/Utilities/rng.cpp
    src/Utilities/shake.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
