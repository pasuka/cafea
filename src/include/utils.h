#ifndef UTILS_H
#define UTILS_H

#include <cmath> //! C-style header.

#include <tuple>
#include <vector>
#include <limits>
#include <string>
#include <complex>

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

//! Initializer list.
template <class T>
using init_list_ = std::initializer_list<T>;
//! Dictionary.
template <class T>
using dict_ = std::unordered_map<int, T>;

//! Gauss integrate point and weight.
std::tuple<vecX_<>, vecX_<>> gauss_quad(int n=2);

//! Starts with special string.
bool startswith(std::string s1, std::string header);
//! Convert to integer.
int str2int(std::string a);
//! Convert to float.
float str2float(std::string a);
}
#endif
