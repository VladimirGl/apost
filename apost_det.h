#ifndef DET_H
#define DET_H

#include <algorithm>

#include "matrix.h"

#include "apost.h"

#include "apost_statical.h"

#include "gauss.h"

namespace interval {



//--------------------------------DETERMENANTS----------------------------------



//--------------------------------LINEAREQUATS----------------------------------






/*-----------------------------------------------------------------------------*/




namespace apost {
Matrix<ProxyIntervalResult> detsGauss(Matrix<ProxyInterval<ArbInterval>> matrix) {
    size_t n = matrix.nrow();
    
    Matrix<ProxyInterval<ArbInterval>> L(n, n);
    std::vector<ProxyInterval<ArbInterval>> dets(n);
    
    std::vector<ProxyIntervalResult> finaldets(n);
    dets[0] = matrix.at(0, 0);
// TODO
//    finaldets[0].data_ = dets[0];
//    finaldets[0].result_ = ((ArbInterval)dets[0]);
    for (size_t i = 0; i < n; ++i) {
        if (i != 0) {
            dets[i] = matrix.at(i, i) * dets[i - 1];
            finaldets[i] = matrix.at(i, i) * dets[i - 1];
        }
            
        for (size_t j = i + 1; j < n; ++j) {
            ProxyInterval<ArbInterval> z = matrix.at(j, i) / matrix.at(i, i);
            z = -z;
            
            matrix.at(j, i) = z;
            L.at(j, i) = z;
            
            for (size_t k = i + 1; k < n; ++k) {
                ProxyInterval<ArbInterval> t = matrix.at(i, k) * z;
                matrix.at(j, k) = matrix.at(j, k) + t;
                L.at(j, k) = matrix.at(j, k);
            }
        }
    }
    
    for (size_t k = 0; k < n; ++k)
		for (size_t i = k + 1; i < n; ++i)
			for (size_t j = i + 1; j < n; ++j) {
			    ProxyInterval<ArbInterval> t = L.at(i, k) * matrix.at(j, i);
			    L.at(j, k) = L.at(j, k) + t;
			}
			
    Matrix<ProxyIntervalResult> L2(n, n);

// TODO
/*    
    L2.at(0, 0).data_ = ArbInterval(1);
    L2.at(0, 0).result_ = L2.at(0, 0).data_;
	for (size_t j = 0; j < n; ++j) {
		for (size_t k = 0; k < j; ++k) {
			if (j == 0) {
				L2.at(j, k) = L.at(j, k);
			} else {
				L2.at(j, k) = L.at(j, k) * dets[j - 1];
			}
		}

		if (j != 0) {
			L2.at(j, j) = finaldets[j - 1];
		}
	}
*/
	
	return L2;
}

}  // namespace apost

}  // namespace interval

#endif  // DET_H
