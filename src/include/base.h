#ifndef BASE_H
#define BASE_H

#include <cmath>
#include <cassert>
#include <tuple>
#include <array>
#include <string>
#include <vector>
#include <limits>
#include <chrono>
#include <initializer_list>

#include <Eigen/Dense>

namespace cafea
{
//! Precision of machine via different types.	
template<class T>
constexpr T EPS() {return std::numeric_limits<T>::epsilon();};
//! M_PI
template<class T>
constexpr T PI() {return T(std::atan(1.0)*4.0);};

/**
 *  Basic parent object.
 */
class ObjectBase {
	public:
		//! A constructor.
		ObjectBase(){};
		//! A destructor.
		virtual ~ObjectBase(){};
		//! Set numeric number for object's name.
		template <class T>
		void set_name(T val){name = std::to_string(val);};
		//! Set object's name.
		void set_name(std::string s){name = s};
		void set_name(const char *cs){name = cs;};
		//! Set object's id.
		void set_id(int x){id = x;};
		//! Set object's group via C++11. 
		/**
		 *  \code{.cpp}
		 *  ObjectBase a;
		 *  a.set_group({1,3,4,5,8,2});
		 *  \endcode
		 */
		void set_group(std::initializer_list<int> abc){
			assert(abc.size()<=8);
			int i{0};
			for(auto const &it: abc)group[i++] = it;
		}
		//! Set object's group via C-style.
		void set_group(const int y[], int n){
			assert(n<=8);
			for(int i=0; i<n; i++)group[i] = y[i];
		};
		//! Get object's name.
		std::string get_name() const {return name;};
		//! Get object's id.
		int get_id() const {return id;};
		//! Get object's group.
		std::array<int, 8> get_group() const {return group;};
		
	protected:
		int id{-1};//!< Object's id.
		std::array<int, 8> group{-1, -1, -1, -1, -1, -1, -1, -1};//!< Object's group array.
		std::string name{"This is a base object!"};//!< Object's name.
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
	private:
		using clock_ = std::chrono::high_resolution_clock;
		using second_ = std::chrono::duration<double, std::ratio<1>>;
		std::chrono::time_point<clock_> beg_;//!< Begin clock.
};

//! Gauss integrate point and weight.
std::tuple<Eigen::VectorXd, Eigen::VectorXd> gauss_quad(int);
}
#endif