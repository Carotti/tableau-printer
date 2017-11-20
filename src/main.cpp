#include <iostream>
#include <cstdlib>
#include <fstream>

#include "matrix.hpp"
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

    Fraction<int> a(5, 3);
    Fraction<int> b(10, 3);

    Fraction<int> result = a + b;

    std::cout << result << std::endl;
}
