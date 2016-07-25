#ifndef BOUNDARY_H
#define BOUNDARY_H

#include <iostream>

#include "fmt/format.h"

#include "base.h"
#include "dof_handler.h"

namespace cafea
{
/**
 *  Boundary type enum.
 */
enum struct BoundaryType {
	FIXED,
	INIT_VAL,
	UNKNOWN,
};

/**
 *  Boundary definition.
 */
template <class T=float>
class Boundary: public ObjectBase {
	public:
		using ObjectBase::ObjectBase;// Inherit Base's constructors.
		Boundary()=delete;
		/**
		 *  \brief Constructor with id boundary type.
		 *  \param [in] id boundary's id.
		 *  \param [in] bc boundary's type.
		 *  \param [in] dl dof label.
		 */
		Boundary(int id, BoundaryType bc, DofLabel dl): bc_(bc), dl_(dl),
			ObjectBase{id, fmt::format("Boundary#{0}", id)}{};
		/**
		 *  \brief Constructor with id boundary type values.
		 *  \param [in] id boundary's id.
		 *  \param [in] type boundary's type.
		 *  \param [in] value of boundary.
		 */
		Boundary(int id, BoundaryType bc, DofLabel dl, T val): bc_(bc), dl_(dl),
			val_(val), ObjectBase{id, fmt::format("Boundary#{0}", id)}{};
		//! A destructor.
		~Boundary(){};
		//! Set boundary type.
		void set_boundary_type(BoundaryType bc) {bc_ = bc;};
		//! Get boundary type.
		BoundaryType get_boundary_type() const {return bc_;};
		//! Set dof label.
		void set_dof_label(DofLabel t) {dl_ = t;};
		//! Get dof label.
		DofLabel get_dof_label() const {return dl_;};
		//! Set boundary value.
		void set_boundary_val(T bv) {val_ = bv;};
		//! Get boundary value.
		T get_boundary_val() const {return val_;};
		//! Print boundary.
		friend std::ostream& operator<<(std::ostream& cout, const Boundary &a)
		{
			cout << a.get_name() << "\t";
			
			switch(a.dl_){
			case DofLabel::UX: cout << "UX"; break;
			case DofLabel::UY: cout << "UY"; break;
			case DofLabel::UZ: cout << "UZ"; break;
			case DofLabel::URX: cout << "URX"; break;
			case DofLabel::URY: cout << "URY"; break;
			case DofLabel::URZ: cout << "URZ"; break;
			case DofLabel::U_ALL: cout << "UX UY UZ"; break;
			case DofLabel::UR_ALL: cout << "URX URY URZ"; break;
			case DofLabel::ALL: cout << "ALL dofs"; break;
			default: cout << "Unknown.";
			}
			
			switch(a.get_boundary_type()){
			case BoundaryType::FIXED: cout << "Fixed "; break;
			case BoundaryType::INIT_VAL: cout << "Initial value " << a.val_; break;
			default: cout << "Unknown.";
			}
			return cout << "\n";
		};
	private:
		BoundaryType bc_ = BoundaryType::UNKNOWN;//!< Enumerate of boundary.
		DofLabel dl_ = DofLabel::UNKNOWN;//!< Enumerate of dof label.
		T val_ = T(0.0);//!< Value of boundary.
};
}
#endif