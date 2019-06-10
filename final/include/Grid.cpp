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

#include "Grid.hpp"
#include <cassert>
#include <cmath>
#include <iostream>

Grid operator-(const Grid& X, const Grid& Y) {
  Grid Z(X);
  for (size_t i = 0; i < Z.num_x(); ++i) {
    for (size_t j = 0; j < Z.num_y(); ++j) {
      Z(i, j) = X(i, j) - Y(i, j);
    }
  }
  return Z;
}

Grid operator+(const Grid& X, const Grid& Y) {
  Grid Z(X);
  for (size_t i = 0; i < Z.num_x(); ++i) {
    for (size_t j = 0; j < Z.num_y(); ++j) {
      Z(i, j) = X(i, j) + Y(i, j);
    }
  }
  return Z;
}

void operator+=(Grid& Z, const Grid& X) {
  for (size_t i = 0; i < Z.num_x(); ++i) {
    for (size_t j = 0; j < Z.num_y(); ++j) {
      Z(i, j) += X(i, j);
    }
  }
}

void operator-=(Grid& Z, const Grid& X) {
  for (size_t i = 0; i < Z.num_x(); ++i) {
    for (size_t j = 0; j < Z.num_y(); ++j) {
      Z(i, j) -= X(i, j);
    }
  }
}

Grid operator*(double a, const Grid& Y) {
  Grid Z(Y);
  for (size_t i = 1; i < Z.num_x() - 1; ++i) {
    for (size_t j = 1; j < Z.num_y() - 1; ++j) {
      Z(i, j) = a * Y(i, j);
    }
  }
  return Z;
}

double dot(const Grid& X, const Grid& Y) {
  double sum = 0.0;
  for (size_t i = 0; i < X.num_x(); ++i) {
    for (size_t j = 0; j < X.num_y(); ++j) {
      sum += X(i, j) * Y(i, j);
    }
  }
  return sum;
}


void swap(Grid& x, Grid& y) { x.swap(y); }

