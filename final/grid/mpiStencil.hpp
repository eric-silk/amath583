//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef __MPI_STENCIL_HPP
#define __MPI_STENCIL_HPP

#include "Grid.hpp"

class mpiStencil { };

Grid operator*(const mpiStencil& A, const Grid& x);
void update_halo(Grid& x);

#endif // __MPI_STENCIL_HPP
