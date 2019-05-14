//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//
#ifndef __AOS_HPP
#define __AOS_HPP

#include <cassert>
#include <tuple>
#include <vector>

#include "Vector.hpp"


class AOSMatrix
{
    private:
        typedef std::tuple<size_t, size_t, double> element;

        public:
        AOSMatrix(size_t M, size_t N) : num_rows_(M), num_cols_(N) {}

        void push_back(size_t i, size_t j, double val)
        {
            assert(i < num_rows_ && i >= 0);
            assert(j < num_cols_ && j >= 0);

            storage_.push_back(element(i, j, val));
        }

        void clear() { storage_.clear(); }

        void reserve(size_t n)
        {
            assert(n >= 0);
            storage_.reserve(n);
        }

        size_t num_rows() const { return num_rows_; }
        size_t num_cols() const { return num_cols_; }
        size_t num_nonzeros() const { return storage_.size(); }

        void stream_coordinates(std::ostream& output_file) const
        {
            for (size_t i = 0; i < num_nonzeros(); ++i)
            {
                // TODO create constant types (or an ENUM?) for tuple access
                // moar redable tht wai
                output_file << std::get<0>(storage_[i]) << " ";
                output_file << std::get<1>(storage_[i]) << " ";
                output_file << std::get<2>(storage_[i])       ;
                output_file << std::endl;
            }
        }

        void matvec(const Vector& x, Vector& y) const
        {
            for (size_t k = 0; k < storage_.size(); ++k)
            {
                y(std::get<0>(storage_[k])) += std::get<2>(storage_[k]) * x(std::get<1>(storage_[k]));
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
        size_t               num_rows_, num_cols_;
        std::vector<element> storage_;
};

#endif    // __AOS_HPP
