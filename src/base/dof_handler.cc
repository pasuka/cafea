#include "cafea.h"

namespace cafea
{
/**
 *  \brief Set num of dofs.
 *  \param[in] val number of dofs.
 */
void DofHandler::set_num_dofs(size_t val)
{
	auto num = this->get_num_dofs();
	if(num<val){
		for(auto i=num; i<val; i++){
			this->dofs_.push_back(static_cast<int>(DofType::NORMAL));
		}
	}
}
/**
 *  \brief Set constraint.
 *  \param[in]
 *  \param[in]
 */
void DofHandler::set_constraint(DofLabel ml, DofType mt)
{
	switch(ml){
	case DofLabel::UX: 
	case DofLabel::UY: 
	case DofLabel::UZ:
	case DofLabel::URX:
	case DofLabel::URY:
	case DofLabel::URZ:
	case DofLabel::WARP:
		this->dofs_[static_cast<int>(ml)] = static_cast<int>(mt);
		break;
	case DofLabel::U_ALL:
		for(int i=0; i<3;)this->dofs_[i++] = static_cast<int>(mt);
		break;
	case DofLabel::UR_ALL:
		for(int i=3; i<6;)this->dofs_[i++] = static_cast<int>(mt);
		break;
	case DofLabel::ALL:
		for(int &it: this->dofs_)it = static_cast<int>(mt);
		break;
	case DofLabel::UNKNOWN:
	default:
		fmt::print("Unsupported dof label.\n");
	}
}
/**
 *  \brief Get constraint.
 *  \param[in]
 *  \return
 */
DofType DofHandler::get_constraint(DofLabel ml) const
{
	int val;
	switch(ml){
	case DofLabel::UX: 
	case DofLabel::UY: 
	case DofLabel::UZ:
	case DofLabel::URX:
	case DofLabel::URY:
	case DofLabel::URZ:
	case DofLabel::WARP: val = this->dofs_[static_cast<int>(ml)]; break;
	case DofLabel::U_ALL:
	case DofLabel::UR_ALL:
	case DofLabel::ALL:
	default: val = 999;
	}
	switch(val){
	case static_cast<int>(DofType::NORMAL): return DofType::NORMAL;
	case static_cast<int>(DofType::ELIMINATE): return DofType::ELIMINATE;
	case static_cast<int>(DofType::CONSTRAINT): return DofType::CONSTRAINT;
	case static_cast<int>(DofType::MASTER): return DofType::MASTER;
	case static_cast<int>(DofType::SLAVE): return DofType::SLAVE;
	case 999:
	default: return DofType::UNKNOWN;
	}
}
/**
 *  \brief
 *  \param[in]
 *  \param[in]
 */
void DofHandler::accum(int *jj, DofType mt)
{
	for(int &it: (*this).dofs_){
		if(static_cast<int>(mt)==it)it = (*jj)++;
	}	
}
}