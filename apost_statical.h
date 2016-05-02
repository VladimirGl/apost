#ifndef APOST_STATICAL_H
#define APOST_STATICAL_H

#include "interval.h"
#include "matrix.h"

namespace interval {

// Computes the final error.
Value ComputeError(const Matrix<ArbInterval>& A,
        const Matrix<ArbInterval>& dA) {
    size_t n = A.nrow();
    size_t m = A.ncol();
        
    ArbInterval result = 0;
    
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            ArbInterval dval = dA.at(i, j);
            dval.abs();
            
            ArbInterval err(A.at(i, j).error());
            result += dval * err;
        }
    }
    
    Value error = result.val() + result.error();
    
    return error;
}

void GaussInverse(Matrix<ArbInterval> A, Matrix<ArbInterval>& dA) {
    size_t n = A.nrow();
    size_t m = A.ncol();

    for (int i = n - 2; i >= 0; --i) {
        for (size_t j = n - 1; j >= i + 1; --j) {
            ArbInterval dot = 0;
            
            for (size_t k = i + 1; k < m; ++k)
                dot += dA.at(j, k) * A.at(i, k); 
            dA.at(j, i) -= dot;
           
            
            for (size_t k = i + 1; k < m; ++k) {
                dA.at(i, k) -= dA.at(j, k) * A.at(j, i);
            }
            
            for (size_t k = i + 1; k < m; ++k) {
                A.at(j, k) += A.at(j, i) * A.at(i, k);
            }
            
            dA.at(i, i) -= dA.at(j, i) * A.at(j, i) / A.at(i, i);
            dA.at(j, i) /= A.at(i, i);
            
            A.at(j, i) *= A.at(i, i);
        }
    }
}


}  // namespace interval

#endif  // APOST_STATICAL
