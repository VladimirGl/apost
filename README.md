# apost

Interval Arithmetic library with aposteriori error computation.

### Installation

Install dependencies [FLINT](http://www.flintlib.org/) C library with [Arb](http://fredrikj.net/arb/) module. See instructions [here](http://fredrikj.net/arb/setup.html#installation-as-part-of-flint). FLINT also depends on the MPIR/GMP and MPFR libraries.

You also should have C++11 compiler.

To build and install apost library use:
```sh
$ git clone https://github.com/VladimirGl/apost
$ cd apost
$ mkdir build
$ cd build
$ cmake ..
$ make
$ make install
```

### Examples

There are several examples in example file. After compilation them can be found in build/bin folder.

### Library usage example

Then you can build your own programs. Example program:

```c++
#include "apost.h"

#include <iostream>

using namespace interval;
using namespace apost;

using IntervalVar = ProxyInterval<ArbInterval>;
using IntervalResult = ProxyIntervalResult;

template<class IntervalT>
IntervalT det(Matrix<IntervalT> matrix) {
    size_t n = matrix.nrow();
 
    gauss_elimination(matrix);
    
    IntervalT d = IntervalT(1);
    for (size_t i = 0; i < n; ++i)
        d = d * matrix.at(i, i);

    return d;
}

int main() {
    Matrix<IntervalVar> A;
    ... set A values ...
    controller.init();
    
    IntervalResult d;
    d = det(A);

    std::cout << d << std::endl;
}
```
Build and run:
```sh
$ g++ main.cpp -std=c++11 -lapost -lflint
$ ./a.out
```

