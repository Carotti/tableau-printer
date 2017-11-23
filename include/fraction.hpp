#ifndef FRACTION_HPP
#define FRACTION_HPP

#include <iostream>
#include <string>
#include <sstream>

// Fractions are always stored in a simplified state
template <typename T>
class Fraction {
public:
    Fraction(T _numerator, T _denominator) :
        numerator(_numerator),
        denominator(_denominator)
    {
        simplify(); // Always simplify fractions on construction
    }

    Fraction(T _numerator) :
        numerator(_numerator),
        denominator(1)
    {
        simplify();
    }

    Fraction(const Fraction<T>& f) :
        numerator(f.numerator),
        denominator(f.denominator)
    {} // No need to simplify, since previous fraction must be simplified

    // Default fraction is 0/1
    Fraction() :
        numerator(0),
        denominator(1)
    {}

    Fraction<T> operator+(const Fraction<T>& rhs) const
    {
        T num = numerator * rhs.denominator + denominator * rhs.numerator;
        T den = denominator * rhs.denominator;
        return Fraction<T>(num, den);
    }

    Fraction<T> operator-(const Fraction<T>& rhs) const
    {
        T num = numerator * rhs.denominator - denominator * rhs.numerator;
        T den = denominator * rhs.denominator;
        return Fraction<T>(num, den);
    }

    Fraction<T> operator*(const Fraction<T>& rhs) const
    {
        T num = numerator * rhs.numerator;
        T den = denominator * rhs.denominator;
        return Fraction<T>(num, den);
    }

    Fraction<T> operator/(const Fraction<T>& rhs) const
    {
        T num = numerator * rhs.denominator;
        T den = denominator * rhs.numerator;
        return Fraction<T>(num, den);
    }

    Fraction<T>& operator+=(const Fraction<T>& rhs)
    {
        numerator = numerator * rhs.denominator + denominator * rhs.numerator;
        denominator = denominator * rhs.denominator;
        simplify();
        return *this;
    }

    Fraction<T>& operator-=(const Fraction<T>& rhs)
    {
        numerator = numerator * rhs.denominator - denominator * rhs.numerator;
        denominator = denominator * rhs.denominator;
        simplify();
        return *this;
    }

    Fraction<T>& operator*=(const Fraction<T>& rhs)
    {
        numerator *= rhs.numerator;
        denominator *= rhs.denominator;
        simplify();
        return *this;
    }

    Fraction<T>& operator/=(const Fraction<T>& rhs)
    {
        numerator *= rhs.denominator;
        denominator *= rhs.numerator;
        simplify();
        return *this;
    }

    bool operator==(const Fraction<T>& rhs) const
    {
        return numerator == rhs.numerator && denominator == rhs.denominator;
    }

    bool operator<(const Fraction<T>& rhs) const
    {
        return (numerator * rhs.denominator) < (denominator * rhs.numerator);
    }

    bool operator>(const Fraction<T>& rhs) const
    {
        return (numerator * rhs.denominator) > (denominator * rhs.numerator);
    }

    bool operator<=(const Fraction<T>& rhs) const
    {
        return (numerator * rhs.denominator) <= (denominator * rhs.numerator);
    }

    bool operator>=(const Fraction<T>& rhs) const
    {
        return (numerator * rhs.denominator) >= (denominator * rhs.numerator);
    }

    template <typename U>
    friend std::ostream& operator<<(std::ostream&, const Fraction<U>& f);

    template <typename U>
    friend std::istream& operator>>(std::istream& is, Fraction<U>& f);

    void print(std::ostream& os) const
    {
        if (denominator == 0) {
            os << infinity;
        } else if (denominator == 1) {
            os << numerator;
        } else {
            os << numerator << delimiter << denominator;
        }
    }

    bool is_infinity()
    {
        return denominator == 0;
    }

    void print_latex(std::ostream& os)
    {
        if (denominator == 0) {
            os << "\\infty";
        } else if (denominator == 1) {
            os << numerator;
        } else {
            os << "\\frac{" << numerator << "}{" << denominator << "}";
        }
    }

private:

    const static char delimiter = '/';
    constexpr const static char* infinity = "inf";

    T numerator;
    T denominator;

    // Returns the greatest common denominator of a and b
    T gcd(const T& a, const T& b) const
    {
        if (a == b)
            return a;

        if (a == 0)
            return b;

        if (b == 0)
            return a;

        if (a > b) {
            return gcd(a % b, b);
        } else {
            return gcd(b % a, a);
        }
    }

    T absolute(T& x) const
    {
        if (x < 0) {
            return -x;
        } else {
            return x;
        }
    }

    void simplify() {
        // denominator is always a non-negative number
        if (denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }

        T divisor = gcd(absolute(numerator), denominator);
        numerator /= divisor;
        denominator /= divisor;

        if (numerator == 0)
            denominator = 1;
    };
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Fraction<T>& f)
{
    f.print(os);
    return os;
}

template <typename T>
std::istream& operator>>(std::istream& is, Fraction<T>& f)
{
    std::string input;
    is >> input;
    size_t delim_pos = input.find(f.delimiter);

    std::stringstream num(input.substr(0, delim_pos));
    num >> f.numerator;

    if (delim_pos == std::string::npos) {
        // No delimiter was found, not a fractional number
        f.denominator = 1;
    } else {
        std::stringstream den(input.substr(delim_pos + 1));
        den >> f.denominator;
    }

    return is;
}


#endif
