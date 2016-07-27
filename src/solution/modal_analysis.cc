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
 *  \brief Get model info.
 *  \return array that contain model statistics.
 */
template <class FileReader, class Scalar, class ResultScalar>
std::array<size_t, 5> SolutionModal<FileReader, Scalar, ResultScalar>::get_info()const
{
	std::array<size_t, 5> param{this->node_group_.size(), this->elem_group_.size(),
		this->matl_group_.size(), this->sect_group_.size(), this->bc_group_.size()};
#if(PRINT_ON==1)		
	fmt::print("Number of node:{}\n", param[0]);
	fmt::print("Number of element:{}\n", param[1]);
	fmt::print("Number of material:{}\n", param[2]);
	fmt::print("Number of section:{}\n", param[3]);
	fmt::print("Number of boundary:{}\n", param[4]);
#endif	
	return param;
};
/**
 *  \brief Check model.
 */
template <class FileReader, class Scalar, class ResultScalar>
void SolutionModal<FileReader, Scalar, ResultScalar>::check()
{
	fmt::print("PASS\n");
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
		}
		else{
			fmt::print("Duplicated node id:{}\n", p_node->id_);
		}
	}
	for(int i=0; i<a2; i++, p_elem++){
		auto got = (*this).elem_group_.find(p_elem->id_);
		if(got==(*this).elem_group_.end()){
			(*this).elem_group_[p_elem->id_] = wrapper_::convert2elem<ResultScalar,
				ResultScalar>(p_elem);
		}
		else{
			fmt::print("Duplicated element id:{}\n", p_elem->id_);
		}
	}
	
	for(int i=0; i<a3; i++, p_real++){
		auto got = this->sect_group_.find(p_real->id_);
		if(got==this->sect_group_.end()){
			(*this).sect_group_[p_real->id_] = wrapper_::convert2sect<Scalar>(p_real);
		}
		else{
			fmt::print("Duplicated section id:{}\n", p_real->id_);
		}
	}
	for(int i=0; i<a4; i++, p_matl++){
		auto got = this->matl_group_.find(p_matl->id_);
		if(got==this->matl_group_.end()){
			(*this).matl_group_[p_matl->id_] = wrapper_::convert2matl<Scalar>(p_matl);
		}
		else{
			fmt::print("Duplicated material id:{}\n", p_matl->id_);
		}
	}
	(*this).file_parser_.clean_model();
};

template <class FileReader, class Scalar, class ResultScalar>
void SolutionModal<FileReader, Scalar, ResultScalar>::assembly()
{
	fmt::print("Empty\n");
};

template <class FileReader, class Scalar, class ResultScalar>
void SolutionModal<FileReader, Scalar, ResultScalar>::solve()
{
	fmt::print("Empty\n");
};

template <class FileReader, class Scalar, class ResultScalar>
void SolutionModal<FileReader, Scalar, ResultScalar>::post_process()
{
	fmt::print("Empty\n");
};
}