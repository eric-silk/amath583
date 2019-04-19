//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//


#include <iostream>
#include "Timer.hpp"
#include <cstdlib>
#include <sstream>
#include <string>

int main(int argc, char* argv[])
{
    std::stringstream ss;
    ss << argv[1];

    size_t loops = 0;
    ss >> loops;

    double a = 3.14, b = 3.14159, c = 0.0;

    Timer T;
    T.start();
    for (size_t i = 0; i < loops; ++i)
    {
        c += a * b;
    }
    T.stop();

    std::cout << "Final value of c: " << c << std::endl;
    std::cout << loops << " loops took " << T.elapsed() << " milliseconds" << std::endl;

    return 0;
}
