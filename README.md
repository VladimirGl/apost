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

int main() {
    IntervalVar x0(ArbInterval(1, 0.01));
    IntervalVar x1(ArbInterval(2, 0.03));
    controller.init();
    
    IntervalVar x2 = x0 * x0;
    IntervalVar x3 = x1 * x0;
    x3 = x3 * x0 + x2;
    IntervalVar x4 = x3 / x2;
    
    ProxyIntervalResult x5;
    x5 = x4;

    std::cout << x5 << std::endl;
}
```
Build and run:
```sh
$ g++ main.cpp -std=c++11 -lapost -lflint
$ ./a.out
```

