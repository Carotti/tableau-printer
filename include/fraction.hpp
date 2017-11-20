#ifndef FRACTION_HPP
#define FRACTION_HPP

#include <iostream>
#include <string>

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
        
    }

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

    Fraction<T> operator/(const Fraction<T> & rhs) const
    {
        T num = numerator * rhs.denominator;
        T den = denominator * rhs.numerator;
        return Fraction<T>(num, den);
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

private:
    T numerator;
    T denominator;

    // Returns the greatest common denominator of a and b
    T gcd(const T& a, const T& b) {
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

    void simplify() {
        T divisor = gcd(numerator, denominator);
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
        os << f.numerator << "/" << f.denominator;
    }
}

#endif