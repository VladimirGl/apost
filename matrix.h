#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>

/*
    This file contains class to work with matrices.
    This is not implemented any matrix operations. Only element access 
    and print functions.
*/

namespace interval {

// Matrix class.
template<class IntervalT>
class Matrix {
public:
    // Constructed nrow*ncol matrix. Sets all elements to 0.
    Matrix(int nrow, int ncol)
    : nrow_(nrow)
    , ncol_(ncol)
    , data_(nrow * ncol) {
    }
    
    // Constructed nrow*ncol matrix. Sets all elements to value.
    Matrix(int nrow, int ncol, IntervalT value)
    : nrow_(nrow)
    , ncol_(ncol)
    , data_(nrow * ncol, value) {
    }
    
    // Copy constructor.
    Matrix(const Matrix& other) {
        nrow_ = other.nrow_;
        ncol_ = other.ncol_;
        data_ = other.data_;
    }
    
    Matrix& operator=(const Matrix& other) {
        nrow_ = other.nrow_;
        ncol_ = other.ncol_;
        data_ = other.data_;
        
        return *this;
    }
    
    IntervalT& at(int r, int c) {
        return data_[r * ncol_ + c];
    }
    
    IntervalT at(int r, int c) const {
        return data_[r * ncol_ + c];
    }
    
    int nrow() const { return nrow_; }
    int ncol() const { return ncol_; }

private:
    int nrow_;
    int ncol_;
    
    std::vector<IntervalT> data_;
};

template<typename IntervalT>
std::ostream& operator<<(std::ostream& os, const Matrix<IntervalT>& x) {
    for (size_t i = 0; i < x.nrow(); ++i) {
        for (size_t j = 0; j < x.ncol() - 1; ++j) {
            os << x.at(i, j) << " ";
        }
        os << x.at(i, x.ncol() - 1) << std::endl;
    }
    return os;
}

}  // namespace interval

#endif  // MATRIX_H

