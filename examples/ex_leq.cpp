# Copyright (c) 2016 The Caroline authors. All rights reserved.
# Use of this source file is governed by a MIT license that can be found in the
# LICENSE file.
# Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "../apost.h"
#include "../leqs.h"

/*
    This file contains example of program of linear equation solving
    using different methods. It also shows the comparision of these methods.
*/

using namespace interval;
using namespace apost;

int main(int argc, char *argv[]) {
    int n = 3;
    Matrix<ArbInterval> m(n, n + 1);
    
    m.at(0, 0) = ArbInterval(2, 0.001);
    m.at(0, 1) = ArbInterval(1, 0.001);
    m.at(0, 2) = ArbInterval(1, 0.001);
    m.at(0, 3) = ArbInterval(8, 0.001);
    
    m.at(1, 0) = ArbInterval(3, 0.001);
    m.at(1, 1) = ArbInterval(1, 0.001);
    m.at(1, 2) = ArbInterval(2, 0.001);
    m.at(1, 3) = ArbInterval(-11, 0.001);
    
    m.at(2, 0) = ArbInterval(2, 0.001);
    m.at(2, 1) = ArbInterval(1, 0.001);
    m.at(2, 2) = ArbInterval(2, 0.001);
    m.at(2, 3) = ArbInterval(-3, 0.001);
    
    Matrix<ProxyInterval<ArbInterval>> m_apost(n, n + 1);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n + 1; ++j) {
            m_apost.at(i, j) = m.at(i, j);
        }
    }
    
    std::cout << "An example of solving a linear system\n";
    std::cout << "--------------------------------------------------\n";
    std::cout << "Input matrix [A|b]:\n";
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j)
            std::cout << m.at(i, j) << " ";
        std::cout << "| " << m.at(i, n) << std::endl;
    }
    std::cout << "Output value - x - solution of Ax = b system\n";
    std::cout << "--------------------------------------------------\n";
    
    controller.init();
    std::cout << "Solve using dinamic aposteriori:\n";
    std::cout << linear_solve_apost(m_apost);
    std::cout << "Solve using static aposteriori:\n";
    std::cout << leq_inv(m);
    std::cout << "Solve using traditional:\n";
    std::cout << linear_solve(m);
    
    return 0;
}

