#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <array>

// m * n Matrix
template <typename T, size_t m, size_t n>
class Matrix {
public:
    T& at(size_t i, size_t j)
    {
        return el[i][j];
    }

private:
    std::array<std::array<T, m>, n> el; // elements of the matrix
};

#endif
