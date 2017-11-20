#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "tableau.hpp"
#include "fraction.hpp"

int main(int argc,char* argv[])
{
    if (argc < 3) {
        std::cerr << "Please enter:" << std::endl;
        std::cerr << "\t- the input file" << std::endl;
        std::cerr << "\t- m, the number of non-objective rows" << std::endl;
        std::cerr << "\t- n, the number of unknowns" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream file;
    file.open(argv[1]);

    if (!file.is_open()) {
        std::cerr << "Couldn't open file: " << argv[1] << std::endl;
        exit(EXIT_FAILURE);
    }

    unsigned m = atoi(argv[2]);
    unsigned n = atoi(argv[3]);

    Tableau<Fraction<int>> t(file, m, n);

    std::stringstream latex;

    latex << "\\begin{equation*}" << std::endl;
    latex << "\t\\begin{array}{c|" << std::string(n, 'c') << "|c}";
    latex << std::endl;
    latex << "\t\t\\text{BVs} & ";
    for (unsigned i = 1; i <= n; i++) {
        latex << "x_" << i << " & ";
    }
    latex << "RHS \\\\" << std::endl;

    while(true) {
        t.print(std::cout);

        unsigned column = t.choose_pivot_column();
        std::cerr << "Which column would you like to pivot on?" << std::endl;
        std::cerr << "Suggested column: " << column << std::endl;
        std::cin >> column;

        unsigned row = t.choose_pivot_row(column);
        std::cerr << "Which row would you like to pivot on?" << std::endl;
        std::cerr << "Suggested row: " << row << std::endl;
        std::cerr << "Enter 0 to stop pivoting" << std::endl;
        std::cin >> row;

        t.print_latex(latex, row, column);

        if (row == 0)
            break;

        t.pivot_on(row, column);
    }

    latex << "\t\\end{array}" << std::endl;
    latex << "\\end{equation*}" << std::endl;

    std::cout << latex.str() << std::endl;
}
