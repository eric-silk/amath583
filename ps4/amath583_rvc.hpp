#include <random>
#include <functional>
#include "RowMatrix.hpp"
#include "ColMatrix.hpp"

void   randomize(RowMatrix& A) {
  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(-1.0, 1.0);
  static auto dice = std::bind(distribution, generator);
  
  for (size_t i = 0; i < A.num_rows(); ++i) {
    for (size_t j = 0; j < A.num_cols(); ++j) {
      A(i, j) = dice();
    }
  }
}


void   randomize(ColMatrix& A) {
  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(-1.0, 1.0);
  static auto dice = std::bind(distribution, generator);
  
  for (size_t i = 0; i < A.num_rows(); ++i) {
    for (size_t j = 0; j < A.num_cols(); ++j) {
      A(i, j) = dice();
    }
  }
}



void   matmat(const RowMatrix& A, const RowMatrix& B, RowMatrix& C)
{
    /* WRITE ME */
    for (size_t i = 0; i < C.num_rows(); ++i)
    {
        for (size_t k = 0; k < C.num_cols(); ++k)
        {
            for (size_t j = 0; j < A.num_cols(); ++j)
            {
                C(i, j) += A(i, k) * B(k, j);
            }
        }
    }
}


void   matmat(const ColMatrix& A, const ColMatrix& B, ColMatrix& C)
{
    /* WRITE ME */
    for (size_t i = 0; i < C.num_cols(); ++i)
    {
        for (size_t k = 0; k < C.num_rows(); ++k)
        {
            for (size_t j = 0; j < A.num_rows(); ++j)
            {
                C(j, i) += A(k, i) * B(j, k);
            }
        }
    }
}
