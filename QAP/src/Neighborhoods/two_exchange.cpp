#include "two_exchange.hpp"
#include "global.hpp"

long long two_exchange::difference(
    const solution& s,
    const unsigned short& i,
    const unsigned short& j
) {
    const matrix& A = *global::A_ptr;
    const matrix& B = *global::B_ptr;
    long long d = (B(i,j)-B(j,i))*(A(s[j],s[i])-A(s[i],s[j]));

    for (unsigned short k = 0; k<i; ++k, ++global::n_evals) {
        d += B(i,k)*(A(s[j],s[k])-A(s[i],s[k]))+B(k,i)*(A(s[k],s[j])-A(s[k],s[i]));
        d += B(j,k)*(A(s[i],s[k])-A(s[j],s[k]))+B(k,j)*(A(s[k],s[i])-A(s[k],s[j]));
    }
    for (unsigned short k = i+1; k<j; ++k, ++global::n_evals) {
        d += B(i,k)*(A(s[j],s[k])-A(s[i],s[k]))+B(k,i)*(A(s[k],s[j])-A(s[k],s[i]));
        d += B(j,k)*(A(s[i],s[k])-A(s[j],s[k]))+B(k,j)*(A(s[k],s[i])-A(s[k],s[j]));
    }
    for (unsigned short k = j+1; k<s.size(); ++k, ++global::n_evals) {
        d += B(i,k)*(A(s[j],s[k])-A(s[i],s[k]))+B(k,i)*(A(s[k],s[j])-A(s[k],s[i]));
        d += B(j,k)*(A(s[i],s[k])-A(s[j],s[k]))+B(k,j)*(A(s[k],s[i])-A(s[k],s[j]));
    }

    return d;
}

long long two_exchange::delta_difference(
    const solution& s,
    const unsigned short& i,
    const unsigned short& j,
    const unsigned short& k,
    const unsigned short& l
) {
    ++global::n_evals;
    const matrix& A = *global::A_ptr;
    const matrix& B = *global::B_ptr;
    long long dd = 0;

    dd += (B(i,k)-B(i,l))*(A(s[j],s[k])-A(s[i],s[k])-A(s[j],s[l])+A(s[i],s[l]));
    dd += (B(k,i)-B(l,i))*(A(s[k],s[j])-A(s[k],s[i])-A(s[l],s[j])+A(s[l],s[i]));

    dd += (B(j,k)-B(j,l))*(A(s[i],s[k])-A(s[j],s[k])-A(s[i],s[l])+A(s[j],s[l]));
    dd += (B(k,j)-B(l,j))*(A(s[k],s[i])-A(s[k],s[j])-A(s[l],s[i])+A(s[l],s[j]));

    return dd;
}

void two_exchange::move(
    solution& s,
    unsigned short i,
    unsigned short j
) {
    unsigned short aux = s[i];
    s[i] = s[j]; s[j] = aux;
}

