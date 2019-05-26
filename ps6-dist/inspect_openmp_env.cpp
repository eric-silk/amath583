#include <string>
#include <iostream>
#include <thread>
#include <cstdlib>
#include <omp.h>

std::string getenv_to_string(const char *in)
{
    char *gotten = std::getenv(in);
    if (NULL == gotten)
    {
        return std::string("");
    }
    else
    {
        return std::string(gotten);
    }
}

std::string getenv(const std::string& in)
{
    return getenv_to_string(in.c_str());
}

int omp_thread_count_par_red()
{
    int n = 0;
#pragma omp parallel reduction(+:n)
    n += 1;
    return n;
}

int omp_thread_count_par()
{
    int n = 0;
#pragma parallel
    {
        int tid = omp_get_thread_num();
        if (0 == tid)
        {
            n = omp_get_num_threads();
        }
    }

    return n;
}

int main()
{
    std::string envName = "OMP_NUM_THREADS";
    std::cout << envName << "            = " << getenv(envName) << std::endl;
    std::cout << "hardware_concurrency()     = " <<
        std::thread::hardware_concurrency() << std::endl;
    std::cout << "omp_get_max_threads()      = " <<
        omp_get_max_threads() << std::endl;
    std::cout << "omp_thread_count_par_red() = " << omp_thread_count_par_red() << std::endl;
    std::cout << "omp_thread_count_par()     = " << omp_thread_count_par() << std::endl;

    size_t user_num;
    if (argc >= 2) user_num = std::stol(argv[1]);
    #pragma omp parallel num_threads(user_num)
    {
        #pragma omp critical
        {
            std::cout << "Hello! I am thread " << omp_get_thread_num()
                << " of " << omp_get_num_threads() << std::endl;
            std::cout << "My C++ std::thread ID is " << 
                std::this_thread::get_id() << std::endl;
        }
    }

    return 0;
}

