#ifndef TABLEAU_HPP
#define TABLEAU_HPP

#include <iostream>
#include <array>

template <size_t m>
using Index_Set = std::array<size_t, m>;

template <typename T, size_t m, size_t n>
using Simplex_Rows = std::array<std::array<T, n + 1>, m + 1>;

const static char delimiter = '\t';

// m rows of the simplex tableau, n variables
template <typename T, size_t m, size_t n>
class Tableau {
public:

    Tableau(Index_Set<m> _basic_vars, Simplex_Rows<T, m, n> _rows) :
        basic_vars(_basic_vars),
        rows(_rows)
    {}

    void print(std::ostream& os)
    {
        os << "BVs" << delimiter;
        for (size_t i = 1; i <= n; i++) {
            os << "x" << i << delimiter;
        }
        os << "RHS" << std::endl;

        for (size_t i = 0; i <= m; i++) {
            if (i == 0)
                os << "z";
            else
                os << "x" << basic_vars[i - 1];
            os << delimiter;

            for (size_t j = 0; j <= n; j++) {
                os << rows[i][j] << delimiter;
            }

            os << std::endl;
        }
    }

    // Returns -1 if no column can be pivoted on
    size_t choose_pivot_column()
    {
        T largest = 0;
        size_t pivot = -1;

        for (size_t i = 0; i < n; i++) {
            if (rows[0][i] > largest && rows[0][i] > 0) {
                largest = rows[0][i];
                pivot = i;
            }
        }

        return pivot;
    }

    // Returns -1 if no row can be pivoted on
    size_t choose_pivot_row(size_t column)
    {
        bool found_positive = false;
        T smallest = 0;
        size_t pivot = -1;

        // From 1 since we ignore the objective row
        for (size_t i = 1; i < m; i++) {
            T ratio = rows[i][m] / rows[i][column];
            if (ratio > 0) {
                if (!found_positive || ratio < smallest) {
                    found_positive = true;
                    smallest = ratio;
                    pivot = i;
                }
            }
        }

        return pivot;
    }

private:
    Index_Set<m> basic_vars;
    Simplex_Rows<T, m, n> rows;
};

#endif
