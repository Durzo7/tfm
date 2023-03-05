#include "insert.hpp"
#include "global.hpp"

long long insert::difference(
    const solution& s,
    const unsigned short& i,
    const unsigned short& j
) {
    long long diff = 0;

    if (i<j) {
        for (unsigned short k = j; k>i; --k, ++global::n_evals) 
            diff += (*global::M_ptr)(s[k],s[i])-(*global::M_ptr)(s[i],s[k]);
    }
    else {
        for (unsigned short k = j; k<i; ++k, ++global::n_evals)
            diff += (*global::M_ptr)(s[i],s[k])-(*global::M_ptr)(s[k],s[i]);
    }

    return diff;
}

void insert::move(
    solution& s,
    const unsigned short& i,
    const unsigned short& j
) {
    unsigned short aux = s[i];

    if (i<j) {
        for (unsigned short k = i; k<j; ++k) s[k] = s[k+1];
    }
    else {
        for (unsigned short k = i; k>j; --k) s[k] = s[k-1];
    }

    s[j] = aux;
}