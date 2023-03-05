#include "neighbor.hpp"

neighbor::neighbor(unsigned short u, unsigned short v, long long d): u(u), v(v), d(d) {}

bool neighbor::operator<(const neighbor& other) { return d<other.d; }