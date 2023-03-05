#include <numeric>

#include "shuffler.hpp"
#include "rng.hpp"

shuffler::shuffler(const unsigned short& n): m_pos(n), m_perm(new unsigned short[n]) {
    std::iota(m_perm,m_perm+n,0);
}

shuffler::shuffler(const unsigned short& n, const unsigned short& i) {
    unsigned short not_first = (unsigned short)(i!=0);
    m_pos = n-1-not_first;
    m_perm = new unsigned short[m_pos];
    std::iota(m_perm,m_perm+i-not_first,0);
    std::iota(m_perm+i-not_first,m_perm+m_pos,i+1);
}

shuffler::~shuffler() {
    delete[] m_perm;
}

const unsigned short& shuffler::next() {
    unsigned short rd_indx = rng::random_uint(--m_pos);
    unsigned short aux = m_perm[m_pos];
    m_perm[m_pos] = m_perm[rd_indx];
    m_perm[rd_indx] = aux;
    
    return m_perm[m_pos];
}

bool shuffler::empty() {
    return m_pos==0;
}