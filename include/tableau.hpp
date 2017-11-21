#ifndef TABLEAU_HPP
#define TABLEAU_HPP

#include <iostream>
#include <vector>

// m rows of the simplex tableau, n variables
template <typename T>
class Tableau {
public:

    const static char delimiter = '\t';
    const static char var_name = 'x';

    using Index_Set = std::vector<unsigned>;
    using Simplex_Rows = std::vector<std::vector<T>>;

    Tableau(Index_Set _basic_vars, Simplex_Rows _rows) :
        basic_vars(_basic_vars),
        rows(_rows)
    {}

    Tableau(std::istream& is, unsigned _m, unsigned _n) :
        m(_m),
        n(_n),
        basic_vars(m),
        rows(m + 1)
    {
        for (int i = 0; i <= m; i++) {
            rows[i].resize(n + 1);
        }

        std::string tmp;

        // Ignore the first row
        for (unsigned i = 0; i <= n + 1; i++)
            is >> tmp;

        for (unsigned i = 0; i <= m; i++) {
            is >> tmp; // First column tells us the index set, after row 0
            if (i != 0) {
                std::string bv = tmp.substr(tmp.find(var_name) + 1);
                basic_vars[i - 1] = std::stoi(bv);
            }
            for (unsigned j = 0; j <= n; j++) {
                is >> tmp;
                rows[i][j] = T(tmp);
            }
        }
    }

    void print(std::ostream& os)
    {
        os << "BVs" << delimiter;
        for (unsigned i = 1; i <= n; i++) {
            os << "x" << i << delimiter;
        }
        os << "RHS" << std::endl;

        for (unsigned i = 0; i <= m; i++) {
            if (i == 0)
                os << "z";
            else
                os << "x" << basic_vars[i - 1];
            os << delimiter;

            for (unsigned j = 0; j <= n; j++) {
                os << rows[i][j] << delimiter;
            }

            os << std::endl;
        }
    }

/*
\begin{equation*}
		\begin{array}{c|cccc|c}
    		\text{BVs} & x_1 & x_2 & s_1 & s_2 & RHS \\ \hline
    		z & 30 & 45 & 0 & 0 & 0 \\
    		s_1 & 48 & 21 & 1 & 0 & 2000  \\
    		s_2 & 5 & 4 & 0 & 1 & 200 \\
		\end{array}
\end{equation*}
*/
    void print_latex(std::ostream& os, unsigned pivot_r, unsigned pivot_c)
    {
        os << "\t\\hline" << std::endl;
        for (unsigned i = 0; i <= m; i++) {
            os << "\t\t";
            if (i == 0)
                os << "z";
            else
                os << "x_" << basic_vars[i - 1];
            for (unsigned j = 0; j <= n; j++) {
                os << " & ";
                if (pivot_r != 0 && pivot_r == i && pivot_c == j) {
                    os << "\\mathbf{";
                    rows[i][j].print_latex(os);
                    os << "}";
                } else {
                    rows[i][j].print_latex(os);
                }
            }

            os << " \\\\ " << std::endl;
        }
    }

    unsigned choose_pivot_column()
    {
        T largest = 0;
        unsigned pivot = 0;

        for (unsigned i = 0; i < n; i++) {
            if (rows[0][i] > largest && rows[0][i] > 0) {
                largest = rows[0][i];
                pivot = i;
            }
        }

        return pivot;
    }

    unsigned choose_pivot_row(unsigned column)
    {
        bool found_positive = false;
        T smallest = 0;
        unsigned pivot = 0;

        // From 1 since we ignore the objective row
        for (unsigned i = 1; i < m; i++) {
            T ratio = rows[i][m] / rows[i][column];
            if (ratio > 0 && (!found_positive || ratio < smallest)) {
                found_positive = true;
                smallest = ratio;
                pivot = i;
            }
        }

        return pivot;
    }

    void pivot_on(unsigned row, unsigned column)
    {
        T divisor = rows[row][column];
        // Divide the pivot row by the pivot element
        for (unsigned i = 0; i <= n; i++) {
            rows[row][i] /= divisor;
        }

        // For each other row, subtract the required amount using row operations
        for (unsigned i = 0; i <= m; i++) {
            if (i == row)
                continue;
            T ratio = rows[i][column];
            std::cerr << ratio << std::endl;
            for (unsigned j = 0; j <= n; j++) {
                rows[i][j] -= ratio * rows[row][j];
            }
        }

        // Update the index set
        basic_vars[row - 1] = column + 1;
    }

private:
    unsigned m;
    unsigned n;

    Index_Set basic_vars;
    Simplex_Rows rows;
};

#endif
