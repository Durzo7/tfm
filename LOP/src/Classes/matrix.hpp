#pragma once

class matrix{
    private:
        const unsigned short m_n;
        int** m_matrix;

    public:
        matrix(const unsigned short& n);
        matrix(const matrix& other) = delete;
        matrix(matrix&& other) noexcept;
        ~matrix();

        matrix& operator=(const matrix& other) = delete;
        matrix& operator=(matrix&& other) = delete;

        int* const& operator[](const unsigned short& row);
        const int* const& operator[](const unsigned short& row) const;

        int& operator()(const unsigned short& row, const unsigned short& column);
        const int& operator()(const unsigned short& row, const unsigned short& column) const;

        const unsigned short& size() const;
};