//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef __GRID_HPP
#define __GRID_HPP

#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>

class Grid {

public:
  explicit Grid(size_t x, size_t y) : x_points_(x + 2), y_points_(y + 2), storage_(x_points_ * y_points_) {}
  explicit Grid(size_t x, size_t y, double init) : x_points_(x + 2), y_points_(y + 2), storage_(x_points_ * y_points_, init) {}

  double&       operator()(size_t i, size_t j) { return storage_[i * y_points_ + j]; }
  const double& operator()(size_t i, size_t j) const { return storage_[i * y_points_ + j]; }

  size_t num_x() const { return x_points_; }
  size_t num_y() const { return y_points_; }

  void swap(Grid& x) {
    std::swap(x.x_points_, x_points_);
    std::swap(x.y_points_, y_points_);
    storage_.swap(x.storage_);
  }

  void operator=(const Grid& x) {
    assert(x.x_points_ == x_points_ && x.y_points_ == y_points_);
    std::copy(x.storage_.begin(), x.storage_.end(), storage_.begin());
  }

private:
  size_t              x_points_, y_points_;
  std::vector<double> storage_;
};

Grid   operator-(const Grid& X, const Grid& Y);
Grid   operator+(const Grid& X, const Grid& Y);
void   operator+=(Grid& Z, const Grid& X);
void   operator-=(Grid& Z, const Grid& X);
Grid   operator*(double a, const Grid& Y);
double dot(const Grid& X, const Grid& Y);
double jacobiStep(const Grid& x, Grid& y);
void   swap(Grid& x, Grid& y);

#endif    // __GRID_HPP
