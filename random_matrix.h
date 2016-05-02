# Copyright (c) 2016 The Caroline authors. All rights reserved.
# Use of this source file is governed by a MIT license that can be found in the
# LICENSE file.
# Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef RANDOM_MATRIX_H
#define RANDOM_MATRIX_H

#include "matrix.h"
#include "interval.h"

#include "flint/arb_mat.h"
#include <string>

namespace interval {

// Generates random invertable matrix with small determenant.
// First generate random matrix A and then computes e^A.
// n - matrix dimension
// error_bound - number of digits in decimal part
// random - function for random number generation
// fname - output file name.
template<class Distribution>
Matrix<ArbInterval> random_matrix(size_t n, size_t error_bound,
        Distribution random) {
    const size_t prec = 1000;
        
    arb_mat_t A;
    arb_mat_t B;
    
    arb_mat_init(A, n, n);
    arb_mat_init(B, n, n);
    
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < n; ++j)
            arb_set_d(arb_mat_entry(A, i, j), random());
            
    for (size_t i = 0; i < n; ++i)
        arb_zero(arb_mat_entry(A, i, i));
            
    arb_mat_exp(B, A, prec);
  
    mag_t error;
    mag_init(error);
    mag_set_ui(error, 1);
    mag_div_ui(error, error, 10);
    mag_pow_ui(error, error, static_cast<slong>(error_bound));

    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < n; ++j)
            mag_set(arb_radref(arb_mat_entry(B, i, j)), error);
    
    Matrix<ArbInterval> result(n, n);
    
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < n; ++j)
            arb_set(result.at(i, j).data(), arb_mat_entry(B, i, j));

    mag_clear(error);
    arb_mat_clear(B);
    arb_mat_clear(A);
    
    return result;
}

// Generates random linear system using random_matrix().
// First generate random matrix A and then computes e^A.
// n - matrix dimension
// error_bound - number of digits in decimal part
// random - function for random number generation
// fname - output file name.
template<class Distribution>
Matrix<ArbInterval> random_linear_system(size_t n, size_t error_bound,
        Distribution random) {
    Matrix<ArbInterval> result(n, n + 1);
    Matrix<ArbInterval> temp = random_matrix(n, error_bound, random);
    
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < n; ++j)
            result.at(i, j) = temp.at(i, j);
            
    mag_t error;
    mag_init(error);
    mag_set_ui(error, 1);
    mag_div_ui(error, error, 10);
    mag_pow_ui(error, error, static_cast<slong>(error_bound));
            
    for (size_t i = 0; i < n; ++i) {
        result.at(i, n) = ArbInterval(random());
        mag_set(arb_radref(result.at(i, n).data()), error);
    }
        
    mag_clear(error);
    
    return result;
}

}  // namespace interval

#endif  // RANDOM_MATRIX_H

