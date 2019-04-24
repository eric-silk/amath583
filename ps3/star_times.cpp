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
    if (argc != 3)
    {
        std::cerr << "Invalid usage. ./star_times vector_size trips" << std::endl;
        return -1;
    }
    size_t size = std::atoi(argv[1]);
    size_t trips = std::atoi(argv[2]);

    double alpha = 867.5309;
    Vector x(size), y(size);

    Timer T;
    T.start();
    for (size_t j = 0; j < trips; ++j)
    {
        for (size_t i = 0; i < size; ++i)
        {
            /* Hand written -- WRITE ME */
            y(i) = y(i) + (alpha * x(i));
        }
    }
    T.stop();

    std::cout << "Hand written: " << T.elapsed()/((double) trips) << std::endl;

    T.start();
    for (size_t j = 0; j < trips; ++j)
    {
        y = y + alpha * x;
    }
    T.stop();

    std::cout << "y = y + alpha * x: " << T.elapsed()/((double) trips) << std::endl;

    T.start();
    for (size_t j = 0; j < trips; ++j)
    {
        y += alpha * x;
    }
    T.stop();

    std::cout << "y += alpha * x: " << T.elapsed()/((double) trips) << std::endl;

    return 0;
}
