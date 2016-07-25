#ifndef DOF_HANDLER_H
#define DOF_HANDLER_H

#include <cstddef>
#include <vector>

namespace cafea
{
/**
 *  DOF type enum.
 */
enum struct DofType {
	ELIMINATE=-1,
	NORMAL,
	MASTER,
	SLAVE,
	FORCED,
	UNKNOWN,
};
/**
 *  DOF label enum.
 */
enum struct DofLabel {
	UX=0, UY, UZ,
	URX, URY, URZ,
	WARP,
	U_ALL, UR_ALL, ALL,
	UNKNOWN,
};
/**
 *  DOF handler
 */
class DofHandler{
	public:
		//! Get number of DOF.
		size_t get_num_dofs() const {return dofs_.size();};
		//! Set number of DOF.
		void set_num_dofs(int val);
		//! Clear.
		void clear(){dofs_.clear();};
		//! Get DOF vector.
		std::vector<int> get_dofs() const {return dofs_;};
		
	private:
		std::vector<int> dofs_;//!< Storage of dofs.
		DofType dt_ = DofType::UNKNOWN;//!< Type of dof.
};
}
#endif