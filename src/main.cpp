#include <iostream>
#include <fstream>

#include "tableau.hpp"
#include "fraction.hpp"

using Element = Fraction<int>;
using SimplexTableau = Tableau<Element>;

void pivot(SimplexTableau& t)
{
    std::cerr << "Enter a variable to enter the basis ";
    std::string suggestion = t.choose_pivot_column();
    if (suggestion != "")
        std::cerr << "(suggested: " << t.choose_pivot_column() << ") ";
    std::string regular;
    std::cin >> regular;

    if (t.is_basic(regular)) {
        std::cerr << regular << " is already basic" << std::endl;
        return;
    }

    if (!t.is_regular(regular)) {
        std::cerr << regular << " is not a variable" << std::endl;
        return;
    }

    std::cerr << "Enter a variable to leave the basis ";
    suggestion = t.choose_pivot_row(regular);
    if (suggestion != "")
        std::cerr << "(suggested: " << t.choose_pivot_row(regular) << "): ";
    std::string basic;
    std::cin >> basic;

    if (!t.is_basic(basic)) {
        std::cerr << basic << " is not a basic variable" << std::endl;
        return;
    }

    t.pivot_on(basic, regular);
}

void swap_basic(SimplexTableau& t)
{

}

void swap_objective(SimplexTableau& t)
{

}

void add_variable(SimplexTableau& t)
{

}

void add_basic_variable(SimplexTableau& t)
{
    std::cerr << "Enter the variable you want to add to the basis ";
    std::string new_basic;
    std::cin >> new_basic;

    if (t.is_basic(new_basic)) {
        std::cerr << new_basic << " is already a basic variable" << std::endl;
        std::cerr << "Pivot " << new_basic << "out then try again" << std::endl;
        return;
    }

    if (!t.is_regular(new_basic)) {
        std::cerr << new_basic << " is not a variable" << std::endl;
        std::cerr << "Try adding it as a variable then try agani" << std::endl;
    }

    std::cerr << "Enter the values for each column in the row" << std::endl;
    SimplexTableau::SimplexRow row;
    for (const std::string& rv : t.get_regular_vars()) {
        Element el;
        std::cerr << rv << ": ";
        std::cin >> el;
        row[rv] = el;
    }

    Element rhs;
    std::cerr << "RHS: ";
    std::cin >> rhs;

    t.add_basic_row(new_basic, row, rhs);
    std::cerr << std::endl << t << std::endl;
    t.basic_row_ops(new_basic);
}

void add_objective_variable(SimplexTableau& t)
{

}

void remove_variable(SimplexTableau& t)
{

}

void remove_objective_variable(SimplexTableau& t)
{

}

void row_operations(SimplexTableau& t)
{

}

void print_latex(SimplexTableau& t)
{

}

bool make_choice(SimplexTableau& t)
{
    std::cerr << "Choose from the following:" << std::endl;
    std::cerr << "\t1  - Pivot" << std::endl;
    std::cerr << "\t2  - Swap basic rows" << std::endl;
    std::cerr << "\t3  - Swap objective rows" << std::endl;
    std::cerr << "\t4  - Add a new variable" << std::endl;
    std::cerr << "\t5  - Add a new basic variable" << std::endl;
    std::cerr << "\t6  - Add a new objective variable" << std::endl;
    std::cerr << "\t7  - Remove a variable" << std::endl;
    std::cerr << "\t8  - Remove an objective variable" << std::endl;
    std::cerr << "\t9  - Manual row operations" << std::endl;
    std::cerr << "\t10 - Print LaTeX" << std::endl;
    std::cerr << "\t11 - Quit" << std::endl;

    int choice;
    std::cin >> choice;

    switch(choice) {
        case 1:
            pivot(t);
            break;
        case 2:
            swap_basic(t);
            break;
        case 3:
            swap_objective(t);
            break;
        case 4:
            add_variable(t);
            break;
        case 5:
            add_basic_variable(t);
            break;
        case 6:
            add_objective_variable(t);
            break;
        case 7:
            remove_variable(t);
            break;
        case 8:
            remove_objective_variable(t);
            break;
        case 9:
            row_operations(t);
            break;
        case 10:
            print_latex(t);
            break;
        case 11:
            return false;
            break;
        default:
            std::cerr << "Unkown choice: " << choice << std::endl;
            return make_choice(t);
            break;
    }

    return true;
}

int main(int argc, char* argv[])
{
    try {
        std::ifstream file;

        if (argc > 1) {
            file.open(argv[1]);

            if (!file.is_open()) {
                std::cerr << "Couldn't open file " << argv[1] << std::endl;
                exit(EXIT_FAILURE);
            }
        } else {
            std::cerr << "Please enter a file name" << std::endl;
            exit(EXIT_FAILURE);
        }

        SimplexTableau t(file);

        do {
            std::cout << std::endl << t << std::endl;
            std::cerr << std::endl << t << std::endl;

            if (t.is_optimal()) {
                std::cerr << "Tableau is optimal" << std::endl;
            }
        } while(make_choice(t));

    } catch (TableauException& e) {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}
