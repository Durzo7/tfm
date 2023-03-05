#include "two_opt.hpp"
#include "global.hpp"

long long two_opt::difference(
    const solution& s,
    const unsigned short& u,
    const unsigned short& v
) {
    ++global::n_evals;
    return (*global::M_ptr)(s[u-1],s[v])+(*global::M_ptr)(s[u],s[v+1])-
           (*global::M_ptr)(s[u-1],s[u])-(*global::M_ptr)(s[v],s[v+1]);
}

void two_opt::move(
    solution& s,
    unsigned short u,
    unsigned short v
) {
    for (; u<v; ++u, --v) {
        unsigned short aux = s[u];
        s[u] = s[v]; s[v] = aux;
    }
}

