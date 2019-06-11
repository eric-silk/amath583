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

size_t ir(const Stencil& A, Grid& x, const Grid& b, size_t max_iter, double tol) {
  for (size_t iter = 0; iter < max_iter; ++iter) {
    Grid   r   = b - A * x;
    double rho = dot(r, r);
    std::cout << "||r|| = " << std::sqrt(rho) << std::endl;
    if (std::sqrt(rho) < tol){
      std::cout << "IR sequential converged at iter=" << iter << "." << std::endl;
      return iter;
    }
    x += r;
  }
  std::cout << "IR sequential failed to converge." << std::endl;
  return max_iter;
}
