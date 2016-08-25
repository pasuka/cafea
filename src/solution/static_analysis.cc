#include "cafea.h"

namespace cafea
{
/**
 *  \brief Clear member variables.
 */
template <class FileReader, class Scalar, class ResultScalar>
void SolutionStatic<FileReader, Scalar, ResultScalar>::init()
{
	if(!this->node_group_.empty())this->node_group_.clear();
	if(!this->elem_group_.empty())this->elem_group_.clear();
	if(!this->matl_group_.empty())this->matl_group_.clear();
	if(!this->sect_group_.empty())this->sect_group_.clear();
	if(!this->load_group_.empty())this->load_group_.clear();
	if(!this->bc_group_.empty())this->bc_group_.clear();
	this->mat_pair_.clear();
};

/**
 *  \brief Load input file.
 *  \param[in] fn file path.
 */
template <class FileReader, class Scalar, class ResultScalar>
void SolutionStatic<FileReader, Scalar, ResultScalar>::load(const char* fn)
{
	auto flag = (*this).file_parser_.load_model(fn);
	
	wrapper_::node_f *p_node{nullptr};
	wrapper_::elem_f *p_elem{nullptr};
	wrapper_::matl_f *p_real{nullptr}, *p_matl{nullptr};
	int a1{0}, a2{0}, a3{0}, a4{0};
	(*this).file_parser_.get_data_ptr(
		&p_node, &p_elem, &p_matl, &p_real, &a1, &a2, &a3, &a4);
	
	assert(a1>0);
	assert(a2>0);
	assert(a3>0);
	assert(a4>0);
#if(PRINT_ON==1)
	fmt::print("*************************************************\n");
	fmt::print("Number of nodes:{}\nNumber of elements:{}\n", a1, a2);
	fmt::print("Number of materials:{}\nNumber of real constants:{}\n", a3, a4);
	fmt::print("*************************************************\n");
#endif
	for(int i=0; i<a1; i++, p_node++){
		auto got = (*this).node_group_.find(p_node->id_);
		if(got==(*this).node_group_.end()){
			(*this).node_group_[p_node->id_] = wrapper_::convert2node<Scalar,
				ResultScalar>(p_node);
			std::vector<int> tmp(p_node->boundary_, p_node->boundary_+7);
			if(std::any_of(tmp.cbegin(), tmp.cend(), [](int i){return i<0;})){
				for(size_t j=0; j<tmp.size(); j++){
					if(tmp[j]<0){
						switch(j){
						case 0: 
							this->bc_group_.push_back({p_node->id_,
								BoundaryType::FIXED, DofLabel::UX});
							break;
						case 1:
							this->bc_group_.push_back({p_node->id_,
								BoundaryType::FIXED, DofLabel::UY});
							break;
						case 2:
							this->bc_group_.push_back({p_node->id_,
								BoundaryType::FIXED, DofLabel::UZ});
							break;
						case 3:
							this->bc_group_.push_back({p_node->id_,
								BoundaryType::FIXED, DofLabel::URX});
							break;
						case 4:
							this->bc_group_.push_back({p_node->id_,
								BoundaryType::FIXED, DofLabel::URY});
							break;
						case 5:
							this->bc_group_.push_back({p_node->id_,
								BoundaryType::FIXED, DofLabel::URZ});
							break;
						case 6:
							this->bc_group_.push_back({p_node->id_,
								BoundaryType::FIXED, DofLabel::WARP});
							break;
						default:
							fmt::print("Only support 6 dofs now\n");
						}
					}
				}
				// fmt::print("Boundary apply on node {}\n", p_node->id_);
			}
		}
		else{
			fmt::print("Duplicated node id:{}\n", p_node->id_);
		}
	}
	for(int i=0; i<a2; i++, p_elem++){
		auto got = (*this).elem_group_.find(p_elem->id_);
		if(got==(*this).elem_group_.end()){
			(*this).elem_group_[p_elem->id_] = wrapper_::convert2elem<ResultScalar>(p_elem);
		}
		else{
			fmt::print("Duplicated element id:{}\n", p_elem->id_);
		}
	}
	
	for(int i=0; i<a4; i++, p_real++){
		auto got = this->sect_group_.find(p_real->id_);
		if(got==this->sect_group_.end()){
			(*this).sect_group_[p_real->id_] = wrapper_::convert2sect<Scalar>(p_real);
		}
		else{
			fmt::print("Duplicated section id:{}\n", p_real->id_);
		}
	}
	for(int i=0; i<a3; i++, p_matl++){
		auto got = this->matl_group_.find(p_matl->id_);
		if(got==this->matl_group_.end()){
			(*this).matl_group_[p_matl->id_] = wrapper_::convert2matl<Scalar>(p_matl);
		}
		else{
			fmt::print("Duplicated material id:{}\n", p_matl->id_);
		}
	}
	(*this).file_parser_.clean_model();	
}
}