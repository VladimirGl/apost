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
    setPrecision(1024);

    const size_t min_n = 3;
    const size_t max_n = 50;
    
    const size_t prec = 100;
    const size_t n_iters = 100;
    
    std::string fname = "dets_time.csv";
    
    std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<double> distribution(-5, 5);
    auto random = [&] () { return distribution(generator); };
    
    std::ofstream fout(fname);
    for (size_t n = min_n; n <= max_n; ++n) {
        std::cout << "dim = " << n << "\n";
        std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
        
        Matrix<ArbInterval> m = random_matrix(n, prec, random);
        
        start = std::chrono::high_resolution_clock::now();
        for (size_t counter = 0; counter < n_iters; ++counter) {
            controller = Controller<ArbInterval>();
            Matrix<ProxyInterval<ArbInterval>> m_apost(n, n);
            for (size_t i = 0; i < n; ++i)
                for (size_t j = 0; j < n; ++j)
                    m_apost.at(i, j) = m.at(i, j);
                    
            controller.init();
            ProxyIntervalResult result;
            result = det_pivot(m_apost);
        }
        end = std::chrono::high_resolution_clock::now();
        
        fout << n << "," << std::chrono::duration_cast<
            std::chrono::milliseconds>(end - start).count() << ",din,"
            << n_iters << "\n";
        
        start = std::chrono::high_resolution_clock::now();
        for (size_t counter = 0; counter < n_iters; ++counter) {
            ArbInterval result = det_pivot(m);
        }
        end = std::chrono::high_resolution_clock::now();
        
        fout << n << "," << std::chrono::duration_cast<
            std::chrono::milliseconds>(end - start).count() << ",trad,"
            << n_iters << "\n";
            
        start = std::chrono::high_resolution_clock::now();
        for (size_t counter = 0; counter < n_iters; ++counter) {
            ArbInterval result = det_inv_pivot(m);
        }
        end = std::chrono::high_resolution_clock::now();
        
        fout << n << "," << std::chrono::duration_cast<
            std::chrono::milliseconds>(end - start).count() << ",stat,"
            << n_iters << "\n";
    }
    
    return 0;
}

