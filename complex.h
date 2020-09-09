/**
 * Implements complex numbers 
 * @file complex.h
 * @author mariogmarq
 */

#ifndef COMPLEX_H
#define COMPLEX_H

#include <math.h>

/**
 * @class Complex
 * @brief Implements Complex numbers
 */

class Complex
{
private:
    double real;
    double imaginary;

public:
    /**
     * @brief Basic class constructors
     */
    Complex() : real(0), imaginary(0){};
    Complex(double x, double y) : real(x), imaginary(y){};
    explicit Complex(double x) : real(x), imaginary(0){};
    /**
     * @brief Returns the real part of the number
     * @return The real part of the number
     */
    inline double GetReal() const { return real; };
    /**
     * @brief Returns the imaginary part of the number
     * @return The imaginary part of the number
     */
    inline double GetImg() const { return imaginary; };
    /**
     * @brief Returns the modulus part of the number
     * @return The modulus part of the number
     */
    inline double Module() const { return sqrt(imaginary * imaginary + real * real); };
};

/**
 * @brief Overload of sum operator for complex numbers
 */
Complex operator+(Complex oleft, Complex oright)
{
    return Complex(oleft.GetReal() + oright.GetReal(), oleft.GetImg() + oright.GetImg());
}

/**
 * @brief Overload of substraction operator for complex numbers
 */
Complex operator-(Complex oleft, Complex oright)
{
    return Complex(oleft.GetReal() - oright.GetReal(), oleft.GetImg() - oright.GetImg());
}

/**
 * @brief Overload of multiplication operator for complex numbers
 */
Complex operator*(Complex oleft, Complex oright)
{
    return Complex(oright.GetReal() * oleft.GetReal() - oright.GetImg() * oright.GetImg(), oright.GetReal() * oleft.GetImg() + oleft.GetReal() * oright.GetImg());
}

#endif
