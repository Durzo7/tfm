#pragma once

struct neighbor {
    unsigned short i;
    unsigned short j;
    long long d;

    neighbor(unsigned short i, unsigned short j, long long d);
    bool operator<(const neighbor& other);
};