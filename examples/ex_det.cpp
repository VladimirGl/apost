#include "../apost.h"
#include "../dets.h"

#include <iostream>

/*
    This file contains example of program of determenant computation
    using different methods. It also shows the comparision of these methods.
*/

using namespace interval;
using namespace apost;

int main() {
    int n = 3;
    Matrix<ArbInterval> m(n, n);
    
    m.at(0, 0) = ArbInterval(4, 0.01);
    m.at(0, 1) = ArbInterval(7, 0.01);
    m.at(0, 2) = ArbInterval(8, 0.01);
    
    m.at(1, 0) = ArbInterval(6, 0.01);
    m.at(1, 1) = ArbInterval(4, 0.01);
    m.at(1, 2) = ArbInterval(6, 0.01);
    
    m.at(2, 0) = ArbInterval(7, 0.01);
    m.at(2, 1) = ArbInterval(3, 0.01);
    m.at(2, 2) = ArbInterval(10, 0.01);
    
    Matrix<ProxyInterval<ArbInterval>> m_apost(n, n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            m_apost.at(i, j) = m.at(i, j);
        }
    }
    
    std::cout << "Example of determenant computations\n";
    std::cout << "--------------------------------------------------\n";
    std::cout << "Input matrix A:\n";
    std::cout << m << std::endl;
    std::cout << "Output value - det(A)\n";
    std::cout << "--------------------------------------------------\n";
    
    ArbInterval d1, d2, d3, dp1, dp2, dp3;
    
    controller.init();
    ProxyIntervalResult proxy_d1, proxy_dp1;
    proxy_d1 = det(m_apost);
    proxy_dp1 = det_pivot(m_apost);
    
    d1 = proxy_d1;
    dp1 = proxy_dp1;
    
    d2 = det_inv(m);
    d3 = det(m);
    
    dp2 = det_inv_pivot(m);
    dp3 = det_pivot(m);
    
    std::cout << "Compute without pivoting:\n";
    std::cout << "Traditional method :          " << d3 << std::endl;
    std::cout << "Aposteriori statical method : " << d2 << std::endl;
    std::cout << "Aposteriori dinamic  method : " << d1 << std::endl;
    std::cout << "--------------------------------------------------\n";
    
    std::cout << "Compute with pivoting:\n";
    std::cout << "Traditional method :          " << dp3 << std::endl;
    std::cout << "Aposteriori statical method : " << dp2 << std::endl;
    std::cout << "Aposteriori dinamic  method : " << dp1 << std::endl;
    std::cout << "--------------------------------------------------\n";
    
    return 0;
}

