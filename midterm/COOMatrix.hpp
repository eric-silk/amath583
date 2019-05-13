//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//
#ifndef __COOMATRIX_HPP
#define __COOMATRIX_HPP

#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <vector>
#include "Matrix.hpp"
#include "Vector.hpp"

class COOMatrix
{
    public:
        COOMatrix(size_t M, size_t N) : num_rows_(M), num_cols_(N) {}

        void push_back(size_t i, size_t j, double val)
        {
            assert(i < num_rows_ && i >= 0);
            assert(j < num_cols_ && j >= 0);

            row_indices_.push_back(i);
            col_indices_.push_back(j);
            storage_.push_back(val);
        }

        void clear()
        {
            row_indices_.clear();
            col_indices_.clear();
            storage_.clear();
        }

        void reserve(size_t n)
        {
            assert(n >= 0);

            row_indices_.reserve(n);
            col_indices_.reserve(n);
            storage_.reserve(n);
        }

        size_t num_rows()     const { return num_rows_;       }
        size_t num_cols()     const { return num_cols_;       }
        size_t num_nonzeros() const { return storage_.size(); }

        void stream_coordinates(std::ostream& output_file) const
        {
            for (size_t i = 0; i < num_nonzeros(); ++i)
            {
                output_file << row_indices_[i] << " ";
                output_file << col_indices_[i] << " ";
                output_file <<     storage_[i]       ;
                output_file << std::endl;
            }
        }


        void matvec(const Vector& x, Vector& y) const
        {
            for (size_t k = 0; k < storage_.size(); ++k)
            {
                y(row_indices_[k]) += storage_[k] * x(col_indices_[k]);
            }
        }

        void matmat(const Matrix& B, Matrix& C) const
        {
            // verify that MxN times NxP => MxP
            assert(num_cols_ == B.num_rows());
            assert(C.num_rows() == num_rows());
            assert(C.num_cols() == B.num_cols());

            // TODO
            // matmat is just repeated matvec. Blatantly abuse the above function :D
            for(size_t i=0; i<B.num_cols(); i++)
            {
                // Get the column and push it into a vector
                Vector col_vec(B.num_rows());
                Vector result_vec(B.num_rows());
                for(size_t j=0; j<B.num_rows(); j++)
                {
                    // step through the column and get all elements
                    col_vec(j) = B(j,i);
                }

                // Use the above function
                matvec(col_vec, result_vec);

                // Now push this into the correct column of the result
                for(size_t j=0; j<C.num_cols(); j++)
                {
                    C(j,i) = result_vec(j);
                }
            }

        }


    private:
        size_t              num_rows_, num_cols_;
        std::vector<size_t> row_indices_, col_indices_;
        std::vector<double> storage_;
};

#endif    // __COOMATRIX_HPP
