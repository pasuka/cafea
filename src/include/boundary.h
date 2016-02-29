#ifndef BOUNDARY_H
#define BOUNDARY_H

#include <cassert>


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
class Boundary: public OjbectBase {
	public:
		//! A constructor.
		Boundary(){};
		/**
		 *  \brief Constructor with id boundary type.
		 *  \param [in] id boundary's id.
		 *  \param [in] type boundary's type.
		 */
		Boundary(int id, BoundaryType bc):id(id), m_val(bc), name("Boundary"){};
		// Boundary(int id, int type):id(id), type_(type), name("Boundary"){};
		//! A destructor.
		~Boundary(){};
		/* //! Set boundary type.
		void set_boundary_type(int i){m_type = i;};
		//! Get boundary type.
		int get_boundary_type() const {return m_type;}; */
		//! Set boundary type.
		void set_boundary_type(BoundaryType bc){m_val = bc;};
		//! Get boundary type.
		BoundaryType get_boundary_type() const {return m_val;};
	private:
		// int m_type{-1};//!< Type of boundary condition.
		BoundaryType m_val = BoundaryType::UNKNOWN;//!< Enumerate of boundary.
};
}
#endif