//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//


#include <iostream>
#include "Timer.hpp"
#include "amath583.hpp"
#include <cstdlib>

int main(int argc, char* argv[])
{
    size_t size = std::atoi(argv[1]); // FIXME
    size_t trips = std::atoi(argv[2]); // FIXME

    Vector x(size), y(size);

    Timer T;
    T.start();
    for (size_t j = 0; j < trips; ++j)
    {
        for (size_t i = 0; i < size; ++i)
        {
            /* Hand written -- WRITE ME */
            x(i) = x(i) + y(i);
        }
    }
    T.stop();

    std::cout << "Hand written: " << T.elapsed()/((double) trips) << std::endl;

    T.start();
    for (size_t j = 0; j < trips; ++j)
    {
        y = y + x;
    }
    T.stop();

    std::cout << "y = y + x: " << T.elapsed()/((double) trips) << std::endl;

    T.start();
    for (size_t j = 0; j < trips; ++j)
    {
        y += x;
    }
    T.stop();

    std::cout << "y += x: " << T.elapsed()/((double) trips) << std::endl;

    return 0;
}
