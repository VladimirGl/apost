# Copyright (c) 2016 The Caroline authors. All rights reserved.
# Use of this source file is governed by a MIT license that can be found in the
# LICENSE file.
# Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef INTERVAL_H
#define INTERVAL_H

#include "precision.h"
#include "value.h"

#include "flint/arb.h"

#include <iostream>

/*
    This file contains  C++ wrapper of Arb library arb_t type for arbitary
    precision real interval representation.
    Now implemented only arithmetical and comparision operations.
*/

namespace interval {

// Class to work with interval values.
class ArbInterval {
public:

    // Constructors.
    ArbInterval() {
        arb_init(data_);
        arb_zero(data_);
    }
    
    ArbInterval(double value, double rad)
    : ArbInterval() {
        arb_set_d(data_, value);
        mag_set_d(arb_radref(data_), rad);
    }
    
    ArbInterval(double value)
    : ArbInterval() {
        arb_set_d(data_, value);
    }

    ArbInterval(const ArbInterval& other) 
    : ArbInterval() {
        arb_set(data_, other.data_);
    }
    
    ArbInterval(const Value& value)
    : ArbInterval() {
        arb_set_arf(data_, value.data_);
    }
    
    ArbInterval(const Value& value, const Value& rad)
    : ArbInterval() {
        arb_set_arf(data_, value.data_);
        arf_get_mag(arb_radref(data_), rad.data_);
    }
    
    // Destructor.
    ~ArbInterval() {
        arb_clear(data_);
    }
    
    // Swap and assignment.
    void swap(ArbInterval& other) {
        arb_swap(data_, other.data_);
    }
    
    ArbInterval& operator=(const ArbInterval& other) {
        ArbInterval temp(other);
        swap(temp);
        
        return *this;
    }
    
    // Arithmetical operations.
    ArbInterval& operator+=(const ArbInterval& x) {
        arb_add(data_, data_, x.data_, getPrecision());
        return *this;
    }
    
    ArbInterval& operator-=(const ArbInterval& x) {
        arb_sub(data_, data_, x.data_, getPrecision());
        return *this;
    }
    
    ArbInterval& operator*=(const ArbInterval& x) {
        arb_mul(data_, data_, x.data_, getPrecision());
        return *this;
    }
    ArbInterval& operator*=(int x) {
        slong s = x;
        arb_mul_si(data_, data_, x, getPrecision());
    
    }
    
    ArbInterval& operator/=(const ArbInterval& x) {
        arb_div(data_, data_, x.data_, getPrecision());
        return *this;
    }
    
    // Sets the interval to its negation.
    void neg() {
        arb_neg(data_, data_);
    }
    
    // Returns value value.
    Value val() const {
        Value temp;
        arf_set(temp.data_, arb_midref(data_));
        
        return temp;
    }
    
    // Returns error value.
    Value error() const {
        Value temp;
        arf_set_mag(temp.data_, arb_radref(data_));
        
        return temp;
    }
    
    // Returns right bound of absolute value of the interval.
    Value abs_ubound() const {
        Value temp;
        arb_get_abs_ubound_arf(temp.data_, data_, getPrecision());
        
        return temp;
    }
    
    // Returns left bound of absolute value of the interval.
    Value abs_lbound() const {
        Value temp;
        arb_get_abs_lbound_arf(temp.data_, data_, getPrecision());
        
        return temp;
    }
    
    // Sets the interval to zero.
    void zero() {
        arb_zero(data_);
    }
    
    // Sets the interval to its absolute value.
    void abs() {
        arb_abs(data_, data_);
    }
    
    // Returns data_ value. Need to refactor this.
    arb_t& data() { return data_; }
    
    // Comparision functions.
    bool eq(const ArbInterval& x) const { return (arb_eq(data_, x.data_)); }
    bool ne(const ArbInterval& x) const { return (arb_ne(data_, x.data_)); }
    bool lt(const ArbInterval& x) const { return (arb_lt(data_, x.data_)); }
    bool le(const ArbInterval& x) const { return (arb_le(data_, x.data_)); }
    bool gt(const ArbInterval& x) const { return (arb_gt(data_, x.data_)); }
    bool ge(const ArbInterval& x) const { return (arb_ge(data_, x.data_)); }
    
    // Returns true if contains zero; otherwise returns false.
    bool contains_zero() const { return arb_contains_zero(data_); }
    
    // Output functions.
    friend std::ostream& operator<<(std::ostream& os, const ArbInterval& x);
    void print() {
        arb_print(data_);
    }
        
private:
    arb_t data_;
};

// Comparision functions.
inline bool operator==(const ArbInterval& x, const ArbInterval& y) {
    return x.eq(y);
}

inline bool operator!=(const ArbInterval& x, const ArbInterval& y) {
    return x.ne(y);
}

inline bool operator<(const ArbInterval& x, const ArbInterval& y) {
    return x.lt(y);
}

inline bool operator<=(const ArbInterval& x, const ArbInterval& y) {
    return x.le(y);
}

inline bool operator>(const ArbInterval& x, const ArbInterval& y) {
    return x.gt(y);
}

inline bool operator>=(const ArbInterval& x, const ArbInterval& y) {
    return x.ge(y);
}

// Arithmetic operations.
inline ArbInterval operator+(ArbInterval x, const ArbInterval& y) {
    x += y;
    return x;
}

inline ArbInterval operator-(ArbInterval x, const ArbInterval& y) {
    x -= y;
    return x;
}

inline ArbInterval operator*(ArbInterval x, const ArbInterval& y) {
    x *= y;
    return x;
}

inline ArbInterval operator/(ArbInterval x, const ArbInterval& y) {
    x /= y;
    return x;
}

inline ArbInterval operator-(ArbInterval x) {
    x.neg();
    return x;
}

// To stream.
std::ostream& operator<<(std::ostream& os, const ArbInterval& x) {
    os << arb_get_str(x.data_, 10, ARB_STR_MORE);
    return os;
}

// Outer swap function.
void swap(ArbInterval& x, ArbInterval& y) {
    x.swap(y);
}

}  // interval


#endif  // INTERVAL_H

