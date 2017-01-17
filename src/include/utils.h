#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <cstddef>

#include <tuple>
#include <regex>
#include <vector>
#include <limits>
#include <string>
#include <complex>

#include <Eigen/Dense>
#include <Eigen/Geometry>

namespace cafea
{
//! REAL(kind=4) in Fortran iso_c_binding.
using REAL4 = float;
//! REAL(kind=8) in Fortran iso_c_binding.
using REAL8 = double;
//! Complex template.
template <class T=REAL8>
using COMPLEX = std::complex<T>;
//! COMPLEX(kind=4) in Fortran iso_c_binding.
using COMPLEX4 = COMPLEX<REAL4>;
//! COMPLEX(kind=8) in Fortran iso_c_binding.
using COMPLEX8 = COMPLEX<REAL8>;

//! Precision of machine via different types.
template<class T=REAL8>
constexpr T EPS() {return std::numeric_limits<T>::epsilon();};
//! M_PI.
template<class T=REAL8>
constexpr T PI() {return T(std::atan(1.0)*4.0);};
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

//! Gauss integrate point and weight.
std::tuple<vecX_<>, vecX_<>> gauss_quad(size_t);

//! Starts with special string.
bool startswith(std::string s1, std::string header);
//! Convert to integer.
int str2int(std::string a);
//! Convert to float.
float str2float(std::string a);
}
#endif
