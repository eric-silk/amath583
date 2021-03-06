//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef AMATH_583_GREEN_BELT_VOVMATRIX_HPP
#define AMATH_583_GREEN_BELT_VOVMATRIX_HPP

#include <assert.h>
#include <vector>

class VoVMatrix {
public:
  VoVMatrix(size_t M, size_t N) : num_rows_(M), num_cols_(N), storage_(M, std::vector<double>(N)) {}

  double& operator()(size_t i, size_t j)
  {
      assert(i < num_rows_ && j < num_cols_);
      return storage_[i][j];
  }
  const double& operator()(size_t i, size_t j) const
  {
      assert(i < num_rows_ && j < num_cols_);
      return storage_[i][j];
  }

  size_t num_rows() const { return num_rows_; }
  size_t num_cols() const { return num_cols_; }

private:
  size_t                           num_rows_, num_cols_;
  std::vector<std::vector<double>> storage_;
};

#endif    // AMATH_583_GREEN_BELT_VOVMATRIX_HPP
