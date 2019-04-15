//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include "amath583.hpp"
#include "Vector.hpp"
#include <cmath>


void zeroize(Vector& x)
{
    // Would rather use an iterator over the std::vector member but its private :(
    for(size_t i = 0; i < x.num_rows(); ++i)
    {
        x(i) = 0;
    }
}

double one_norm(const Vector& x)
{
    double sum = 0.0;
    for (size_t i = 0; i < x.num_rows(); ++i)
    {
        sum += std::abs(x(i));
    }
    return sum;
}

double two_norm(const Vector& x)
{
    // Currently assumes REAL std::vectors, may need to change this for complex std::vectors
    double result = 0.0;
    for (size_t i = 0; i < x.num_rows(); ++i)
    {
        result += std::pow(std::abs(x(i)), 2.0);
    }

    result = std::sqrt(result);

    return result;
}

double inf_norm(const Vector& x)
{
    double result = 0.0;
    for (size_t i = 0; i < x.num_rows(); ++i)
    {
        if (std::abs(x(i)) > result)
        {
            result = std::abs(x(i));
        }
    }

    return result;
}

// AMATH 583 students will need to add dot() 
Vector dot(const Vector& x, const Vector& y)
{
    if (x.num_rows() != y.num_rows())
    {
        throw "Dot product of unequal length vectors is not defined!";
    }

    double result = 0.0;

    for (size_t i=0; i < x.num_rows(); ++i)
    {
        result += x(i)*y(i);
    }

    return result;
}
