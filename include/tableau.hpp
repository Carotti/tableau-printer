#ifndef TABLEAU_HPP
#define TABLEAU_HPP

#include <iostream>
#include <vector>
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

class NoRow : public TableauException {
public:
    virtual const char* what() const throw()
    {
        return "No row found for a specified variable";
    }
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
        print_rows(os, objective_rows, objective_vars);
        print_rows(os, basic_rows, basic_vars);
    }

    // Returns wheter or not var is a basic variable
    bool is_basic(const std::string& var)
    {
        return variable_is(var, basic_vars);
    }

    // Returns whether or not var is a regular variable
    bool is_regular(const std::string& var) const
    {
        return variable_is(var, regular_vars);
    }

    void choose_pivot_column(const std::string& objective_var) const
    {

    }

private:

    const static char delimiter = '\t';

    constexpr const static char* first_start = "BVs";
    constexpr const static char* first_end = "RHS";

    using SimplexVars = std::vector<std::string>;
    using SimplexRows = std::vector<std::vector<T>>;

    SimplexVars regular_vars;

    SimplexVars basic_vars; // Each basic_var has a basic_row
    SimplexVars objective_vars; // Each objective_var has an objective_row

    SimplexRows basic_rows;
    SimplexRows objective_rows;

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
        if (is_regular(tmp)) {
            basic_vars.push_back(tmp);
            read_elements(is, false);
        } else {
            objective_vars.push_back(tmp);
            read_elements(is, true);
        }
    }

    void read_elements(std::istream& is, bool objective)
    {
        T tmp;
        std::vector<T> row;

        for (unsigned i = 0; i <= regular_vars.size(); i++) {
            is >> tmp;

            if (!is)
                throw InvalidInput("Not enough values in rows");

            row.push_back(tmp);
        }

        if (objective) {
            objective_rows.push_back(row);
        } else {
            basic_rows.push_back(row);
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

    void print_rows(std::ostream& os, const SimplexRows& rows,
        const SimplexVars& vars) const
    {
        for (unsigned i = 0; i < rows.size(); i++) {
            os << vars[i];
            for (const T& element : rows[i]) {
                os << delimiter << element;
            }
            os << std::endl;
        }
    }

    bool variable_is(const std::string& var, const SimplexVars& vars) const
    {
        try {
            row_index_of(var, vars);
        } catch (NoRow& e)  {
            return false;
        }

        return true;
    }

    // Returns the index of the row which corresponds to var
    unsigned row_index_of(const std::string& var, const SimplexVars& vars) const
    {
        for (unsigned i = 0; i < vars.size(); i++) {
            if (vars[i] == var)
                return i;
        }
        throw NoRow();
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Tableau<T>& t)
{
    t.print(os);
    return os;
}


#endif
