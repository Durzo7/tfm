#include "neighbor.hpp"

neighbor::neighbor(unsigned short i, unsigned short j, long long d): i(i), j(j), d(d) {}

bool neighbor::operator<(const neighbor& other) { return d<other.d; }