#ifndef UTILS_H
#define UTILS_H

#include <cmath>

#include <tuple>
#include <limits>
#include <complex>
#include <unordered_map>
#include <initializer_list>

#include <Eigen/Dense>

namespace cafea
{
//! Precision of machine via different types.	
template<class T>
constexpr T EPS() {return std::numeric_limits<T>::epsilon();};
//! M_PI.
template<class T>
constexpr T PI() {return T(std::atan(1.0)*4.0);};
//! Vector3.
template <class T>
using vec3_ = Eigen::Matrix<T, 3, 1>;
//! Vector.
template <class T>
using vecX_ = Eigen::Matrix<T, Eigen::Dynamic, 1>;
//! Matirx.
//! C++11's trick for template aliase.
template <class U>
using matrix_ = Eigen::Matrix<U, Eigen::Dynamic, Eigen::Dynamic>;
//! Matlab style varargout with 2 return.
//! Template aliases trick in C++11.
template <class T>
using varargout_2_ = std::tuple<T, matrix_<T> >; 
//! Matlab style varargout with 3 return.
template <class T>
using varargout_3_ = std::tuple<T, matrix_<T>, matrix_<T> >;
//! Initializer list.
template <class T>
using init_list_ = std::initializer_list<T>;
//! Dictionary.
template <class T>
using dict_ = std::unordered_map<int, T>;
//! REAL(kind=4) in Fortran iso_c_binding.
using REAL4 = float;
//! REAL(kind=8) in Fortran iso_c_binding.
using REAL8 = double;
//! COMPLEX(kind=4) in Fortran iso_c_binding.
using COMPLEX4 = std::complex<float>;
//! COMPLEX(kind=8) in Fortran iso_c_binding.
using COMPLEX8 = std::complex<double>;

//! Gauss integrate point and weight.
std::tuple<Eigen::VectorXd, Eigen::VectorXd> gauss_quad(int);	
}
#endif