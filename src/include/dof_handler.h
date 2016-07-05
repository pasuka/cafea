#ifndef DOF_HANDLER_H
#define DOF_HANDLER_H

#include <vector>
#include <cstddef>

namespace cafea
{
/**
 *  DOF handler
 */
class DofHandler{
	public:
		size_t get_num_dofs() const {return dofs_.size();};
		void set_num_dofs();
		void set_active(bool stat){activate_ = stat;};
		bool get_active()const{return activate_;};
		std::vector<int> get_dofs() const {return dofs_;};
	private:
		std::vector<int> dofs_;
		bool activate_{false};
	
};
}
#endif