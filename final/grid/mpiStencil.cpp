//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//


#include <mpi.h>
#include "Grid.hpp"
#include "mpiStencil.hpp"


void update_halo(Grid& x) {
  int uptag = 0, downtag = 1;
  // Populate the local edge arrays
  // If the left or right partition, don't populate both!
  size_t myrank = MPI::COMM_WORLD.Get_rank();
  size_t mysize = MPI::COMM_WORLD.Get_size();

  size_t const edge_size = x.num_x();

  if (myrank < mysize-1){
    // Need to send the bottom for all but the bottommost
    MPI::COMM_WORLD.Sendrecv(&x(0, x.num_x()-1), edge_size, MPI::DOUBLE, myrank+1, downtag,
                             &x(0,   x.num_x()), edge_size, MPI::DOUBLE, myrank+1, uptag);
  }
  if (myrank > 0){
    // Need to send the top for all but the topmost
    MPI::COMM_WORLD.Sendrecv(&x(0, 1), edge_size, MPI::DOUBLE, myrank-1, uptag,
                             &x(0, 0), edge_size, MPI::DOUBLE, myrank-1, downtag);
  }

}

Grid operator*(const mpiStencil& A, const Grid& x) {
  Grid y(x);

  update_halo(const_cast<Grid&>(x));

  for (size_t i = 1; i < x.num_x()-1; ++i) {
    for (size_t j = 1; j < x.num_y()-1; ++j) {
      y(i, j) = x(i,j) - (x(i-1, j) + x(i+1, j) + x(i, j-1) + x(i, j+1))/4.0;
    }
  }

  return y;
}

