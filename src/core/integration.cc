/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#include <string>
#include <iostream>
#include "cafea/utils_ext.h"

namespace cafea::utility {
	template std::pair<vecX_<>, vecX_<>> gauss_quad<1, REAL8>();
    template std::pair<vecX_<>, vecX_<>> gauss_quad<2, REAL8>();
    template std::pair<vecX_<>, vecX_<>> gauss_quad<3, REAL8>();
    template std::pair<vecX_<>, vecX_<>> gauss_quad<4, REAL8>();
	template std::pair<vecX_<>, vecX_<>> gauss_quad<5, REAL8>();
	// template std::pair<vecX_<>, vecX_<>> gauss_quad<6, REAL8>();
    // template std::pair<vecX_<>, vecX_<>> gauss_quad<7, REAL8>();
    // template std::pair<vecX_<>, vecX_<>> gauss_quad<8, REAL8>();
    // template std::pair<vecX_<>, vecX_<>> gauss_quad<9, REAL8>();
	// template std::pair<vecX_<>, vecX_<>> gauss_quad<10, REAL8>();
	// template std::pair<vecX_<>, vecX_<>> gauss_quad<11, REAL8>();
    // template std::pair<vecX_<>, vecX_<>> gauss_quad<12, REAL8>();
    // template std::pair<vecX_<>, vecX_<>> gauss_quad<13, REAL8>();
    // template std::pair<vecX_<>, vecX_<>> gauss_quad<14, REAL8>();
	// template std::pair<vecX_<>, vecX_<>> gauss_quad<15, REAL8>();
	// template std::pair<vecX_<>, vecX_<>> gauss_quad<16, REAL8>();
    // template std::pair<vecX_<>, vecX_<>> gauss_quad<17, REAL8>();
    // template std::pair<vecX_<>, vecX_<>> gauss_quad<18, REAL8>();
    // template std::pair<vecX_<>, vecX_<>> gauss_quad<19, REAL8>();
	// template std::pair<vecX_<>, vecX_<>> gauss_quad<20, REAL8>();
	// template std::pair<vecX_<>, vecX_<>> gauss_quad<21, REAL8>();
    // template std::pair<vecX_<>, vecX_<>> gauss_quad<22, REAL8>();
    // template std::pair<vecX_<>, vecX_<>> gauss_quad<23, REAL8>();
    // template std::pair<vecX_<>, vecX_<>> gauss_quad<24, REAL8>();
	// template std::pair<vecX_<>, vecX_<>> gauss_quad<25, REAL8>();
	// template std::pair<vecX_<>, vecX_<>> gauss_quad<26, REAL8>();
	// template std::pair<vecX_<>, vecX_<>> gauss_quad<27, REAL8>();
    // template std::pair<vecX_<>, vecX_<>> gauss_quad<28, REAL8>();
    // template std::pair<vecX_<>, vecX_<>> gauss_quad<29, REAL8>();
    // template std::pair<vecX_<>, vecX_<>> gauss_quad<30, REAL8>();
	// template std::pair<vecX_<>, vecX_<>> gauss_quad<31, REAL8>();
    // template std::pair<vecX_<>, vecX_<>> gauss_quad<32, REAL8>();
    // template std::pair<vecX_<>, vecX_<>> gauss_quad<33, REAL8>();
    // template std::pair<vecX_<>, vecX_<>> gauss_quad<34, REAL8>();
	// template std::pair<vecX_<>, vecX_<>> gauss_quad<35, REAL8>();
	// template std::pair<vecX_<>, vecX_<>> gauss_quad<36, REAL8>();
	// template std::pair<vecX_<>, vecX_<>> gauss_quad<37, REAL8>();
    // template std::pair<vecX_<>, vecX_<>> gauss_quad<38, REAL8>();
    // template std::pair<vecX_<>, vecX_<>> gauss_quad<39, REAL8>();
	template <size_t nPoints, class T>
	constexpr std::pair<vecX_<T>, vecX_<T>> gauss_quad() {
		assert(0 < nPoints);
		vecX_<T> pt = vecX_<T>::Zero(nPoints), wt = pt;
		if constexpr (1 == nPoints) {
			pt << T(0.0);
			wt << T(2.0);
		} else {
			matrix_<T> ma = matrix_<T>::Zero(nPoints, nPoints);
			vecX_<T> subDiag = vecX_<T>::Ones(nPoints-1);
			subDiag /= std::sqrt(4.0 - 1.0/T(nPoints-1)/T(nPoints-1));
			ma.diagonal(-1) = subDiag;
			std::cout << ma << "\n";
			Eigen::SelfAdjointEigenSolver<matrix_<T>> eigenSolver(ma);
			assert(Eigen::ComputationInfo::Success == eigenSolver.info());
			pt = eigenSolver.eigenvalues();
			wt = T(2.0)*eigenSolver.eigenvectors().col(0).cwiseAbs2();
		}
		return std::make_pair(pt, wt);
	}
}
// #include "cafea/cafea.h"

// using Eigen::VectorXd;

// namespace cafea {
// /**
//  *  \brief Gauss-Legendre integrate point and weight.
//  *  \param [in] n number of points.
//  *  \return integrate point and weight values.
//  */
// std::tuple<VectorXd, VectorXd> gauss_quad(int n) {
// 	assert(0 < n && n < 11);
// 	VectorXd pt = VectorXd::Zero(n);//!< quadrature point
// 	VectorXd wt = pt;//!< quadrature weight
// 	switch (n) {
// 		case 1: pt(0) = .0; wt(0) = 2.; break;
// 		case 2: pt(0) = -.577350269189626E0; wt(0) = 1.0E0;
// 				pt(1) =  .577350269189626E0; wt(1) = 1.0E0; break;
// 		case 3: pt(0) = -.774596669241483E0; wt(0) = .555555555555555E0;
// 				pt(1) =  .0E0;               wt(1) = .888888888888889E0;
// 				pt(2) =  .774596669241483E0; wt(2) = .555555555555555E0; break;
// 		case 4: pt(0) = -.861136311594053E0; wt(0) = .347854845137454E0;
// 				pt(1) = -.339981043584856E0; wt(1) = .652145154862546E0;
// 				pt(2) =  .339981043584856E0; wt(2) = .652145154862546E0;
// 				pt(3) =  .861136311594053E0; wt(3) = .347854845137454E0; break;
// 		case 5: pt(0) = -.906179845938664E0; wt(0) = .236926885056189E0;
// 				pt(1) = -.538469310105683E0; wt(1) = .478628670499367E0;
// 				pt(2) =  .000000000000000E0; wt(2) = .568888888888889E0;
// 				pt(3) =  .538469310105683E0; wt(3) = .478628670499367E0;
// 				pt(4) =  .906179845938664E0; wt(4) = .236926885056189E0; break;
// 		case 6: pt(0) = -.932469514203152E0; wt(0) = .171324492379169E0;
// 				pt(1) = -.661209386466264E0; wt(1) = .360761573048139E0;
// 				pt(2) = -.238619186083197E0; wt(2) = .467913934572692E0;
// 				pt(3) =  .238619186083197E0; wt(3) = .467913934572692E0;
// 				pt(4) =  .661209386466264E0; wt(4) = .360761573048139E0;
// 				pt(5) =  .932469514203152E0; wt(5) = .171324492379169E0; break;
// 		case 7: pt(0) = -.949107912342758E0; wt(0) = .129484966168870E0;
// 				pt(1) = -.741531185599394E0; wt(1) = .279705391489277E0;
// 				pt(2) = -.405845151377397E0; wt(2) = .381830050505119E0;
// 				pt(3) =  .000000000000000E0; wt(3) = .417959183673470E0;
// 				pt(4) =  .405845151377397E0; wt(4) = .381830050505119E0;
// 				pt(5) =  .741531185599394E0; wt(5) = .279705391489277E0;
// 				pt(6) =  .949107912342758E0; wt(6) = .129484966168870E0; break;
// 		case 8: pt(0) = -.960289856497536E0; wt(0) = .101228536290374E0;
// 				pt(1) = -.796666477413627E0; wt(1) = .222381034453375E0;
// 				pt(2) = -.525532409916329E0; wt(2) = .313706645877888E0;
// 				pt(3) = -.183434642495650E0; wt(3) = .362683783378363E0;
// 				pt(4) =  .183434642495650E0; wt(4) = .362683783378363E0;
// 				pt(5) =  .525532409916329E0; wt(5) = .313706645877888E0;
// 				pt(6) =  .796666477413627E0; wt(6) = .222381034453375E0;
// 				pt(7) =  .960289856497536E0; wt(7) = .101228536290374E0; break;
// 		case 9:	pt(0) = -.968160239507626E0; wt(0) = .081274388361575E0;
// 				pt(1) = -.836031107326636E0; wt(1) = .180648160694857E0;
// 				pt(2) = -.613371432700590E0; wt(2) = .260610696402935E0;
// 				pt(3) = -.324253423403809E0; wt(3) = .312347077040003E0;
// 				pt(4) =  .000000000000000E0; wt(4) = .330239355001259E0;
// 				pt(5) =  .324253423403809E0; wt(5) = .312347077040003E0;
// 				pt(6) =  .613371432700590E0; wt(6) = .260610696402935E0;
// 				pt(7) =  .836031107326636E0; wt(7) = .180648160694857E0;
// 				pt(8) =  .968160239507626E0; wt(8) = .081274388361575E0; break;
// 		case 10:pt(0) = -.973906528517172E0; wt(0) = .066671344308688E0;
// 				pt(1) = -.865063366688985E0; wt(1) = .149451349150581E0;
// 				pt(2) = -.679409568299024E0; wt(2) = .219086362515982E0;
// 				pt(3) = -.433395394129247E0; wt(3) = .269266719309996E0;
// 				pt(4) = -.148874338981631E0; wt(4) = .295524224714753E0;
// 				pt(5) =  .148874338981631E0; wt(5) = .295524224714753E0;
// 				pt(6) =  .433395394129247E0; wt(6) = .269266719309996E0;
// 				pt(7) =  .679409568299024E0; wt(7) = .219086362515982E0;
// 				pt(8) =  .865063366688985E0; wt(8) = .149451349150581E0;
// 				pt(9) =  .973906528517172E0; wt(9) = .066671344308688E0; break;
// 		default: fmt::print("Input: {} exceede!\n", n);
// 	}
// 	return std::make_tuple(pt, wt);
// }
// }  // namespace cafea
