#include "cafea.h"

namespace cafea
{
/**
 *  \brief Initilize variables.
 */
template <class FileReader, class Scalar, class ResultScalar>
void SolutionModal<FileReader, Scalar, ResultScalar>::init()
{
	if(!(*this).node_group_.empty())this->node_group_.clear();
	if(!(*this).elem_group_.empty())this->elem_group_.clear();
	if(!(*this).matl_group_.empty())this->matl_group_.clear();
	if(!(*this).sect_group_.empty())this->sect_group_.clear();
	if(!(*this).bc_group_.empty())this->bc_group_.clear();
	this->mat_pair_.clear();
	(*this).mode_shape_.resize(0, 0);
	(*this).natural_freq_.resize(0, 0);
};

/**
 *  \brief Check model.
 */
template <class FileReader, class Scalar, class ResultScalar>
void SolutionModal<FileReader, Scalar, ResultScalar>::check()
{
	
};

/**
 *  \brief Load input file.
 *  \param[in] fn file path.
 */
template <class FileReader, class Scalar, class ResultScalar>
void SolutionModal<FileReader, Scalar, ResultScalar>::load(const char* fn)
{
	auto flag = (*this).file_parser_.load_model(fn);
	
	wrapper_::node_f *p_node{nullptr};
	wrapper_::elem_f *p_elem{nullptr};
	wrapper_::matl_f *p_real{nullptr}, *p_matl{nullptr};
	int a1{0}, a2{0}, a3{0}, a4{0};
	(*this).file_parser_.get_data_ptr(
		&p_node, &p_elem, &p_real, &p_matl, &a1, &a2, &a3, &a4);
	
	assert(a1>0);
	assert(a2>0);
	assert(a3>0);
	assert(a4>0);
	fmt::print("*************************************************\n");
	fmt::print("Number of nodes:{}\nNumber of elements:{}\n", a1, a2);
	fmt::print("Number of materials:{}\nNumber of real constants:{}\n", a3, a4);
	fmt::print("*************************************************\n");
	for(int i=0; i<a1; i++, p_node++){
		auto got = (*this).node_group_.find(p_node->id_);
		if(got!=(*this).node_group_.end()){
			(*this).node_group_[p_node->id_] = Node<Scalar, ResultScalar>(
				p_node->id_, p_node->xyz_[0], p_node->xyz_[1], p_node->xyz_[2],
				p_node->rot_[0], p_node->rot_[1], p_node->rot_[2]);
		}
		else{
			fmt::print("Duplicated node id:{}\n", p_node->id_);
		}
	}
	for(int i=0; i<a2; i++, p_elem++){
		auto got = (*this).elem_group_.find(p_elem->id_);
		if(got!=(*this).elem_group_.end()){
			/*(*this).elem_group_[p_elem->id_] = Element<Scalar, ResultScalar>(
				p_elem->id_, p_elem->etype_, p_elem->prop_[0], p_elem->prop_[1],
				);*/
		}
		else{
			fmt::print("Duplicated element id:{}\n", p_elem->id_);
		}
	}
	for(int i=0; i<a3; i++, p_matl++){
		auto got = this->matl_group_.find(p_matl->id_);
		if(got!=this->matl_group_.end()){
			
		}
		else{
			fmt::print("Duplicated material id:{}\n", p_matl->id_);
		}
	}
	for(int i=0; i<a4; i++, p_real++){
		auto got = this->sect_group_.find(p_real->id_);
		if(got!=this->sect_group_.end()){
			
		}
		else{
			fmt::print("Duplicated section id:{}\n", p_real->id_);
		}
	}
	(*this).file_parser_.clean_model();
};
}