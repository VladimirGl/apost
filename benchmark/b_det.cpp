# Copyright (c) 2016 The Caroline authors. All rights reserved.
# Use of this source file is governed by a MIT license that can be found in the
# LICENSE file.
# Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "../apost.h"
#include "../dets.h"
#include "../random_matrix.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <fstream>
#include <string>

using namespace interval;
using namespace apost;

int main() {
    const size_t n = 5;
    const size_t min_err = n + 5;
    const size_t max_err = 200;
    const size_t err_step = 5;
    const size_t n_matrices = 10000;
    std::string fname = "dets.csv";
    
    std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<double> distribution(-5, 5);
    auto random = [&] () { return distribution(generator); };
    
    std::vector<double> v1(n_matrices);
    std::vector<double> v2(n_matrices);
    std::vector<double> v3(n_matrices);
    std::vector<double> v4(n_matrices);
    std::vector<double> v5(n_matrices);
    
    std::ofstream fout(fname);
    for (size_t prec = min_err; prec <= max_err; prec += err_step) {
        std::cout << prec << std::endl;
        for (size_t c = 0; c < n_matrices; ++c) {
            if (c % 1000 == 0) std::cout << c << std::endl;
            Matrix<ArbInterval> m = random_matrix(n, prec, random);
            
            controller = Controller<ArbInterval>();
            Matrix<ProxyInterval<ArbInterval>> m_apost(n, n);
            for (size_t i = 0; i < n; ++i)
                for (size_t j = 0; j < n; ++j)
                    m_apost.at(i, j) = m.at(i, j);
            
            controller.init();
            ProxyIntervalResult proxy_d1, proxy_dp1;
            proxy_d1 = det(m_apost);
            proxy_dp1 = det_pivot(m_apost);
            
            Value d1 = static_cast<ArbInterval>(proxy_d1).error();
            Value dp1 = static_cast<ArbInterval>(proxy_dp1).error();
            
            Value d2 = det_inv(m).error();
            Value d3 = det(m).error();
            
            Value dp2 = det_inv_pivot(m).error();
            Value dp3 = det_pivot(m).error();
            
            v1[c] = (d3 / d1);
            v2[c] = (d3 / d2);
            v3[c] = (d3 / dp1);
            v4[c] = (d3 / dp2);
            v5[c] = (d3 / dp3);
        }
        
        auto median = [n_matrices] (std::vector<double> v) {
            std::sort(v.begin(), v.end());
            return (v[n_matrices / 2 - 1] + v[n_matrices / 2]) / 2;
        };
        
        fout << prec << " " << median(v1) << " " << median(v2) << " " <<
            median(v3) << " " << median(v4) << " " << median(v5) << "\n";
    }
    
    return 0;
}

