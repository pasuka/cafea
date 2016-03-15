#ifndef BOUNDARY_H
#define BOUNDARY_H

#include <cassert>
#include <iostream>

#include "cppformat/format.h"

#include "base.h"

namespace cafea
{
/**
 *  Boundary type enum.
 */
enum struct BoundaryType {
	FIX_ALL,
	FIX_U_ALL,
	FIX_UR_ALL,
	FIX_UX, FIX_UY, FIX_UZ,
	FIX_URX, FIX_URY, FIX_URZ,
	UNKNOWN,
};

/**
 *  Boundary definition.
 */
class Boundary: public ObjectBase {
	public:
		using ObjectBase::ObjectBase;// Inherit Base's constructors.
		//! A constructor.
		Boundary(){};
		/**
		 *  \brief Constructor with id boundary type.
		 *  \param [in] id boundary's id.
		 *  \param [in] type boundary's type.
		 */
		Boundary(int id, BoundaryType bc):val_(bc),
			ObjectBase{id, fmt::format("Boundary#{0}", id)} {assert(id_>0);};
		//! A destructor.
		~Boundary(){};
		//! Set boundary type.
		void set_boundary_type(BoundaryType bc) {val_ = bc;};
		//! Get boundary type.
		BoundaryType get_boundary_type() const {return val_;};
		//! Print boundary.
		friend std::ostream& operator<<(std::ostream& cout, const Boundary &a)
		{
			cout << a.get_name() << "\t";
			switch(a.get_boundary_type()){
			case BoundaryType::FIX_ALL:
				cout << "Fix all dofs.";
				break;
			case BoundaryType::FIX_U_ALL:
				cout << "Fix ux uy uz.";
				break;
			case BoundaryType::FIX_UR_ALL:
				cout << "Fix rotx roty rotz.";
				break;
			case BoundaryType::FIX_URX:
				cout << "Fix rotx.";
				break;
			case BoundaryType::FIX_URY:
				cout << "Fix roty.";
				break;
			case BoundaryType::FIX_URZ:
				cout << "Fix rotz.";
				break;
			case BoundaryType::FIX_UX:
				cout << "Fix ux.";
				break;
			case BoundaryType::FIX_UY:
				cout << "Fix uy.";
				break;
			case BoundaryType::FIX_UZ:
				cout << "Fix uz.";
				break;
			default:
				cout << "Unkown.";
			}
			return cout << "\n";
		};
	private:
		BoundaryType val_ = BoundaryType::UNKNOWN;//!< Enumerate of boundary.
};
}
#endif