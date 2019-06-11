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
  // Written assuming the halo is included already!

  // We need an array to receive into on both the top and bottom (but not sides, per Dr. Lumsdaine)
  double *above = nullptr, *below = nullptr;
  // Similarly, we need arrays of the top and bottom edges of the current grid to send
  double *top_edge = nullptr, *bottom_edge = nullptr;

  size_t const edge_size = x.num_x();


  // Populate the local edge arrays
  // If the top or bottom partition, don't populate both!
  size_t myrank = MPI::COMM_WORLD.Get_rank();
  size_t mysize = MPI::COMM_WORLD.Get_size();

  // TODO: Verify row vs. column major order
  // Currently assuming "above" is myrank-1, TODO verify this

  if (myrank == 0){
    // Top of the group
    // Need to send/receive the bottom only
    below       = (double*) malloc(edge_size * sizeof(double));
    bottom_edge = (double*) malloc(edge_size * sizeof(double));

    // When iterating, include the corners as well
    // Get the top edges to send
    for (size_t i = 0; i < edge_size; ++i){
      bottom_edge[i] = x(i, x.num_y()-1);
    }

    MPI::COMM_WORLD.Sendrecv(bottom_edge, edge_size, MPI::DOUBLE, myrank+1, 0,
                             below,       edge_size, MPI::DOUBLE, myrank+1, 0);
    // Now push the halo values in
    for (size_t i = 0; i < edge_size; ++i){
      x(i, x.num_y()) = below[i];
    }

    free(below);
    free(bottom_edge);
  }

  else if (myrank == mysize-1){
    // Bottom of the group
    // Need to send/receive the top only
    above       = (double*) malloc(edge_size * sizeof(double));
    top_edge    = (double*) malloc(edge_size * sizeof(double));

    // When iterating, include the corners as well
    // Get the top edges to send
    for (size_t i = 0; i < edge_size; ++i){
      top_edge[i] = x(i, 1);
    }

    MPI::COMM_WORLD.Sendrecv(top_edge,    edge_size, MPI::DOUBLE, myrank-1, 0,
                             above,       edge_size, MPI::DOUBLE, myrank-1, 0);
    // Now push the halo values in
    for (size_t i = 0; i < edge_size; ++i){
      x(i, 0) = above[i];
    }
    free(top_edge);
    free(above);
  }

  else{
    // Non-edge partition
    // Both top and bottom required
    above       = (double*) malloc(edge_size * sizeof(double));
    below       = (double*) malloc(edge_size * sizeof(double));
    top_edge    = (double*) malloc(edge_size * sizeof(double));
    bottom_edge = (double*) malloc(edge_size * sizeof(double));

    // When iterating, include the corners as well
    // Get the top edges to send
    for (size_t i = 0; i < edge_size; ++i){
      top_edge[i] = x(i, 1);
      bottom_edge[i] = x(i, x.num_y()-1);
    }

    MPI::COMM_WORLD.Sendrecv(top_edge,    edge_size, MPI::DOUBLE, myrank-1, 0,
                             above,       edge_size, MPI::DOUBLE, myrank-1, 0);
    MPI::COMM_WORLD.Sendrecv(bottom_edge, edge_size, MPI::DOUBLE, myrank+1, 0,
                             below,       edge_size, MPI::DOUBLE, myrank+1, 0);
    // Now push the halo values in
    for (size_t i = 0; i < edge_size; ++i){
      x(i, 0) = above[i];
      x(i, x.num_y()) = below[i];
    }
    free(top_edge);
    free(above);
    free(bottom_edge);
    free(below);
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

