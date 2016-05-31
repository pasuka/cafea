#ifndef BASE_H
#define BASE_H

#include <cmath>
#include <tuple>
#include <array>
#include <string>
#include <vector>
#include <limits>
#include <chrono>
#include <complex>
#include <cassert>
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
/**
 *  Basic parent object.
 */
class ObjectBase {
	public:
		//! A constructor.
		ObjectBase(){};
		//! Another constructor.
		ObjectBase(int id, std::string s):id_(id), name_(s) {assert(id_>0);};
		//! Another constructor.
		ObjectBase(int id):id_(id) {assert(id_>0);};
		//! A destructor.
		virtual ~ObjectBase(){};
		//! Set numeric number for object's name.
		template <class T>
		void set_name(T val){name_ = std::to_string(val);};
		//! Set object's name.
		void set_name(std::string s){name_ = s;};
		void set_name(const char *cs){name_ = cs;};
		//! Set object's id.
		void set_id(int x){id_ = x;};
		//! Set object's group via C++11. 
		/**
		 *  \code{.cpp}
		 *  ObjectBase a;
		 *  a.set_group({1,3,4,5,8,2});
		 *  \endcode
		 */
		void set_group(std::initializer_list<int> abc)
		{
			assert(abc.size()<=8);
			int i{0};
			for(auto const &it: abc)group_[i++] = it;
		}
		//! Set object's group via C-style.
		void set_group(const int y[], int n)
		{
			assert(n<=8);
			for(int i=0; i<n; i++)group_[i] = y[i];
		};
		//! Get object's name.
		std::string get_name() const {return name_;};
		//! Get object's id.
		int get_id() const {return id_;};
		//! Get object's group.
		std::array<int, 8> get_group() const {return group_;};
		//! Print object's id and name.
		friend std::ostream& operator<<(std::ostream& cout, const ObjectBase &a)
		{
			cout << "Object id:\t" << a.id_ << "\tname:\t" << a.name_ << "\n";
			return cout;
		}
		
	protected:
		int id_{-1};//!< Object's id.
		std::array<int, 8> group_{{-1, -1, -1, -1, -1, -1, -1, -1}};//!< Object's group array.
		std::string name_{"This is a base object!"};//!< Object's name.
};

/**
 *  Timer object.
 */
class Timer{
	public:
		//! A constructor.
		Timer(): beg_(clock_::now()){};
		//! Rest timer.
		void reset(){beg_ = clock_::now();};
		/**
		 *  \brief Get time consuming.
		 *  \param [out] time elapsed.
		 */
		double elapsed() const {
			return std::chrono::duration_cast<second_>(clock_::now()-beg_).count();
		};
		//! Print time cost.
		friend std::ostream& operator<<(std::ostream& cout, const Timer &a)
		{
			return cout << "Time cost:\t" << a.elapsed() << "seconds.\n";
		}
	private:
		using clock_ = std::chrono::high_resolution_clock;
		using second_ = std::chrono::duration<double, std::ratio<1>>;
		std::chrono::time_point<clock_> beg_;//!< Begin clock.
};

//! Gauss integrate point and weight.
std::tuple<Eigen::VectorXd, Eigen::VectorXd> gauss_quad(int);
}
#endif