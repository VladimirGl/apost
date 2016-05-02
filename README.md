# apost

Intarval Arithmetic with aposteriori error computation library.

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

### Usage example

Then you can build your own programs. Example program:

```c++
#include "apost.h"

#include <iostream>

using namespace interval;
using namespace apost;

int main() {
    ProxyInterval<ArbInterval> x0(ArbInterval(1, 0.01));
    ProxyInterval<ArbInterval> x1(ArbInterval(2, 0.03));

    controller.init();
    ProxyInterval<ArbInterval> x2 = x0 * x0;
    ProxyInterval<ArbInterval> x3 = x1 * x0;
    
    ProxyIntervalResult x4;
    x4 = x3 / x2;

    std::cout << x4 << std::endl;
}
```
Build and run:
```sh
$ g++ main.cpp -std=c++11 -lapost -lflint
$ ./a.out
```

