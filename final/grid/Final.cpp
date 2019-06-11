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
#include "Final.hpp"
#include "mpiStencil.hpp"
#include "Stencil.hpp"
#include "Grid.hpp"


double mpiDot(const Grid& X, const Grid& Y) {
  double sum = 0.0;
  size_t myrank = MPI::COMM_WORLD.Get_rank();
  size_t mysize = MPI::COMM_WORLD.Get_size();

  // Have to be careful not to count the halos
  size_t start = (myrank == 0          ? 0        : 1         );
  size_t stop  = (myrank == (mysize-1) ? X.num_x() : X.num_x()-1);  

  for (size_t i = start; i < stop; ++i) {
    for (size_t j = 0; j < X.num_y(); ++j) {
      sum += X(i, j) * Y(i, j);
    }
  }
  double sigma = sum;
  // sigma now has the local portion of the inner product
  
  /* Complete me */
  // MPI::COMM_WORLD.Allreduce(/*sendbuf*/,/*recvbuf*/,/*count*/,/*type*/, MPI::SUM);
  MPI::COMM_WORLD.Allreduce(&signam, &dot, 1, MPI::DOUBLE, MPI::SUM);

  // This should have the total inner product -- on all nodes
  return sum;
}



size_t jacobi(const mpiStencil& A, Grid& x, const Grid& b, size_t maxiter, double tol) {

  Grid y = b;
  swap(x, y);

  for (size_t iter = 0; iter < maxiter; ++iter) {

    double rho = 0;

    for (size_t i = 1; i < x.num_x() - 1; ++i) {
      for (size_t j = 1; j < x.num_y() - 1; ++j) {
	y(i, j) = (x(i - 1, j) + x(i + 1, j) + x(i, j - 1) + x(i, j + 1)) / 4.0;
	rho += (y(i, j) - x(i, j)) * (y(i, j) - x(i, j));
      }
    }

    double sigma = rho;
    MPI::COMM_WORLD.Allreduce(&sigma, &rho, 1, MPI::DOUBLE, MPI::SUM);

    
    if (MPI::COMM_WORLD.Get_rank() == 0)  {
      std::cout << "||r|| = " << std::sqrt(rho) << std::endl;
    }
    if (std::sqrt(rho) < tol) return 0;

    swap(x, y);
    update_halo(const_cast<Grid&>(x));
  }
    
  return maxiter;
}


/* Parallelize me */
size_t ir(const mpiStencil& A, Grid& x, const Grid& b, size_t max_iter, double tol) {
  for (size_t iter = 0; iter < max_iter; ++iter) {
    Grid r = b - A*x;

    double sigma = dot(r, r);

    std::cout << "||r|| = " << std::sqrt(sigma) << std::endl;
    if (std::sqrt(sigma) < tol) return iter;

    x += r;
  
  }
  return max_iter;
}

/* AMATH 583 -- parallelize me */
size_t cg(const mpiStencil& A, Grid& x, const Grid& b, size_t max_iter, double tol) {

  Grid r = b - A*x, p(b);
  double rho = dot(r, r), rho_1 = 0.0;

  for (size_t iter = 0; iter < max_iter; ++iter) {
    std::cout << iter << ": ||r|| = " << std::sqrt(rho) << std::endl;

    if (iter == 0) {
      p = r;
    } else {
      double beta = (rho / rho_1);
      p = r +  beta * p;
    }
    
    Grid q = A*p;

    double alpha = rho / dot(p, q);
    
    x += alpha * p;
    
    rho_1 = rho;
    r -= alpha * q;
    rho = dot(r,r);

    if (rho < tol) return iter;
  }
  return max_iter;
}
