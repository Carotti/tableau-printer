#ifndef TABLEAU_HPP
#define TABLEAU_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
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

    using SimplexVars = std::vector<std::string>;

    using SimplexRow = std::map<std::string, T>;
    using SimplexRows = std::map<std::string, SimplexRow>;

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

    bool is_objective(const std::string& var) const
    {
        for (const std::string& obj_var : objective_vars) {
            if (var == obj_var)
                return true;
        }
        return false;
    }

    unsigned num_regular_vars() const
    {
        return regular_vars.size();
    }

    // If no objective variable is specified, just use the first one
    std::string choose_pivot_column() const
    {
        return choose_pivot_column(objective_vars.front());
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
        if (!is_regular(regular_var))
            throw InvalidVariable("variable is not regular");

        // Set the smallest value to the ratio of the first basic row
        T smallest = 0;
        std::string pivot = "";
        bool found = false;

        for (const std::string bv : basic_vars) {
            T ratio = get_ratio(bv, regular_var);
            if ((ratio < smallest || !found) && ratio > 0) {
                found = true;
                smallest = ratio;
                pivot = bv;
            }
        }

        return pivot;
    }

    void basic_row_ops(const std::string& regular)
    {
        T ratio = basic_rows[regular][regular];

        for (const std::string& rv : regular_vars) {
            basic_rows[regular][rv] /= ratio;
        }
        rhs[regular] /= ratio;

        for (const std::string& bv : basic_vars) {
            T factor = basic_rows[bv][regular] * -1;
            if (bv != regular)
                row_operation(bv, regular, factor);
        }

        for (const std::string& ov : objective_vars) {
            T factor = objective_rows[ov][regular] * -1;
            row_operation(ov, regular, factor);
        }
    }

    void pivot_on(const std::string& basic, const std::string& regular)
    {
        // Add the regular variable to the basis
        basic_rows[regular] = basic_rows[basic];
        rhs[regular] = rhs[basic];

        // Replace the old basic variable in the set of basic variables
        for (std::string& bv : basic_vars) {
            if (bv == basic) {
                bv = regular;
                break;
            }
        }

        // Remove the old rows from the map
        basic_rows.erase(basic);
        rhs.erase(basic);

        basic_row_ops(regular);
    }

    bool is_optimal(const std::string& objective_var) const
    {
        for (const std::string& rv : regular_vars) {
            if (objective_rows.find(objective_var)->second.find(rv)->second > 0)
                return false;
        }
        return true;
    }

    bool is_optimal() const
    {
        return is_optimal(objective_vars.front());
    }

    void row_operation(const std::string& l, const std::string& r, const T& f)
    {
        rhs[l] += rhs[r] * f;
        add_row(row_from_var(l), row_from_var(r), f);
    }

    void add_basic_row(const std::string& bv, SimplexRow& new_row, const T& r)
    {
        basic_vars.push_back(bv);
        basic_rows[bv] = new_row;
        rhs[bv] = r;
    }

    const SimplexVars& get_regular_vars() const
    {
        return regular_vars;
    }

private:

    const static char delimiter = '\t';

    constexpr const static char* first_start = "BVs";
    constexpr const static char* first_end = "RHS";

    SimplexVars regular_vars;
    SimplexVars basic_vars;
    SimplexVars objective_vars;

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
            basic_vars.push_back(var);
        } else {
            objective_rows[var] = row;
            objective_vars.push_back(var);
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
        for (const auto& bv: vars) {
            os << bv << delimiter;
            for (const auto& rv : regular_vars) {
                os << rows.at(bv).at(rv) << delimiter;
            }
            os << rhs.at(bv) << std::endl;
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

    void add_row(SimplexRow& l, SimplexRow& r, const T& f)
    {
        for (const std::string& rv : regular_vars) {
            l[rv] += r[rv] * f;
        }
    }

    SimplexRow& row_from_var(const std::string& var)
    {
        return is_basic(var) ? basic_rows[var] : objective_rows[var];
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Tableau<T>& t)
{
    t.print(os);
    return os;
}


#endif
