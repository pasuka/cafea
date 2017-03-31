/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#ifndef CAFEA_BASE_H_
#define CAFEA_BASE_H_

#include <cassert>
#include <array>
#include <string>
#include <ostream>
#include <iterator>
#include <algorithm>

#include "fmt/format.h"

#include "cafea/utils.h"

namespace cafea {
/**
 * \class ObjectBase
 *  Basic parent object.
 */
class ObjectBase {
	public:
		//! Constructor.
		ObjectBase() { group_.fill(0);}
		//! Another constructor.
		ObjectBase(int id, std::string s): id_(id), name_(s) {
			assert(id_ > 0);
			group_.fill(0);
		}
		//! Another constructor.
		explicit ObjectBase(int id): id_(id) {
			assert(id_ > 0);
			group_.fill(0);
		}
		//! A destructor.
		virtual ~ObjectBase() {}
		//! Set object's name.
		template <class T>
		void set_name(T val) { name_ = fmt::format("{}", val);}
		/*
		//! Set numeric number for object's name.
		template <class T>
		void set_name(T val)
		{
			static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value);
			name_ = std::to_string(val);
		};
		//! Set object's name.
		void set_name(std::string s) { name_ = s;};
		void set_name(const char *cs) { name_ = cs;};
		*/
		//! Set object's id.
		void set_id(int x) {
			assert(x > 0);
			id_ = x;
		}
		//! Set object's group via C++11.
		/**
		 *  \code{.cpp}
		 *  ObjectBase a;
		 *  a.set_group({1,3,4,5,8,2});
		 *  \endcode
		 */
		void set_group(init_list_<int> abc) {
			assert(abc.size() <= 10);
			std::copy(abc.begin(), abc.end(), group_.begin());
		}
		//! Set object's group via C-style.
		void set_group(const int y[], int n) {
			assert(n <= 10);
			for (int i = 0; i < n; i++) group_[i] = y[i];
		}
		//! Get object's name.
		std::string get_name() const { return name_;}
		//! Get object's id.
		int get_id() const { return id_;}
		//! Get object's group.
		std::array<int, 10> get_group() const { return group_;}
		//! Print object's id and name.
		friend std::ostream& operator<<(std::ostream& cout, const ObjectBase &a) {
			return cout << fmt::format("Object id:{} name:{}\n", a.id_, a.name_);
		}

	protected:
		int id_{-1};//!< Object's id.
		std::array<int, 10> group_;//!< Object's group array.
		std::string name_{"Empty"};//!< Object's name.
};
}  // namespace cafea
#endif  // CAFEA_BASE_H_
