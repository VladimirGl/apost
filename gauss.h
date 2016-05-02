#ifndef GAUSS_H
#define GAUSS_H

#include "matrix.h"

/*
    This file contains Gaussian elimination methods.
*/

namespace interval {

// Finds the pivot element for Gaussian elimination.
template<class IntervalT>
int find_pivot(const Matrix<IntervalT> &matrix, size_t r, size_t c) {
    int best_row = -1;
    
    for (size_t i = r; i < matrix.nrow(); ++i) {
        // if element at (i, c) does not contain zero
        if (!((ArbInterval)(matrix.at(i, c))).contains_zero()) {
            if (best_row == -1) {
                best_row = i;
            // if abs(new) upper bound > abs(old) upper bound
            } else if (((ArbInterval)(matrix.at(i, c))).abs_ubound() >
                        ((ArbInterval)(matrix.at(best_row, c))).abs_ubound()) {
                best_row = i;
            }
        }
    }
    
    return best_row;
}

// Performs the Gaussian elimination of matrix (without pivoting).
template<class IntervalT>
void gauss_elimination(Matrix<IntervalT>& matrix) {
    size_t n = matrix.nrow();
    size_t m = matrix.ncol();
    
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            IntervalT z = matrix.at(j, i) / matrix.at(i, i);
            matrix.at(j, i) = z;
            for (size_t k = i + 1; k < m; ++k) {
                IntervalT t = z * matrix.at(i, k);
                matrix.at(j, k) = matrix.at(j, k) - t;
            }
        }
    }
}

// Performs the Gaussian elimination of matrix (with pivoting).
// Returns (-1)^(number of permutations).
template<class IntervalT>
int gauss_elimination_pivot(Matrix<IntervalT>& matrix) {
    size_t n = matrix.nrow();
    size_t m = matrix.ncol();
    
    int sign = 1;

    for (size_t i = 0; i < n; ++i) {
        int r = find_pivot(matrix, i, i);
        if (i != r) {
            for (size_t j = 0; j < m; ++j) {
                std::swap(matrix.at(i, j), matrix.at(r, j));
            }
            sign *= -1;
        }
        
        for (size_t j = i + 1; j < n; ++j) {
            IntervalT z = matrix.at(j, i) / matrix.at(i, i);
            matrix.at(j, i) = z;
            for (size_t k = i + 1; k < m; ++k) {
                IntervalT temp = z * matrix.at(i, k);
                matrix.at(j, k) = matrix.at(j, k) - temp;
            }
        }
    }
    
    return sign;
}

}  // namespace interval

#endif  // GAUSS_H

