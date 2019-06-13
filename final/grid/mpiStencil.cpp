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

  // We need an array to receive into on both the left and right
  double *left = nullptr, *right = nullptr;
  // Similarly, we need arrays of the left and right edges of the current grid to send
  double *left_edge = nullptr, *right_edge = nullptr;

  size_t const edge_size = x.num_y();

  // Populate the local edge arrays
  // If the left or right partition, don't populate both!
  size_t myrank = MPI::COMM_WORLD.Get_rank();
  size_t mysize = MPI::COMM_WORLD.Get_size();

  if (myrank == 0){
    // Leftmost of the group
    // Need to send/receive the right only
    right      = (double*) malloc(edge_size * sizeof(double));
    right_edge = (double*) malloc(edge_size * sizeof(double));

    if (nullptr == right || nullptr == right_edge){
      std::cout << "0 Failed to alloc memory!" << std::endl;
    }
      

    // When iterating, include the corners as well
    // Get the left edges to send
    for (size_t i = 0; i < edge_size; ++i){
      right_edge[i] = x(x.num_x()-1, i);
    }

    MPI::COMM_WORLD.Sendrecv(right_edge, edge_size, MPI::DOUBLE, myrank+1, 0,
                             right,      edge_size, MPI::DOUBLE, myrank+1, 0);
    // Now push the halo values in
    for (size_t i = 0; i < edge_size; ++i){
      x(x.num_x(), i) = right[i];
    }

    free(right);
    free(right_edge);
  }

  else if (myrank == mysize-1){
    // Rightmost of the group
    // Need to send/receive the left only
    left      = (double*) malloc(edge_size * sizeof(double));
    left_edge = (double*) malloc(edge_size * sizeof(double));

    if (nullptr == left || nullptr == left_edge){
      std::cout << myrank << " Failed to alloc memory!" << std::endl;
    }

    // When iterating, include the corners as well
    // Get the left edges to send
    for (size_t i = 0; i < edge_size; ++i){
      left_edge[i] = x(1, i);
    }

    MPI::COMM_WORLD.Sendrecv(left_edge, edge_size, MPI::DOUBLE, myrank-1, 0,
                             left,      edge_size, MPI::DOUBLE, myrank-1, 0);
    // Now push the halo values in
    for (size_t i = 0; i < edge_size; ++i){
      x(0, i) = left[i];
    }
    free(left_edge);
    free(left);
  }

  else{
    // Non-edge partition
    // Both left and right required
    left       = (double*) malloc(edge_size * sizeof(double));
    right      = (double*) malloc(edge_size * sizeof(double));
    left_edge  = (double*) malloc(edge_size * sizeof(double));
    right_edge = (double*) malloc(edge_size * sizeof(double));
    if (nullptr == left || nullptr == left_edge || nullptr == right || nullptr == right_edge){
      std::cout << myrank << " Failed to alloc memory!" << std::endl;
    }

    // When iterating, include the corners as well
    // Get the left edges to send
    for (size_t i = 0; i < edge_size; ++i){
      left_edge[i] = x(1, i);
      right_edge[i] = x(x.num_x()-1, i);
    }

    MPI::COMM_WORLD.Sendrecv(left_edge,  edge_size, MPI::DOUBLE, myrank-1, 0,
                             left,       edge_size, MPI::DOUBLE, myrank-1, 0);
    MPI::COMM_WORLD.Sendrecv(right_edge, edge_size, MPI::DOUBLE, myrank+1, 0,
                             right,      edge_size, MPI::DOUBLE, myrank+1, 0);
    // Now push the halo values in
    for (size_t i = 0; i < edge_size; ++i){
      x(0, i) = left[i];
      x(x.num_x(), i) = right[i];
    }
    free(left_edge);
    free(left);
    free(right_edge);
    free(right);
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

