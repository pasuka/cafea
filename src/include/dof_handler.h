#ifndef DOF_HANDLER_H
#define DOF_HANDLER_H

#include <vector>
#include <cstddef>

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
};
/**
 *  DOF label enum.
 */
enum struct DofLabel {
	UX=0, UY, UZ,
	URX, URY, URZ,
	WARP,
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
		//! Activate DOF handler.
		void activate(bool stat=true){activate_ = stat;if(!stat)dofs_.clear();};
		//! Check status of DOF handler.
		bool is_activated()const{return activate_;};
		//! Clear.
		void clear(){dofs_.clear();};
		//! Get DOF vector.
		std::vector<int> get_dofs() const {return dofs_;};
		
	private:
		std::vector<int> dofs_;//!< Storage of dofs.
		bool activate_{false};//!< Status of dof handler.
};
}
#endif