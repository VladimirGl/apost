# Copyright (c) 2016 The Caroline authors. All rights reserved.
# Use of this source file is governed by a MIT license that can be found in the
# LICENSE file.
# Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

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

