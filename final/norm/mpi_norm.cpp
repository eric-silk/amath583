//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include <vector>
#include <numeric>
#include <cmath>
#include <iostream>
#include <mpi.h>
#include <string>
#include "Timer.hpp"
#include "amath583.hpp"


double mpi_norm(const Vector& local_x) {

  // local_rho is the sum of squares of the vector on THIS node
  double local_rho = std::inner_product(&local_x(0), &local_x(0)+local_x.num_rows(), &local_x(0), 0.0);

  // We need to reduce all of the local_rho to global_rho
  double global_rho = 0.0;

  // Write me -- fill in the arguments to allreduce -- and then uncomment it.
  // MPI::COMM_WORLD.Allreduce(/* write me */);
  // MPI::COMM_WORLD.Allreduce(void* send_data,
  //                           void* recv_data,
  //                           int count,
  //                           MPI_Datatype Datatype,
  //                           MPI_Op op,
  //                           MPI_Comm communicator)

  MPI::COMM_WORLD.Allreduce(&local_rho, &global_rho, 1,
                            MPI::DOUBLE, MPI::SUM);

  return std::sqrt(global_rho);
}


int main(int argc, char* argv[]) {
  MPI::Init();

  size_t myrank = MPI::COMM_WORLD.Get_rank();
  size_t mysize = MPI::COMM_WORLD.Get_size();

  size_t exponent           = 27;
  size_t num_trips          = 32;
  size_t num_elements = 0;

  if (0 == myrank) {

    if (argc >= 2) exponent   = std::stol(argv[1]);
    if (argc >= 3) num_trips  = std::stol(argv[2]);

    size_t total_elements = 1 << exponent;
    num_elements = total_elements / mysize;

    MPI::COMM_WORLD.Bcast(&num_elements, 1, MPI::DOUBLE, 0);
    MPI::COMM_WORLD.Bcast(&num_trips, 1, MPI::UNSIGNED_LONG, 0);

  } else {
    MPI::COMM_WORLD.Bcast(&num_elements, 1, MPI::DOUBLE, 0);
    MPI::COMM_WORLD.Bcast(&num_trips, 1, MPI::UNSIGNED_LONG, 0);
  }

  Vector local_x(num_elements);


  // 
  // Write me -- the contents of vector x should be randomized and scattered to all ranks
  // (Written!)
  // 
  double rho = 0.0;
  if (myrank == 0) {
    Vector x(num_elements * mysize);
    randomize(x, 0.0, 10.0);
    MPI::COMM_WORLD.Scatter(      &x(0), num_elements, MPI::DOUBLE,
				  &local_x(0), num_elements, MPI::DOUBLE, 0);

    // Compute the "correct" answer sequentially
    rho = std::sqrt(std::inner_product(&x(0), &x(0)+x.num_rows(), &x(0), 0.0));

  } else {
    MPI::COMM_WORLD.Scatter(        nullptr, num_elements, MPI::DOUBLE,
				    &local_x(0), num_elements, MPI::DOUBLE, 0);
  }

  double sigma = 0.0;

  DEF_TIMER(mpi_norm);
  START_TIMER(mpi_norm);
  for (size_t i = 0; i < num_trips; ++i) {
    sigma = mpi_norm(local_x);
  }
  if (myrank == 0) {
    STOP_TIMER(mpi_norm);
    std::cout << "|| x || = " << sigma << " ( " << rho << " )" << std::endl;
  } else {
    double foo = STOP_TIMER_QUIETLY(mpi_norm);
  }
  
  MPI::Finalize();
  
  return 0;
}
