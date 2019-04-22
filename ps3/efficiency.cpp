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
    if(argc != 2)
    {
        std::cerr << "Invalid parameters. Usage: './float_vs_double*.exe <Number of repetitions>'"
            << std::endl;
        return -1;
    }
    std::stringstream ss;
    ss << argv[1];

    size_t loops = 0;
    ss >> loops;


    std::cout << "Number of loops: " << loops << std::endl;
    if(loops > 30000000000)
    {
        std::cout<< "Warning: ~30,000,000,000 loops takes ~1 minute to run." << std::endl;
    }

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
