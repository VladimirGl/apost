#ifndef VALUE_H
#define VALUE_H

#include "precision.h"

#include "flint/arf.h"

/*
    This file contains  C++ wrapper of Arb library arf_t type for arbitary
    precision real values representation.
    Now implemented only comparision operations and addition.
*/

namespace interval {

// TODO :
//  1) implement more arf functions;

// Struct to work with real values. It is used for interval value.
// It is also used then you need to work with errors (in internal interval
// implementation used mag_t type).
struct Value {
    Value() {
        arf_init(data_);
    }
    
    ~Value() {
        arf_clear(data_);
    }

    arf_t data_;
    
    // double cast.
    operator double() const {
        return arf_get_d(data_, ARF_RND_UP);
    }
    
    // Addition.
    Value operator+(const Value& other) {
        Value temp;
        arf_add(temp.data_, data_, other.data_, getPrecision(), ARF_RND_UP);
        
        return temp;
    }
    
    Value operator/(const Value& other) {
        Value temp;
        arf_div(temp.data_, data_, other.data_, getPrecision(), ARF_RND_UP);
        
        return temp;
    }
    
    void print() const {
        arf_print(data_);
    }
};

// Comparision functions.
inline bool operator==(const Value& x, const Value& y) {
    return arf_cmp(x.data_, y.data_) == 0;
}

inline bool operator!=(const Value& x, const Value& y) {
    return arf_cmp(x.data_, y.data_) != 0;
}

inline bool operator<(const Value& x, const Value& y) {
    return arf_cmp(x.data_, y.data_) < 0;
}

inline bool operator<=(const Value& x, const Value& y) {
    return arf_cmp(x.data_, y.data_) <= 0;
}

inline bool operator>(const Value& x, const Value& y) {
    return arf_cmp(x.data_, y.data_) > 0;
}

inline bool operator>=(const Value& x, const Value& y) {
    return arf_cmp(x.data_, y.data_) >= 0;
}

}  // namespace interval


#endif  // VALUE_H

