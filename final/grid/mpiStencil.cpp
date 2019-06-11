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

  /* Write me */
  // We need an array to receive into on both the top and bottom (but not sides, per Dr. Lumsdaine)
  double *above = nullptr, *below = nullptr;
  // Similarly, we need arrays of the top and bottom edges of the current grid to send
  double *top_edge = nullptr, *bottom_edge = nullptr;

  size_t const edge_size = x.num_x() -2;

  above       = (double*) malloc(edge_size * sizeof(double));
  below       = (double*) malloc(edge_size * sizeof(double));

  top_edge    = (double*) malloc(edge_size * sizeof(double));
  bottom_edge = (double*) malloc(edge_size * sizeof(double));

  MPI::COMM_WORLD.Sendrecv(top_edge, 

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

