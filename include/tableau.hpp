#ifndef TABLEAU_HPP
#define TABLEAU_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <exception>

class TableauException : public std::exception {
public:
    virtual const char* what() const throw() = 0;
};

class InvalidInput : public TableauException {
public:
    InvalidInput(const char* _msg) : msg(_msg)
    {}

    virtual const char* what() const throw()
    {
        std::string error("Invalid input tableau: ");
        error += std::string(msg);
        return error.c_str();
    }

private:
    const char* msg;
};

class InvalidVariable : public TableauException {
public:
    InvalidVariable(const char* _msg) : msg(_msg)
    {}

    virtual const char* what() const throw()
    {
        std::string error("Invalid variable use: ");
        error += std::string(msg);
        return error.c_str();
    }

private:
    const char* msg;
};

// m rows of the simplex tableau, n variables
template <typename T>
class Tableau {
public:

    Tableau(std::istream& is)
    {
        read_first_row(is);
        while(is)
            read_row(is);
    }

    template <typename U>
    friend std::ostream& operator<<(std::ostream&, const Tableau<U>& t);

    void print(std::ostream& os) const
    {
        print_first_row(os);
        print_rows(os, objective_rows);
        print_rows(os, basic_rows);
    }

    bool is_basic(const std::string& var) const
    {
        return basic_rows.find(var) != basic_rows.end();
    }

    // regular variables are non-objective variables
    // objective variables are non-regular variables
    bool is_regular(const std::string& var) const
    {
        for (const std::string& regular_var : regular_vars) {
            if (var == regular_var)
                return true;
        }
        return false;
    }

    // Choose the best pivot column based on the specified objective variable
    // Returns an empty string if no column can be chosen
    std::string choose_pivot_column(const std::string& objective_var) const
    {
        T largest = 0;
        std::string pivot = "";

        auto row = objective_rows.find(objective_var);
        if (row == objective_rows.end()) // Variable was not objective
            throw InvalidVariable("variable is not objective");

        for (auto it = row->second.begin(); it != row->second.end(); ++it) {
            if (it->second > largest) {
                largest = it->second;
                pivot = it->first;
            }
        }

        return pivot;
    }

    // Choose the best pivot row based on the specified regular variable
    // Returns an empty string of no row can be chosen
    std::string choose_pivot_row(const std::string& regular_var) const
    {
        // Set the smallest value to the ratio of the first basic row
        T smallest = get_ratio(basic_rows.begin()->first, regular_var);
        std::string pivot = "";

        if (!is_regular(regular_var))
            throw InvalidVariable("variable is not regular");

        for (auto it = basic_rows.begin(); it != basic_rows.end(); ++it) {
            T ratio = get_ratio(it->first, regular_var);
            if (ratio < smallest && ratio > 0) {
                smallest = ratio;
                pivot = it->first;
            }
        }

        return pivot;
    }

    void pivot_on(const std::string& basic, const std::string& regular)
    {
        
    }

private:

    const static char delimiter = '\t';

    constexpr const static char* first_start = "BVs";
    constexpr const static char* first_end = "RHS";

    using SimplexVars = std::vector<std::string>;

    using SimplexRow = std::map<std::string, T>;
    using SimplexRows = std::map<std::string, SimplexRow>;

    SimplexVars regular_vars;

    SimplexRows basic_rows;
    SimplexRows objective_rows;

    // All of the right hand side values in the tableau
    SimplexRow rhs;

    void read_first_row(std::istream& is)
    {
        std::string tmp;
        is >> tmp;
        if (tmp != first_start) {
            throw InvalidInput("Symbol not found at start of first row");
        }

        for (is >> tmp; tmp != first_end && is; is >> tmp) {
            regular_vars.push_back(tmp);
        }

        if (tmp != first_end) {
            throw InvalidInput("Symbol not found at end of first row");
        }
    }

    void read_row(std::istream& is)
    {
        std::string tmp;
        is >> tmp;

        // No more rows are left
        if (tmp.empty())
            return;

        // Objective variables didn't appear in the first row
        read_elements(is, tmp, is_regular(tmp));
    }

    void read_elements(std::istream& is, const std::string& var, bool regular)
    {
        T tmp;
        SimplexRow row;

        for (unsigned i = 0; i <= regular_vars.size(); i++) {
            is >> tmp;

            if (!is)
                throw InvalidInput("Not enough values in rows");

            // Last value is the right hand side
            if (i == regular_vars.size()) {
                rhs[var] = tmp;
            } else {
                row[regular_vars[i]] = tmp;
            }
        }

        if (regular) {
            basic_rows[var] = row;
        } else {
            objective_rows[var] = row;
        }
    }

    void print_first_row(std::ostream& os) const
    {
        os << first_start << delimiter;
        for (const std::string& var : regular_vars) {
            os << var << delimiter;
        }
        os << first_end << std::endl;
    }

    void print_rows(std::ostream& os, const SimplexRows& rows) const
    {
        for (auto it = rows.begin(); it != rows.end(); ++it) {
            os << it->first;
            for (const std::string& var : regular_vars) {
                os << delimiter << (it->second).find(var)->second;
            }
            os << delimiter << rhs.find(it->first)->second;
            os << std::endl;
        }
    }

    T get_ratio(const std::string& basic, const std::string& regular) const
    {
        if (!is_basic(basic))
            throw InvalidVariable("variable is not basic");

        if (!is_regular(regular))
            throw InvalidVariable("variable is not regular");

        return rhs.at(basic) / basic_rows.at(basic).at(regular);
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Tableau<T>& t)
{
    t.print(os);
    return os;
}


#endif
