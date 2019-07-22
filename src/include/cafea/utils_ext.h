/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#ifndef CAFEA_UTILS_EXT_H_
#define CAFEA_UTILS_EXT_H_

#include <tuple>

#include <Eigen/Dense>

#include "cafea/utils.h"

namespace cafea {
//! Vector3.
template <class T = REAL8>
using vec3_ = Eigen::Matrix<T, 3, 1>;
//! Vector.
template <class T = REAL8>
using vecX_ = Eigen::Matrix<T, Eigen::Dynamic, 1>;
template <class T = REAL8, size_t N=1>
using vecN_ = Eigen::Matrix<T, N, 1>;
//! Matirx.
//! C++11's trick for template aliase.
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

//! Gauss integrate point and weight.
// std::tuple<vecX_<>, vecX_<>> gauss_quad(int n = 2);
template <size_t nPoints=2>
constexpr std::optional<std::pair<>> gauss_quad();
}  // namespace cafea
#endif  // CAFEA_UTILS_EXT_H_
