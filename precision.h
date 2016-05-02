#ifndef PRECISION_H
#define PRECISION_H

// Now all calculations are made with PRECISION precision (in bits).

namespace interval {

static int PRECISION = 256;

inline void setPrecision(int precision) {
    PRECISION = precision;
}

inline int getPrecision() {
    return PRECISION;
}

}  // namespace interval


#endif  // PRECISION_H

