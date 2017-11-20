#include <iostream>
#include <cstdlib>
#include <fstream>

#include "tableau.hpp"
#include "fraction.hpp"

int main(int argc,char* argv[])
{
    if (argc < 1) {
        std::cerr << "Please enter the input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream file;
    file.open(argv[1]);

    if (!file.is_open()) {
        std::cerr << "Couldn't open file: " << argv[1] << std::endl;
        exit(EXIT_FAILURE);
    }

    file.close();

    Tableau<Fraction<int>, 2, 4> t(
        {3, 4},
        {{
            {Fraction<int>(5, 1), Fraction<int>(10, 1), Fraction<int>(0, 1), Fraction<int>(0, 1), Fraction<int>(0, 1)},
            {Fraction<int>(1, 1), Fraction<int>(3, 2), Fraction<int>(1, 1), Fraction<int>(0, 1), Fraction<int>(2, 1)},
            {Fraction<int>(4, 1), Fraction<int>(2, 1), Fraction<int>(0, 1), Fraction<int>(1, 1), Fraction<int>(2, 1)}
        }}
    );

    t.print(std::cout);

    std::cout << t.choose_pivot_column() << std::endl;
    std::cout << t.choose_pivot_row(t.choose_pivot_column()) << std::endl;
}
