#ifndef FRACTION_HPP
#define FRACTION_HPP

#include <iostream>
#include <string>
#include <sstream>

// Fractions are always stored in a simplified state
// denominator is always a positive number
template <typename T>
class Fraction {
public:
    const static char delimiter = '/';

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

    Fraction(std::string str)
    {
        size_t delim_pos = str.find(delimiter);

        std::stringstream num(str.substr(0, delim_pos));
        num >> numerator;

        if (delim_pos == std::string::npos) {
            denominator = 1;
        } else {
            std::stringstream den(str.substr(delim_pos + 1));
            den >> denominator;
        }
    }

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

    void print_latex(std::ostream& os)
    {
        if (denominator == 1)
            os << numerator;
        else
            os << "\\frac{" << numerator << "}{" << denominator << "}";
    }

private:
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

        if (a > b)
            return gcd(a % b, b);
        else
            return gcd(b % a, a);
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
        if (denominator < 0) {
            numerator *= -1;
            denominator *= -1;
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
    if (f.denominator == 1) {
        os << f.numerator;
    } else {
        os << f.numerator << f.delimiter << f.denominator;
    }
}

#endif
