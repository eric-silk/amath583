//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//
#ifndef __CSRMATRIX_HPP
#define __CSRMATRIX_HPP

#include "Vector.hpp"
#include "Matrix.hpp"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

class CSRMatrix
{
    public:
        CSRMatrix(size_t M, size_t N) : is_open(false),
                                        num_rows_(M),
                                        num_cols_(N),
                                        row_indices_(num_rows_ + 1, 0) {}

        void open_for_push_back()
        {
            is_open = true;
        }

        void close_for_push_back()
        {
            is_open = false;
            for (size_t i = 0; i < num_rows_; ++i)
            {
                row_indices_[i + 1] += row_indices_[i];
            }
            for (size_t i = num_rows_; i > 0; --i)
            {
                row_indices_[i] = row_indices_[i - 1];
            }
            row_indices_[0] = 0;
        }

        void push_back(size_t i, size_t j, double value)
        {
            assert(is_open);
            assert(i < num_rows_ && i >= 0);
            assert(j < num_cols_ && j >= 0);

            ++row_indices_[i];
            col_indices_.push_back(j);
            storage_.push_back(value);
        }

        void clear()
        {
            col_indices_.clear();
            storage_.clear();
            std::fill(row_indices_.begin(), row_indices_.end(), 0);
        }

        size_t num_rows() const { return num_rows_; }
        size_t num_cols() const { return num_cols_; }
        size_t num_nonzeros() const { return storage_.size(); }

        void stream_coordinates(std::ostream& output_file) const
        {
            for (size_t i = 0; i < num_rows_; ++i)
            {
                for (size_t j = row_indices_[i]; j < row_indices_[i+1]; ++j)
                {
                    output_file << i               << " ";
                    output_file << col_indices_[j] << " ";
                    output_file <<     storage_[j]       ;
                    output_file << std::endl;
                }
            }
        }

        void matvec(const Vector& x, Vector& y) const
        {
            assert(x.num_rows() == y.num_rows());
            assert(num_cols_ == y.num_rows());
            std::cout << "CSRMatrix" << std::endl;
            std::cout << "i j" << std::endl << "------------------" << std::endl;
            for (size_t i = 0; i < num_rows_; ++i)
            {
                for (size_t j = row_indices_[i]; j < row_indices_[i+1]; ++j)
                {
                    std::cout << i << " " << j << std::endl;
                    y(i) += storage_[j] * x(col_indices_[j]);
                }
            }
        }

        void matmat(const Matrix& B, Matrix& C) const
        {
            // verify that MxN times NxP => MxP
            assert(num_cols_ == B.num_rows());
            assert(C.num_rows() == B.num_cols());

            // matmat is just repeated matvec. Blatantly abuse that function :D
            for(size_t i=0; i<B.num_cols(); ++i)
            {
                // Get the column and push it into a vector
                Vector col_vec(B.num_rows());
                Vector result_vec(B.num_rows());
                for(size_t j=0; j<B.num_rows(); ++j)
                {
                    // step through the column and get all elements
                    col_vec(j) = B(j,i);
                }

                // Use the above function
                matvec(col_vec, result_vec);

                // Now push this into the correct column of the result
                for(size_t j=0; j<C.num_rows(); ++j)
                {
                    C(j,i) = result_vec(j);
                }
            }
        }


    private:
        bool                is_open;
        size_t              num_rows_, num_cols_;
        std::vector<size_t> row_indices_, col_indices_;
        std::vector<double> storage_;
};

#endif    // __CSRMATRIX_HPP
