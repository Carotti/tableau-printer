#include <iostream>

#include "tableau.hpp"
#include "fraction.hpp"

int main()
{
    try {
        Tableau<Fraction<int>> t(std::cin);
        t.print(std::cout);
        std::cout << t.is_basic(std::string("x3")) << std::endl;

    } catch (TableauException& e) {
        std::cerr << e.what() << std::endl;
    }
}
