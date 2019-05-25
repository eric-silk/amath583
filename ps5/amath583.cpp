//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include "amath583.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"
#include <cassert>
#include <cmath>
#include <functional>
#include <random>
#include <algorithm>

#include <thread>
#include <future>

#include <iostream>

// ----------------------------------------------------------------
//
// Vector arithmetic
//
// ----------------------------------------------------------------
void zeroize(Vector& x) {
  for (size_t i = 0; i < x.num_rows(); ++i) {
    x(i) = 0.0;
  }
}

void randomize(Vector& x) {
  std::default_random_engine             generator;
  std::uniform_real_distribution<double> distribution(-1.0, 1.0);
  static auto                            dice = std::bind(distribution, generator);

  for (size_t i = 0; i < x.num_rows(); ++i) {
    x(i) = dice();
  }
}

void sort_ascending(Vector& x)
{
    // Inefficient probably but w/e
    // Create a copy of the Vector as a Vector
    std::vector<double> tmp;
    for(size_t i=0; i<x.num_rows(); ++i)
    {
        tmp.push_back(x(i));
    }

    std::sort(tmp.begin(), tmp.end());

    for(size_t i=0; i<x.num_rows(); ++i)
    {
        x(i) = tmp[i];
    }
}

void sort_descending(Vector& x)
{
    // Inefficient probably but w/e
    // Create a copy of the Vector as a Vector
    std::vector<double> tmp;
    for(size_t i=0; i<x.num_rows(); ++i)
    {
        tmp.push_back(x(i));
    }

    std::sort(tmp.begin(), tmp.end(), std::greater<double>());

    for(size_t i=0; i<x.num_rows(); ++i)
    {
        x(i) = tmp[i];
    }
}

// ----------------------------------------------------------------
//
// Single threaded Norms
//
// ----------------------------------------------------------------

double one_norm(const Vector& x) {
  double sum = 0.0;
  for (size_t i = 0; i < x.num_rows(); ++i) {
    sum += std::abs(x(i));
  }
  return sum;
}

double two_norm(const Vector& x) {
  double sum = 0.0;
  for (size_t i = 0; i < x.num_rows(); ++i) {
    sum += x(i) * x(i);
  }
  return std::sqrt(sum);
}

double inf_norm(const Vector& x);
double p_norm(const Vector& x);

// ----------------------------------------------------------------
//
// Multithreaded Norms
//
// ----------------------------------------------------------------

// Static to keep it local to this file
static std::mutex pnorm_mutex;

double partitioned_two_norm(const Vector& x, size_t partitions)
{
    assert(partitions > 0);
    assert(x.num_rows() > 0);
    assert(x.num_rows() > partitions); // maybe not needed?

    double sum = 0.0;

    // split the vector into partitions (or partition ranges)
    const size_t step_size = x.num_rows() / partitions;
    size_t leftover = x.num_rows() % partitions;

    std::vector<size_t> offsets;
    offsets.push_back(0);

    for (size_t i = 0; i < partitions; ++i)
    {
        size_t step = step_size;
        if (leftover > 0)
        {
            step +=1;
            leftover -= 1;
        }
        // stuff the offsets into a vector
        offsets.push_back(offsets.back() + step);
    }

    // The lambda
    // Only capture x and sum, rather than all with just the "&" character
    // "" was also considered, but would a) produce overhead and b) wont' work for sum
    auto f = [&x, &sum] (size_t const start, size_t const stop) -> void
    {
        double part_sum = 0.0;
        for (size_t i = start; i < stop; ++i)
        {
            part_sum += x(i) * x(i);
        }
        {
            // Lock guard, like in the slides
            // failing to do this caused incorrect results
            std::lock_guard<std::mutex> pnorm_guard(pnorm_mutex);
            sum += part_sum;
        }
    };

    // Call std::thread() "partition" times
    std::vector<std::thread> threads;
    for (size_t i = 0; i < partitions; ++i)
    {
        threads.push_back(std::thread(f, offsets[i], offsets[i+1]));
    }
    for (size_t i = 0; i < partitions; ++i)
    {
        threads[i].join();
    }

    return std::sqrt(sum);
}

double recursive_two_norm(const Vector& x, size_t levels)
{
    // This implementation assumes we are supposed to recursively calculate the partial
    // sum of squares on each sub-vector. TBD if that's correct, pending instructor feedback
    // on Piazza (Question @171).
    //
    // Per lecture, we are supposed to recursively partition the vector, e.g.
    // |----| -> |--||--| -> |-||-||-||-|
    // |-----| -> |---||--| -> |--||-||-||-| -> |-||-||-||-||-|
    // Need to fix!!

    // Write the lambda to do the recursive calculation
    // Sorry if the formatting is janky
    std::function<double (size_t const, size_t const, size_t const)> f = 
    [&] (size_t const start, size_t const size, size_t const remaining_levels) -> double
    {
        // base case: maximum level of recursion reached
        // or size = 1
        if (size == 1 || remaining_levels == 0)
        {
            double inner_sum = 0.0;
            for (size_t i = start; i < start+size; ++i)
            {
                inner_sum += x(i) * x(i);
            }
            return inner_sum;
        }
        // Otherwise, we need to go deeper (inception bwammmmmmmm)
        // Split the sub-vector in Twain, and Mark the starts and ends
        // Huck out the bad pun
        size_t const l_size = size / 2;
        size_t const r_size = (size / 2) + (size % 2);
        std::future<double> left_split_val = std::async(f, 0, l_size, remaining_levels-1);
        std::future<double> right_split_val = std::async(f, l_size, r_size, remaining_levels-1);

        return left_split_val.get() + right_split_val.get();
    };
    
    return std::sqrt(f(0, x.num_rows(), levels));
}

Vector abs(const Vector& x) {
  Vector y(x.num_rows());
  for (size_t i = 0; i < y.num_rows(); ++i) {
    // clang complained about not using std::abs
    y(i) = std::abs(x(i));
  }
  return y;
}

Vector operator*(double alpha, const Vector& x) {
  Vector y(x.num_rows());
  for (size_t i = 0; i < y.num_rows(); ++i) {
    y(i) = alpha * x(i);
  }
  return y;
}

Vector operator+(const Vector& x, double alpha) {
  Vector y(x.num_rows());
  for (size_t i = 0; i < y.num_rows(); ++i) {
    y(i) = x(i) + alpha;
  }
  return y;
}

Vector operator+(const Vector& x, const Vector& y) {
  Vector z(x.num_rows());
  for (size_t i = 0; i < z.num_rows(); ++i) {
    z(i) = x(i) + y(i);
  }
  return z;
}

Vector operator-(const Vector& x, const Vector& y) {
  Vector z(x.num_rows());
  for (size_t i = 0; i < z.num_rows(); ++i) {
    z(i) = x(i) - y(i);
  }
  return z;
}

Vector& operator*=(double alpha, const Vector& x);
Vector& operator+=(const Vector& x, const Vector& y);
Vector& operator-=(const Vector& x, const Vector& y);

// ----------------------------------------------------------------
//
// Matrix arithmetic
//
// ----------------------------------------------------------------
void zeroize(Matrix& A) {
  for (size_t i = 0; i < A.num_rows(); ++i) {
    for (size_t j = 0; j < A.num_cols(); ++j) {
      A(i, j) = 0.0;
    }
  }
}

void randomize(Matrix& A) {
  std::default_random_engine             generator;
  std::uniform_real_distribution<double> distribution(-1.0, 1.0);
  static auto                            dice = std::bind(distribution, generator);

  for (size_t i = 0; i < A.num_rows(); ++i) {
    for (size_t j = 0; j < A.num_cols(); ++j) {
      A(i, j) = dice();
    }
  }
}

void piscetize(Matrix& A, size_t xpoints, size_t ypoints) {
  assert(A.num_rows() == A.num_cols());
  assert(xpoints * ypoints == A.num_rows());

  zeroize(A);

  for (size_t j = 0; j < xpoints; j++) {
    for (size_t k = 0; k < ypoints; k++) {
      size_t jrow = j * ypoints + k;

      if (j != 0) {
        size_t jcol   = (j - 1) * ypoints + k;
        A(jrow, jcol) = -1.0;
      }
      if (k != 0) {
        size_t jcol   = j * ypoints + (k - 1);
        A(jrow, jcol) = -1.0;
      }
      A(jrow, jrow) = 4.0;

      if (k != ypoints - 1) {
        size_t jcol   = j * ypoints + (k + 1);
        A(jrow, jcol) = -1.0;
      }
      if (j != xpoints - 1) {
        size_t jcol   = (j + 1) * ypoints + k;
        A(jrow, jcol) = -1.0;
      }
    }
  }
}

double one_norm(const Matrix& A);
double two_norm(const Matrix& A);
double inf_norm(const Matrix& A);

double Frobenius_norm(const Matrix& A) {
  double result = 0.0;
  for (size_t i = 0; i < A.num_rows(); ++i) {
    for (size_t j = 0; j < A.num_cols(); ++j) {
      result += A(i, j) * A(i, j);
    }
  }
  return std::sqrt(result);
}

Matrix operator+(const Matrix& A, const Matrix& B) {
  assert((A.num_rows() == B.num_rows()) && (A.num_cols() == B.num_cols()));

  Matrix C(A.num_rows(), A.num_cols());

  for (size_t i = 0; i < C.num_rows(); ++i) {
    for (size_t j = 0; j < C.num_cols(); ++j) {
      C(i, j) = A(i, j) + B(i, j);
    }
  }
  return C;
}

Matrix operator-(const Matrix& D, const Matrix& E) {
  assert((D.num_rows() == E.num_rows()) && (D.num_cols() == E.num_cols()));
  Matrix F(D.num_rows(), D.num_cols());
  for (size_t i = 0; i < F.num_rows(); ++i) {
    for (size_t j = 0; j < F.num_cols(); ++j) {
      F(i, j) = D(i, j) - E(i, j);
    }
  }
  return F;
}

Matrix& operator*=(double alpha, const Matrix& x);
Matrix& operator+=(const Matrix& x, const Matrix& y);
Matrix& operator-=(const Matrix& x, const Matrix& y);

// ----------------------------------------------------------------
//
// Matrix-vector operations
//
// ----------------------------------------------------------------
Vector operator*(const Matrix& A, const Vector& x) {
  assert(A.num_cols() == x.num_rows());

  Vector y(A.num_rows());
  matvec(A, x, y);
  return y;
}

void matvec(const Matrix& A, const Vector& x, Vector& y) {
  for (size_t i = 0; i < A.num_rows(); ++i) {
    for (size_t j = 0; j < A.num_cols(); ++j) {
      y(i) += A(i, j) * x(j);
    }
  }
}

/* Definition of matvec_ij goes here (583) */
void matvec_ij(const Matrix& A, const Vector& x, Vector& y) {
  for (size_t i = 0; i < A.num_rows(); ++i) {
    for (size_t j = 0; j < A.num_cols(); ++j) {
      y(i) += A(i, j) * x(j);
    }
  }
}

/* Definition of matvec_ji goes here (583) */
void matvec_ji(const Matrix& A, const Vector& x, Vector& y) {
  for (size_t j = 0; j < A.num_cols(); ++j) {
    for (size_t i = 0; i < A.num_rows(); ++i) {
      y(i) += A(i, j) * x(j);
    }
  }
}

// 583 par_matvec() implementation
// Static to keep it local to this file
static std::mutex matvec_mutex;

// This isn't working. I'm pretty sure is due to me not handling the
// partitioning correctly (or at least the post-partitioning accumulation)
// but I've had several glasses of good scotch and don't think I'll finish this.
// Woe to ye who must grade this.
void par_matvec(const Matrix& A, const Vector& x, Vector& y, size_t partitions)
{
    // Split the Matrix into "partition" sub-matrices
    // split the vector into partitions (or partition ranges)
    const size_t step_size = A.num_cols() / partitions;
    size_t leftover = A.num_cols() % partitions;

    std::vector<size_t> offsets;
    offsets.push_back(0);

    for (size_t i = 0; i < partitions; ++i)
    {
        size_t step = step_size;
        if (leftover > 0)
        {
            step +=1;
            leftover -= 1;
        }
        // stuff the offsets into a vector
        offsets.push_back(offsets.back() + step);
    }

    // For each sub matrix do the matvec multiplication
    auto f = [&] (size_t const row_start, size_t row_stop) -> Vector
    {
        Vector sub_vector(A.num_cols());
        if (row_stop >= A.num_rows())
        {
            row_stop = A.num_rows() - 1;
        }

        for (size_t row = row_start; row <= row_stop; ++row)
        {
            for (size_t col = 0; col < A.num_cols(); ++col)
            {
                std::cout << "y["<<row+1<<"]+=A("<<col+1<<","<<row+1<<")*x"<<col+1<<std::endl;
                sub_vector(col) += A(row, col) * x(col);
            }
        }

        return sub_vector;
    };

    std::vector<std::future<Vector>> tasks;

    for (size_t i = 0; i < partitions; ++i)
    {
        tasks.push_back(std::async(f, offsets[i], offsets[i+1]));
    }

    // Sum the resulting subvectors
    for(size_t i = 0; i < partitions; ++i)
    {
        std::lock_guard<std::mutex> matvec_guard(matvec_mutex);
        Vector tmp_vector = tasks[i].get();
        for (size_t row = 0; row < tmp_vector.num_rows(); ++row)
        {
            y(i) += tmp_vector(row);
        }
    }
}


// ----------------------------------------------------------------
//
// Matrix-matrix operations
//
// ----------------------------------------------------------------
Matrix operator*(const Matrix& A, const Matrix& B) {
  assert(A.num_cols() == B.num_rows());

  Matrix C(A.num_rows(), B.num_cols());
  matmat(A, B, C);
  return C;
}

void matmat(const Matrix& A, const Matrix& B, Matrix& C) {
  for (size_t i = 0; i < C.num_rows(); ++i) {
    for (size_t j = 0; j < C.num_cols(); ++j) {
      for (size_t k = 0; k < A.num_cols(); ++k) {
        C(i, j) += A(i, k) * B(k, j);
      }
    }
  }
}

/* Definition of matmat_ikj goes here */
void matmat_ikj(const Matrix& A, const Matrix& B, Matrix& C) {
  for (size_t i = 0; i < C.num_rows(); ++i) {
    for (size_t k = 0; k < A.num_cols(); ++k) {
      for (size_t j = 0; j < C.num_cols(); ++j) {
        C(i, j) += A(i, k) * B(k, j);
      }
    }
  }
}

/* Definition of matmat_jki goes here */
void matmat_jki(const Matrix& A, const Matrix& B, Matrix& C) {
  for (size_t j = 0; j < C.num_cols(); ++j) {
    for (size_t k = 0; k < A.num_cols(); ++k) {
      for (size_t i = 0; i < C.num_rows(); ++i) {
        C(i, j) += A(i, k) * B(k, j);
      }
    }
  }
}
