# Copyright (c) 2016 The Caroline authors. All rights reserved.
# Use of this source file is governed by a MIT license that can be found in the
# LICENSE file.
# Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef LEQS_H
#define LEQS_H

#include "apost_statical.h"
#include "gauss.h"
#include "interval.h"
#include "matrix.h"

namespace interval {

// Solves the linear equation system using Gauss elimination method
// for aposteriori ProxyInterval values (withoud pivoting).
template<class IntervalT>
Matrix<apost::ProxyIntervalResult> linear_solve_apost(Matrix<IntervalT> M) {
    size_t n = M.nrow();

    gauss_elimination(M);
    
    Matrix<apost::ProxyIntervalResult> x(n, 1);
    Matrix<IntervalT> temp(n, 1);
    for (int i = n - 1; i >= 0; --i) {
        temp.at(i, 0) = M.at(i, n);
        for (int j = i + 1; j < n; ++j) {
            IntervalT t = M.at(i, j) * temp.at(j, 0);
            temp.at(i, 0) = temp.at(i, 0) - t;
        }
        
        temp.at(i, 0) = temp.at(i, 0) / M.at(i, i);
        x.at(i, 0) = temp.at(i, 0);
    }
    
    return x;
}

// Solves the linear equation system using Gauss elimination method
// (withoud pivoting). Can't be used with dynamic aposteriori method due to 
// many output values.
template<class IntervalT>
Matrix<IntervalT> linear_solve(Matrix<IntervalT> &M) {
    size_t n = M.nrow();

    gauss_elimination(M);
    
    Matrix<IntervalT> x(n, 1);
    for (int i = n - 1; i >= 0; --i) {
        x.at(i, 0) = M.at(i, n);
        for (int j = i + 1; j < n; ++j) {
            x.at(i, 0) = x.at(i, 0) - M.at(i, j) * x.at(j, 0);
        }
        x.at(i, 0) = x.at(i, 0) / M.at(i, i);
    }

    return x;
}

// Computes the solution of system of linear equations using Gaussian
// elimination (with pivoting) and statical implementation
// of aposteriori method.
Matrix<ArbInterval> leq_inv(Matrix<ArbInterval> M) {
    Matrix<ArbInterval> init = M;

    size_t n = M.nrow();
    size_t m = M.ncol();
    
    if (n + 1 != m) {
        return Matrix<ArbInterval>(1, 1, 0);
    }
    
    if (n == 0) {
        return Matrix<ArbInterval>(1, 1, 0);
    }
    
    if (n == 1) {
        return Matrix<ArbInterval>(1, 1, 0);
    }
    
    Matrix<ArbInterval> xs = linear_solve(M);

    for (int c = 0; c < n; ++c) {
    
        ArbInterval f = xs.at(c, 0);
        ArbInterval x = f;
        Matrix<ArbInterval> dM(n, m, 0);
        ArbInterval df = 1;
    
    
        for (int i = c; i <= n - 1; ++i) {
            ArbInterval t = xs.at(i, 0) * M.at(i, i);
            ArbInterval dt = df / M.at(i, i);
            dM.at(i, i) -= f * df / M.at(i, i);
            
            for (int j = n - 1; j >= i + 1; --j) {
                ArbInterval z = M.at(i, j) * xs.at(j, 0);
                t = t + z;
                
                dM.at(i, j) += t * dt;
                dt += M.at(i, j) * dt;
            }
            dM.at(i, n) += dt;
        }
    
    
        GaussInverse(M, dM);
        xs.at(c, 0) = ArbInterval(x.val(), ComputeError(init, dM));
    }
    
    return xs;
}

}  // namespace interval

#endif  // LEQS_H

