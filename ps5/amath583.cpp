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

// Static to keep it local to this file
static std::mutex rnorm_mutex;

double recursive_two_norm(const Vector& x, size_t levels)
{
    // This implementation assumes we are supposed to recursively calculate the partial
    // sum of squares on each sub-vector. TBD if that's correct, pending instructor feedback
    // on Piazza (Question @171).

    double sum = 0.0;

    // Determine the number of partitions based upon the maximum level of recursion
    // "0" levels of recursion is 1 level deep for this
    levels += 1;
    size_t partitions = x.num_rows() / levels;
    if (x.num_rows() % levels)
    {
        partitions += 1;
    }

    /*
    std::cout << "Levels:" << levels << std::endl;
    std::cout << "x.num_rows(): " << x.num_rows() << std::endl;
    std::cout << "Num parts: " << partitions << std::endl;
    */

    // create the initial offsets, each at least "levels" apart from each other
    std::vector<size_t> offsets;
    for (size_t i = 0; i < partitions; ++i)
    {
        offsets.push_back(i * levels);
    }

    // Write the lambda to do the recursive calculation
    std::function<double (size_t, size_t)> f = [&] (size_t const l_offset, size_t const l_level) -> double
    {
        assert(l_offset < x.num_rows());
        assert(l_level >= 0);
        // The base case: max recursion or end of the Vector
        if (l_level < 1)
        {
            // min level of recursion
            return x(l_offset) * x(l_offset);
        }
        else if(l_offset >= x.num_rows()-1)
        {
            // end of the Vector
            return x(l_offset) * x(l_offset);
        }
        else
        {
            return (x(l_offset) * x(l_offset)) + f(l_offset+1, l_level-1);
        }
    };
    
    // Call std::async for creating multiple tasks
    // a vector of futures of double...MOAR TEMPLATES
    std::vector<std::future<double>> part_sum_futures;
    for(size_t i = 0; i < partitions; ++i)
    {
        part_sum_futures.push_back(std::async(f, offsets[i], levels));
        std::lock_guard<std::mutex> rnorm_guard(rnorm_mutex);
        sum += part_sum_futures[i].get();
    }

    assert(partitions == part_sum_futures.size());
    for(size_t i = 0; i < partitions; ++i)
    {
        //nada
    }
    
    return std::sqrt(sum);
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

/* Definition of matvec_ji goes here (583) */
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

void par_matvec(const Matrix& A, const Vector& x, Vector& y, size_t partitions)
{
    // TODO
    // Split the Matrix into "partition" sub-matrices, each "partition" rows by J columns
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

    std::vector<Vector> subvectors;
    for (size_t i = 0; i < partitions; ++i)
    {
        subvectors.push_back(Vector(y.num_rows()));
    }

    // For each sub matrix do the matvec multiplication
    // Consider an std::vector of subvectors and then summing them for better
    // performance, time allowing
    auto f = [&] (size_t const row_start, size_t row_stop, size_t const partition) -> void
    {
        if (row_stop >= A.num_cols())
        {
            row_stop = A.num_cols() - 1;
        }
        assert(row_start >= 0);
        assert(row_stop < A.num_cols());

        for (size_t j = 0; j < A.num_cols(); ++j)
        {
            for (size_t i = row_start; i < A.num_rows(); ++i)
            {
                subvectors[partition](i) += A(i, j) * x(j);
            }
        }
    };

    // Call std::thread() "partition" times
    std::vector<std::thread> threads;
    for (size_t i = 0; i < partitions; ++i)
    {
        threads.push_back(std::thread(f, offsets[i], offsets[i+1], i));
    }
    for (size_t i = 0; i < partitions; ++i)
    {
        threads[i].join();
    }

    // Sum the resulting subvectors
    for(size_t i = 0; i < subvectors.size(); ++i)
    {
        for (size_t row = 0; row < y.num_rows(); ++row)
        {
            y(row) += subvectors[i](row);
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
