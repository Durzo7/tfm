#include "swap.hpp"
#include "global.hpp"

long long swap::difference(
    const solution& s,
    unsigned short i,
    unsigned short j
) {
    long long diff = 0;

    if (i>j) {
        unsigned short aux = i;
        i = j; j = aux;
    }

    for (unsigned short k = i+1; k<j; ++k, ++global::n_evals) {
        diff += (*global::M_ptr)(s[j],s[k])-(*global::M_ptr)(s[k],s[j]);
        diff += (*global::M_ptr)(s[k],s[i])-(*global::M_ptr)(s[i],s[k]);
    }
    diff += (*global::M_ptr)(s[j],s[i])-(*global::M_ptr)(s[i],s[j]);

    return diff;
}

void swap::move(
    solution& s,
    const unsigned short& i,
    const unsigned short& j
) {
    unsigned short aux = s[i];
    s[i] = s[j]; s[j] = aux;
}