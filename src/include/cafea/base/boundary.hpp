/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#ifndef CAFEA_BOUNDARY_HPP_
#define CAFEA_BOUNDARY_HPP_

#include <type_traits>

#include "fmt/format.h"

#include "cafea/base/enum_lib.hpp"
#include "cafea/base/base.hpp"
#include "cafea/fortran/fortran_wrapper.hpp"

namespace cafea {
/**
 *  \class Boundary definition.
 */
template <class T = REAL4>
class Boundary: public ObjectBase {
	static_assert(std::is_floating_point<T>::value, "Boundary<T>: T must be floating type.");
	public:
		using ObjectBase::ObjectBase;// Inherit Base's constructors.
		Boundary() = delete;
		/**
		 *  \brief Constructor with id boundary type.
		 *  \param [in] id boundary's id.
		 *  \param [in] bc boundary's type.
		 *  \param [in] dl dof label.
		 */
		Boundary(int id, BoundaryType bc, DofLabel dl): bc_(bc), dl_(dl),
			ObjectBase {id, "Boundary"} {}
		/**
		 *  \brief Constructor with id boundary type values.
		 *  \param [in] id boundary's id.
		 *  \param [in] type boundary's type.
		 *  \param [in] value of boundary.
		 */
		Boundary(int id, BoundaryType bc, DofLabel dl, T val): bc_(bc), dl_(dl),
			val_(val), ObjectBase {id, "Boundary"} {}
		Boundary(const wrapper_::bndy_f03 *p): ObjectBase {p->id_, "Boundary"} {}
		Boundary(const wrapper_::cdb_bc *p): ObjectBase {p->id_, "Boundary"} {}
		//! A destructor.
		~Boundary() override {}
		//! Set boundary type.
		void set_boundary_type(BoundaryType bc) { bc_ = bc;}
		//! Get boundary type.
		BoundaryType get_boundary_type() const { return bc_;}
		//! Set dof label.
		void set_dof_label(DofLabel t) { dl_ = t;}
		//! Get dof label.
		DofLabel get_dof_label() const { return dl_;}
		//! Set boundary value.
		void set_boundary_val(T bv) { val_ = bv;}
		//! Get boundary value.
		T get_boundary_val() const { return val_;}
		//! Print boundary.
		friend std::ostream& operator<<(std::ostream& cout, const Boundary &a) {
			return cout << fmt::format("{}\n", a.name_);
		}

	private:
		BoundaryType bc_ = BoundaryType::UNKNOWN;//!< Enumerate of boundary.
		DofLabel dl_ = DofLabel::UNKNOWN;//!< Enumerate of dof label.
		T val_ = T(0.0);//!< Value of boundary.
};
//! Specialization.
//template class Boundary<REAL4>;
//template class Boundary<REAL8>;
}  // namespace cafea
#endif  // CAFEA_BOUNDARY_HPP_
