#pragma once

#include "matrix.hpp"

class solution {
    private:
        static unsigned short c_n;
        static const matrix* c_A_ptr;
        static const matrix* c_B_ptr;

        unsigned short* m_perm;

        void shuffle();

    public:
        static void set_matrices(const matrix& A, const matrix& B);
        static const unsigned short& size();

        solution();
        solution(const solution& other);
        solution(solution&& other) = delete;
        ~solution();

        solution& operator=(const solution& other) = delete;
        solution& operator=(solution&& other) noexcept;

        unsigned short& operator[](const unsigned short& i);
        const unsigned short& operator[](const unsigned short& i) const;

        long long cost() const;
};