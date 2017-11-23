#include <iostream>

#include "tableau.hpp"
#include "fraction.hpp"

int main()
{
    try {
        Tableau<Fraction<int>> t(std::cin);
        t.print(std::cout);
        std::cout << t.choose_pivot_row(std::string("x2")) << std::endl;

    } catch (TableauException& e) {
        std::cerr << e.what() << std::endl;
    }
}
