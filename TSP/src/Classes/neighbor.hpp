#pragma once

struct neighbor {
    unsigned short u;
    unsigned short v;
    long long d;

    neighbor(unsigned short u, unsigned short v, long long d);
    bool operator<(const neighbor& other);
};