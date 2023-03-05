#include <numeric>
#include <cstring>

#include "solution.hpp"
#include "global.hpp"
#include "rng.hpp"

unsigned short solution::c_n;
const matrix* solution::c_A_ptr;
const matrix* solution::c_B_ptr;

void solution::set_matrices(const matrix& A, const matrix& B) {
    c_n = A.size();
    c_A_ptr = &A;
    c_B_ptr = &B;
}

const unsigned short& solution::size() {
    return c_n;
}

solution::solution(): m_perm(new unsigned short[c_n]) {
    std::iota(m_perm,m_perm+c_n,0);
    shuffle();
}

solution::solution(const solution& other): m_perm(new unsigned short[c_n]) {
    std::memcpy(m_perm,other.m_perm,sizeof(unsigned short)*c_n);
}

solution::~solution() {
    if (m_perm!=nullptr) delete[] m_perm;
}

solution& solution::operator=(solution&& other) noexcept {
    if (this!=&other) {
        delete[] m_perm;
        m_perm = other.m_perm;
        other.m_perm = nullptr;
    }

    return *this;
}

unsigned short& solution::operator[](const unsigned short& i) {
    return m_perm[i];
}

const unsigned short& solution::operator[](const unsigned short& i) const {
    return m_perm[i%c_n];
}

void solution::shuffle() {
    for (unsigned short i = c_n-1; i>0; --i) {
        unsigned short rd_indx = rng::random_uint(i);
        unsigned short aux = m_perm[i];
        m_perm[i] = m_perm[rd_indx];
        m_perm[rd_indx] = aux;
    }
}

long long solution::cost() const {
    long long c = 0;

    for (unsigned short i = 0; i<c_n; ++i) {
        for (unsigned short j = 0; j<i; ++j, ++global::n_evals)
            c += (*c_B_ptr)(i,j)*(*c_A_ptr)(m_perm[i],m_perm[j]);

        for (unsigned short j = i+1; j<c_n; ++j, ++global::n_evals)
            c += (*c_B_ptr)(i,j)*(*c_A_ptr)(m_perm[i],m_perm[j]);
    }

    return c;
}