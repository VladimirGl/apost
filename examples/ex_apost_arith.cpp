# Copyright (c) 2016 The Caroline authors. All rights reserved.
# Use of this source file is governed by a MIT license that can be found in the
# LICENSE file.
# Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "../apost.h"

#include <iostream>

/*
    This file contains simple example of program
    for which the error of output value computes
    using aposteriori method.
*/

using namespace interval;
using namespace apost;

int main() {
    ProxyInterval<ArbInterval> x0(ArbInterval(1, 0.01));
    ProxyInterval<ArbInterval> x1(ArbInterval(2, 0.03));

    apost::controller.init();

//    apost::debug = true;      // Uncomment this to see debug information.   
    
    std::cout << "Example of simple computation error improvement\n";
    std::cout << "--------------------------------------------------\n";
    std::cout << "x0 = " << x0 << std::endl;
    std::cout << "x1 = " << x1 << std::endl;
    std::cout << "--------------------------------------------------\n";
    
    std::cout << "We compute:\n"
                 "x2 = x0 * x0\n"
                 "x3 = x1 * x0\n"
                 "x4 = x3 / x2\n";
    std::cout << "Output value - x4\n";
    
    ProxyInterval<ArbInterval> x2 = x0 * x0;
    ProxyInterval<ArbInterval> x3 = x1 * x0;
    ProxyInterval<ArbInterval> x4 = x3 / x2;
    
    ArbInterval y = controller.evaluate();
    
    std::cout << "--------------------------------------------------\n";
    std::cout << "True value :        " << x1 / x0 << std::endl;
    std::cout << "Traditional value : " << x4 << std::endl;
    std::cout << "Aposteriori value : " << y << std::endl;
    
    return 0;
}

