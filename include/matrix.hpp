#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <array>

template <typename T, size_t m, size_t n>
using elements = std::array<std::array<T, m>, n>;

template <typename T, size_t n>
using square_elements = elements<T, n, n>;

// m * n Matrix
template <typename T, size_t m, size_t n>
class Matrix {
public:
    Matrix()
    {}

    Matrix(elements<T, m, n>& _el) :
        el(_el) // Copy the elements of the matrix
    {}

    T& at(size_t i, size_t j) {
        return el[i][j];
    }

protected:
     elements<T, m, n> el; // elements of the matrix
};

// n * n matrix
template <typename T, size_t n>
class SquareMatrix : protected Matrix<T, n, n> {
public:
    SquareMatrix inverse() const
    {
        SquareMatrix result;

        // For Chris
    }
};

#endif
