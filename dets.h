# Copyright (c) 2016 The Caroline authors. All rights reserved.
# Use of this source file is governed by a MIT license that can be found in the
# LICENSE file.
# Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef DETS_H
#define DETS_H

#include "apost_statical.h"
#include "gauss.h"
#include "interval.h"
#include "matrix.h"

namespace interval {

// Computes the determenant of matrix using Gauss
// elimination (without pivoting).
template<class IntervalT>
IntervalT det(Matrix<IntervalT> matrix) {
    size_t n = matrix.nrow();
 
    gauss_elimination(matrix);
    
    IntervalT d = IntervalT(1);
    for (size_t i = 0; i < n; ++i)
        d = d * matrix.at(i, i);

    return d;
}

// Computes the determenant of matrix using Gauss
// elimination (with pivoting).
template<class IntervalT>
IntervalT det_pivot(Matrix<IntervalT> matrix) {
    size_t n = matrix.nrow();
    
    int sign = gauss_elimination_pivot(matrix);

    IntervalT d = IntervalT(1);
    for (size_t i = 0; i < n; ++i) {
        d = d * matrix.at(i, i);
    }
    d = IntervalT(sign) * d;
    
    return d;
}

// Computes the determenant of matrix using Gaussian
// elimination (without pivoting) and statical implementation
// of aposteriori method.
ArbInterval det_inv(Matrix<ArbInterval> M) {
    Matrix<ArbInterval> init = M;
    size_t n = M.nrow();
    
    if (n == 0) {
        return ArbInterval(0);
    }
    
    if (n == 1) {
        return M.at(0, 0);
    }
    
    gauss_elimination(M);

    ArbInterval f = 1;
    for (size_t i = 0; i < n; ++i) {
        f = f * M.at(i, i);
    }
    ArbInterval det = f;
    
    Matrix<ArbInterval> dM(n, n, 0);
    ArbInterval df = 1;
    
    // inverse step
    for (int i = n - 1; i >= 0; --i) {
        f /= M.at(i, i);
        dM.at(i, i) += df * f;
        df *= M.at(i, i);
    }
    
    GaussInverse(M, dM);
    det = ArbInterval(det.val(), ComputeError(init, dM));
    
    return det;
}

// Computes the determenant of matrix using Gaussian
// elimination (with pivoting) and statical implementation
// of aposteriori method.
ArbInterval det_inv_pivot(Matrix<ArbInterval> M) {
    Matrix<ArbInterval> init = M;
    Matrix<ArbInterval> i2 = M;

    size_t n = M.nrow();
    if (n == 0) {
        return ArbInterval(0);
    }
    
    if (n == 1) {
        return M.at(0, 0);
    }
    
    int sign = gauss_elimination_pivot(M);

    ArbInterval f = 1;
    for (size_t i = 0; i < n; ++i) {
        f = f * M.at(i, i);
    }
    ArbInterval det = ArbInterval(sign) * f;
    
    Matrix<ArbInterval> dM(n, n, 0);
    ArbInterval df = sign;
    
    // inverse step
    for (int i = n - 1; i >= 0; --i) {
        f /= M.at(i, i);
        dM.at(i, i) += df * f;
        df *= M.at(i, i);
    }

    GaussInverse(M, dM);
    det = ArbInterval(det.val(), ComputeError(init, dM));
    
    return det;
}

}  // namespace interval

#endif  // DETS_H

