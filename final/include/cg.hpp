//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef __CG_HPP
#define __CG_HPP

#include "Grid.hpp"
#include "Stencil.hpp"

size_t cg(const Stencil& A, Grid& x, const Grid& b, size_t max_iter, double tol);

#endif    // __CG_HPP
