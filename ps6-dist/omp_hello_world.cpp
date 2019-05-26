#include <string>
#include <iostream>
#include <thread>
#include <cstdlib>
#include <omp.h>

int main()
{
    std::cout << "omp_get_max_threads() = " <<
        omp_get_max_threads() << std::endl;
    std::cout << "omp_get_num_threads() = " <<
        omp_get_num_threads() << std::endl;

#pragma omp parallel
#pragma omp critical
    {
        std::cout << "Hello! I am thread " << omp_get_thread_num() <<
            " of " << omp_get_num_threads() << std::endl;
        std::cout << "My C++ std::thread ID is " <<
            std::this_thread::get_id() << std::endl;
    }

    return 0;
}
