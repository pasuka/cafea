/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#ifndef CAFEA_UTILS_HPP_
#define CAFEA_UTILS_HPP_

#include <complex>
#include <cmath>
#include <limits>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <utility>

#include <Eigen/Dense>

namespace cafea {
//! Using i stands for imag part of complex.
using namespace std::literals::complex_literals;
//! Using s stands for string.
using namespace std::literals::string_literals;
//! Using sv stands for string view.
using namespace std::literals::string_view_literals;
//! REAL(kind=4) in Fortran iso_c_binding.
using REAL4 = float;
//! REAL(kind=8) in Fortran iso_c_binding.
using REAL8 = double;
//! Complex template.
template <class T = REAL8>
using COMPLEX = std::complex<T>;
//! COMPLEX(kind=4) in Fortran iso_c_binding.
using COMPLEX4 = COMPLEX<REAL4>;
//! COMPLEX(kind=8) in Fortran iso_c_binding.
using COMPLEX8 = COMPLEX<REAL8>;
//! REAL(kind=16) in Fortran iso_c_binding.
#ifndef REAL_16_ON
#define REAL_16_ON 0
#endif
#if (0 < REAL_16_ON)
using REAL16 = long double;
using COMPLEX16 = COMPLEX<REAL16>;
#endif
//! Precision of machine via different types.
template<class T = REAL8>
constexpr T EPS() { return std::numeric_limits<T>::epsilon();}
//! M_PI.
template<class T = REAL8>
constexpr T PI() { return std::atan(std::forward<T>(1.0))*std::forward<T>(4.0);}

//! Initializer list.
template <class T>
using init_list_ = std::initializer_list<T>;
//! Dictionary.
template <class T>
using dict_ = std::unordered_map<int, T>;
//! Vector3.
template <class T = REAL8>
using vec3_ = Eigen::Matrix<T, 3, 1>;
//! Vector.
template <class T = REAL8>
using vecX_ = Eigen::Matrix<T, Eigen::Dynamic, 1>;
//! Matrix.
//! C++11's trick for template aliases.
template <class U = REAL8>
using matrix_ = Eigen::Matrix<U, Eigen::Dynamic, Eigen::Dynamic>;
//! Complex matrix.
template <class U = REAL8>
using cmatrix_ = matrix_<COMPLEX<U>>;
//! Matlab style varargout with 2 return.
//! Template aliases trick in C++11.
template <class T = REAL8>
using varargout_2_ = std::tuple<T, matrix_<T>>;
//! Matlab style varargout with 3 return.
template <class T = REAL8>
using varargout_3_ = std::tuple<T, matrix_<T>, matrix_<T>>;
//! Matlab style varargout with 4 return.
template <class T = REAL8>
using varargout_4_ = std::tuple<T, matrix_<T>, matrix_<T>, matrix_<T>>;
//! Matlab style varargout with 5 return.
template <class T = REAL8>
using varargout_5_ = std::tuple<T, matrix_<T>, matrix_<T>, matrix_<T>, matrix_<T>>;
//! Matlab style varargout with 6 return.
template <class T = REAL8>
using varargout_6_ = std::tuple<T, matrix_<T>, matrix_<T>, matrix_<T>, matrix_<T>, matrix_<T>>;
}  // namespace cafea
#endif  // CAFEA_UTILS_HPP_
