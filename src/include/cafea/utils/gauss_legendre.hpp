#ifndef CAFEA_GAUSS_LEGENDRE_HPP_
#define CAFEA_GAUSS_LEGENDRE_HPP_

#include "cafea/utils/utils.hpp"

namespace cafea {
inline namespace utility {
	/**
	 * \brief Gauss-Legendre integrate point and weight.
 	 * \tparam nPoints number of points
 	 * \tparam T       numerical type, i.e. float, double, long double, et al.
	 * \return integrate point and weight value
 	*/
	template <size_t nPoints = 2, class T = REAL8>
	constexpr std::pair<vecX_<T>, vecX_<T>> gauss_quad() {
		assert(0 < nPoints);
		vecX_<T> pt = vecX_<T>::Zero(nPoints), wt = pt;
		if constexpr (1 == nPoints) {
			pt << T(0.0);
			wt << T(2.0);
		} else {
			matrix_<T> ma = matrix_<T>::Zero(nPoints, nPoints);
			vecX_<T> subDiag = vecX_<T>::LinSpaced(nPoints-1, 1, nPoints-1);
			// Only low triangular part of matrix is required.
			ma.diagonal(-1) = subDiag.unaryExpr([](T x) { return x/std::sqrt(T(4.0)*x*x-T(1.0));});
			Eigen::SelfAdjointEigenSolver<matrix_<T>> eigenSolver(ma);
			assert(Eigen::ComputationInfo::Success == eigenSolver.info());
			pt = eigenSolver.eigenvalues();
			wt = T(2.0)*eigenSolver.eigenvectors().row(0).cwiseAbs2();
		}
		return std::make_pair(pt, wt);
	}
}
}
#endif // CAFEA_GAUSS_LEGENDRE_HPP_
