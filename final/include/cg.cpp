//
// This file is part of the course materials for AMATH483/583 at the University
// of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0
// International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include "cg.hpp"
#include "Grid.hpp"
#include "Stencil.hpp"
#include <cmath>
#include <iostream>

size_t cg(const Stencil& A, Grid& x, const Grid& b, size_t max_iter, double tol) {

  Grid   r   = b - A * x, p(b);
  double rho = dot(r, r), rho_1 = 0.0;
  for (size_t iter = 0; iter < max_iter; ++iter) {

    std::cout << "||r|| = " << std::sqrt(rho) << std::endl;

    if (iter == 0) {
      p = r;
    } else {
      double beta = (rho / rho_1);
      p           = r + beta * p;
    }

    Grid   q     = A * p;
    double alpha = rho / dot(p, q);

    x += alpha * p;

    rho_1 = rho;
    r -= alpha * q;
    rho = dot(r, r);

    if (rho < tol){
      std::cout << "Sequential CG converged at iter=" << iter << "." << std::endl;
      return iter;
    }
  }
  std::cout << "Sequential CG failed to converge." << std::endl;
  return max_iter;
}
