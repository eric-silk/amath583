//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef __CSCMATRIX_HPP
#define __CSCMATRIX_HPP

#include <iostream>
#include "Vector.hpp"
#include <algorithm>
#include <cassert>
#include <vector>

class CSCMatrix
{
    public:
        CSCMatrix(size_t M, size_t N) : is_open(false),
                                        num_rows_(M),
                                        num_cols_(N),
                                        col_indices_(num_cols_ + 1, 0) {}

        void open_for_push_back()
        {
            is_open = true;
        }

        void close_for_push_back()
        {
            is_open = false;
            for (size_t i = 0; i < num_cols_; ++i)
            {
                col_indices_[i + 1] += col_indices_[i];
            }
            for (size_t i = num_cols_; i > 0; --i)
            {
                col_indices_[i] = col_indices_[i - 1];
            }
            col_indices_[0] = 0;
        }

        void push_back(size_t i, size_t j, double value)
        {
            assert(is_open);
            assert(i < num_cols_ && i >= 0);
            assert(j < num_rows_ && j >= 0);
            assert(i < col_indices_.size() && i >= 0);

            ++col_indices_[i];
            row_indices_.push_back(j);
            storage_.push_back(value);
        }

        void clear()
        {
            row_indices_.clear();
            storage_.clear();
            std::fill(col_indices_.begin(), col_indices_.end(), 0);
        }

        size_t num_rows() const { return num_rows_; }
        size_t num_cols() const { return num_cols_; }
        size_t num_nonzeros() const { return storage_.size(); }

        void stream_coordinates(std::ostream& output_file) const
        {
            for (size_t i = 0; i < num_cols_; ++i)
            {
                for (size_t j = col_indices_[i]; j < col_indices_[i+1]; ++j)
                {
                    output_file << i               << " ";
                    output_file << row_indices_[j] << " ";
                    output_file <<     storage_[j]       ;
                    output_file << std::endl;
                }
            }
        }

        void matvec(const Vector& x, Vector& y) const
        {
            assert(x.num_rows() == y.num_rows());
            assert(num_cols_ == y.num_rows());
            for (size_t col = 0; col < num_cols_; ++col)
            {
                for (size_t j = col_indices_[col]; j < col_indices_[col+1]; ++j)
                {
                    //y(i) += storage_[j] * x(row_indices_[j]);
                    y(row_indices_[j]) += storage_[j] * x(col);
                }
            }
        }

        void matmat(const Matrix& B, Matrix& C) const {  /* Write Me for Extra Credit*/  }


    private:
        bool                is_open;
        size_t              num_rows_, num_cols_;
        std::vector<size_t> col_indices_, row_indices_;
        std::vector<double> storage_;
};

#endif    // __CSCMATRIX_HPP
