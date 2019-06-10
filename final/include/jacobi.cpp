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

#include "ir.hpp"
#include "Grid.hpp"
#include "Stencil.hpp"
#include <cmath>
#include <iostream>

size_t jacobi(const Stencil& A, Grid& x, const Grid& b, size_t maxiter, double tol) {

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
    std::cout << "||r|| = " << std::sqrt(rho) << std::endl;
    if (std::sqrt(rho) < tol) return iter;
    swap(x, y);
  }
    
  return maxiter;
}

