#include <cstring>

#include "matrix.hpp"

matrix::matrix(const unsigned short& n): m_n(n), m_matrix(new int*[m_n]) {
    for (unsigned short i = 0; i<m_n; ++i) {
        m_matrix[i] = new int[m_n];
        std::memset(m_matrix[i],0,sizeof(int)*m_n);
    }
}

matrix::matrix(matrix&& other) noexcept: m_n(other.m_n), m_matrix(other.m_matrix) {
    other.m_matrix = nullptr;
}

matrix::~matrix() {
    if (m_matrix!=nullptr) {
        for (unsigned short i = 0; i<m_n; ++i) delete[] m_matrix[i];
        delete[] m_matrix;
    }
}

int* const& matrix::operator[](const unsigned short& row) {
    return m_matrix[row];
}

const int* const& matrix::operator[](const unsigned short& row) const {
    return m_matrix[row];
}

int& matrix::operator()(const unsigned short& row, const unsigned short& column) {
    return m_matrix[row][column];
}

const int& matrix::operator()(const unsigned short& row, const unsigned short& column) const {
    return m_matrix[row][column];
}

const unsigned short& matrix::size() const {
    return m_n;
}