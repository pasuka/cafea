#ifndef BOUNDARY_H
#define BOUNDARY_H

#include <iostream>

#include "fmt/format.h"

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
	UX, UY, UZ, URX, URY, URZ,
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
		 *  \param [in] type boundary's type.
		 */
		Boundary(int id, BoundaryType bc): bc_(bc),
			ObjectBase{id, fmt::format("Boundary#{0}", id)}{};
		/**
		 *  \brief Constructor with id boundary type values.
		 *  \param [in] id boundary's id.
		 *  \param [in] type boundary's type.
		 *  \param [in] value of boundary.
		 */
		Boundary(int id, BoundaryType bc, T val): bc_(bc), val_(val),
			ObjectBase{id, fmt::format("Boundary#{0}", id)}{};
		//! A destructor.
		~Boundary(){};
		//! Set boundary type.
		void set_boundary_type(BoundaryType bc) {bc_ = bc;};
		//! Get boundary type.
		BoundaryType get_boundary_type() const {return bc_;};
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
			case BoundaryType::UX:
				cout << fmt::format("Forced:{0} value:{1}", "ux", val_);
				break;
			case BoundaryType::UY:
				cout << fmt::format("Forced:{0} value:{1}", "uy", val_);
				break;
			case BoundaryType::UZ:
				cout << fmt::format("Forced:{0} value:{1}", "uz", val_);
				break;
			case BoundaryType::URX:
				cout << fmt::format("Forced:{0} value:{1}", "urx", val_);
				break;
			case BoundaryType::URY:
				cout << fmt::format("Forced:{0} value:{1}", "ury", val_);
				break;
			case BoundaryType::URZ:
				cout << fmt::format("Forced:{0} value:{1}", "urz", val_);
				break;
			default:
				cout << "Unkown.";
			}
			return cout << "\n";
		};
	private:
		BoundaryType bt_ = BoundaryType::UNKNOWN;//!< Enumerate of boundary.
		T val_ = T(0.0);//!< Value of boundary.
};
}
#endif