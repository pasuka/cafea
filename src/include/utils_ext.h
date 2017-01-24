#ifndef UTILS_EXT_H
#define UTILS_EXT_H

#include <Eigen/Dense>

#include "utils.h"

namespace cafea
{
//! Vector3.
template <class T=REAL8>
using vec3_ = Eigen::Matrix<T, 3, 1>;
//! Vector.
template <class T=REAL8>
using vecX_ = Eigen::Matrix<T, Eigen::Dynamic, 1>;
//! Matirx.
//! C++11's trick for template aliase.
template <class U=REAL8>
using matrix_ = Eigen::Matrix<U, Eigen::Dynamic, Eigen::Dynamic>;
//! Matlab style varargout with 2 return.
//! Template aliases trick in C++11.
template <class T=REAL8>
using varargout_2_ = std::tuple<T, matrix_<T>>;
//! Matlab style varargout with 3 return.
template <class T=REAL8>
using varargout_3_ = std::tuple<T, matrix_<T>, matrix_<T>>;
//! Matlab style varargout with 4 return.
template <class T=REAL8>
using varargout_4_ = std::tuple<T, matrix_<T>, matrix_<T>, matrix_<T>>;
//! Matlab style varargout with 5 return.
template <class T=REAL8>
using varargout_5_ = std::tuple<T, matrix_<T>, matrix_<T>, matrix_<T>, matrix_<T>>;
//! Matlab style varargout with 6 return.
template <class T=REAL8>
using varargout_6_ = std::tuple<T, matrix_<T>, matrix_<T>, matrix_<T>, matrix_<T>, matrix_<T>>;
}
#endif
