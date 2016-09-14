#include "cafea.h"

using std::type_index;

namespace cafea
{
/**
 *  \brief Clear member variables.
 */
template <class FileReader, class Scalar, class ResultScalar>
void SolutionStatic<FileReader, Scalar, ResultScalar>::clear()
{
	if(!this->node_group_.empty())this->node_group_.clear();
	if(!this->elem_group_.empty())this->elem_group_.clear();
	if(!this->matl_group_.empty())this->matl_group_.clear();
	if(!this->sect_group_.empty())this->sect_group_.clear();
	if(!this->load_group_.empty())this->load_group_.clear();
	if(!this->bc_group_.empty())this->bc_group_.clear();
	this->mat_pair_.clear();
	if(this->solver_)this->solver_.reset(nullptr);
};
/**
 *  \brief Initliazation member variables.
 */
template <class FileReader, class Scalar, class ResultScalar>
void SolutionStatic<FileReader, Scalar, ResultScalar>::init()
{
	this->clear();
	std::unique_ptr<LinearSolver<ResultScalar>> p(new LinearSolver<ResultScalar>);
	this->solver_ = std::move(p);
};
/**
 *  \brief Load input file.
 *  \param[in] fn file path.
 */
template <class FileReader, class Scalar, class ResultScalar>
void SolutionStatic<FileReader, Scalar, ResultScalar>::load(const char* fn)
{
	auto flag = (*this).file_parser_.load_model(fn);
	
	auto is_cdb = [this](){return type_index(typeid(file_parser_))==type_index(typeid(AnsysCdbReader<float>));};
	auto is_bcy = [this](){return type_index(typeid(file_parser_))==type_index(typeid(BcyReader<float>));};
	wrapper_::AdapterF2Cpp<Scalar, ResultScalar> f2cpp;
	
	if(is_cdb()){	
		wrapper_::node_f *p_node{nullptr};
		wrapper_::elem_f *p_elem{nullptr};
		wrapper_::matl_f *p_real{nullptr}, *p_matl{nullptr};
		int a1{0}, a2{0}, a3{0}, a4{0};
		wrapper_::model_data_ptr(
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
				(*this).node_group_[p_node->id_] = f2cpp.cdb2node(p_node);
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
				(*this).elem_group_[p_elem->id_] = f2cpp.cdb2elem(p_elem);
			}
			else{
				fmt::print("Duplicated element id:{}\n", p_elem->id_);
			}
		}
		
		for(int i=0; i<a4; i++, p_real++){
			auto got = this->sect_group_.find(p_real->id_);
			if(got==this->sect_group_.end()){
				(*this).sect_group_[p_real->id_] = f2cpp.cdb2sect(p_real);
			}
			else{
				fmt::print("Duplicated section id:{}\n", p_real->id_);
			}
		}
		for(int i=0; i<a3; i++, p_matl++){
			auto got = this->matl_group_.find(p_matl->id_);
			if(got==this->matl_group_.end()){
				(*this).matl_group_[p_matl->id_] = f2cpp.cdb2matl(p_matl);
			}
			else{
				fmt::print("Duplicated material id:{}\n", p_matl->id_);
			}
		}
	}
	else{
		if(is_bcy()){
			fmt::print("This is BCY reader.\n");
			wrapper_::node_bcy *p_node{nullptr};
			wrapper_::elem_bcy *p_elem{nullptr};
			wrapper_::matl_bcy *p_matl{nullptr};
			wrapper_::sect_bcy *p_sect{nullptr};
			wrapper_::load_bcy *p_load{nullptr};
			wrapper_::bndy_bcy *p_bndy{nullptr};
			int a1{0}, a2{0}, a3{0}, a4{0}, a5{0}, a6{0};
			wrapper_::model_data_ptr_bcy(&p_node, &p_elem, &p_matl, &p_sect, 
				&p_bndy, &p_load, &a1, &a2, &a3, &a4, &a5, &a6);
			assert(a1>0);
			assert(a2>0);
			assert(a3>0);
			assert(a4>0);
			for(int i=0; i<a1; i++, p_node++){
				auto got = this->node_group_.find(p_node->id_);
				if(got==this->node_group_.end()){
					this->node_group_[p_node->id_] = f2cpp.bcy2node(p_node);
				}
				else{
					fmt::print("Duplicated node id:{}\n", p_node->id_);
				}
			}
			for(int i=0; i<a2; i++, p_elem++){
				auto got = this->elem_group_.find(p_elem->id_);
				if(got==this->elem_group_.end()){
					this->elem_group_[p_elem->id_] = f2cpp.bcy2elem(p_elem);
				}
				else{
					fmt::print("Duplicated element id:{}\n", p_elem->id_);
				}
			}
			for(int i=0; i<a3; i++, p_matl++){
				auto got = this->matl_group_.find(p_matl->id_);
				if(got==this->matl_group_.end()){
					this->matl_group_[p_matl->id_] = f2cpp.bcy2matl(p_matl);
				}
				else{
					fmt::print("Duplicated material id:{}\n", p_matl->id_);
				}
			}
			for(int i=0; i<a4; i++, p_sect++){
				auto got = this->sect_group_.find(p_sect->id_);
				if(got==this->sect_group_.end()){
					this->sect_group_[p_sect->id_] = f2cpp.bcy2sect(p_sect);
				}
				else{
					fmt::print("Duplicated section id:{}\n", p_sect->id_);
				}
			}
			if(a5>0){
				for(int i=0; i<a5; i++)this->bc_group_.push_back(f2cpp.bcy2bndy(p_bndy++));
			}
			if(a6>0){
				for(int i=0; i<a6; i++)this->load_group_.push_back(f2cpp.bcy2load(p_load++));
			}
		}
	}
	(*this).file_parser_.clean_model();	
}

/**
 *  \brief Get model info.
 *  \return array that contain model statistics.
 */
template <class FileReader, class Scalar, class ResultScalar>
std::array<size_t, 5> SolutionStatic<FileReader, Scalar, ResultScalar>::get_info()const
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
void SolutionStatic<FileReader, Scalar, ResultScalar>::check()
{
	if(this->node_group_.empty())fmt::print("None node in model\n");
	if(this->elem_group_.empty())fmt::print("None element in model\n");
	if(this->matl_group_.empty())fmt::print("None material in model\n");
	if(this->sect_group_.empty())fmt::print("None section in model\n");
	if(this->load_group_.empty())fmt::print("None load in model\n");
	if(this->bc_group_.empty())fmt::print("None boundary in model\n");
	fmt::print("Model has been checked!\n");
};

/**
 *  \brief Analyze pattern.
 */
template <class FileReader, class Scalar, class ResultScalar>
void SolutionStatic<FileReader, Scalar, ResultScalar>::analyze()
{
	for(const auto &it: (*this).elem_group_){
		const auto &p_elem = it.second;//!< Get element ptr.
		auto node_list = p_elem.get_node_list();
		auto et = p_elem.get_element_type();
		for(int i=0; i<p_elem.get_active_num_of_node(); i++){
			auto got = this->node_group_.find(node_list[i]);
			if(got!=this->node_group_.end()){
				got->second.activate(true);
				got->second.dof_init(et);
			}
		}
	}
	for(const auto &bc: (*this).bc_group_){
		auto got = this->node_group_.find(bc.get_id());
		if(got!=this->node_group_.end()){
			got->second.dof_apply(bc);
		}
	}
	int num{0};
	for(auto &it: this->node_group_)it.second.dof_accum(&num, DofType::NORMAL);
	
	fmt::print("Total Dimension:{}\n", num);
	
	for(const auto &it: this->elem_group_){
		const auto &p_elem = it.second;
		auto node_list = p_elem.get_node_list();
		auto num = p_elem.get_active_num_of_node();
		for(int i=0; i<num; i++){
			auto got_i = this->node_group_.find(node_list[i]);
			if(got_i!=this->node_group_.end()){
				auto dof_i = got_i->second.dof_list();
				for(int j=0; j<num; j++){
					auto got_j = this->node_group_.find(node_list[j]);
						if(got_j!=this->node_group_.end()){
							auto dof_j = got_j->second.dof_list();
							for(auto ia: dof_i){
								if(ia>=0){
									for(auto ib: dof_j){
										if(ib>=0)this->mat_pair_.append(ia, ib);
									}
								}
							}
						}
				}
			}
		}
	}
	this->mat_pair_.unique();
	fmt::print("Non Zeros: {}\n", this->mat_pair_.get_nnz());
	fmt::print("Dimension: {}\n", this->mat_pair_.get_dim());
};

/**
 *  \brief Assembly global matrix.
 */
template <class FileReader, class Scalar, class ResultScalar>
void SolutionStatic<FileReader, Scalar, ResultScalar>::assembly()
{
	for(auto &it: this->elem_group_){
		auto &p_elem = it.second;
		auto node_list = p_elem.get_node_list();
		auto mt = p_elem.get_material_id();
		auto st = p_elem.get_section_id();
		auto got_mt = this->matl_group_.find(mt);
		auto got_st = this->sect_group_.find(st);
		assert(got_mt!=this->matl_group_.end());
		assert(got_st!=this->sect_group_.end());
		Node<Scalar, ResultScalar> pt[node_list.size()];
		for(size_t i=0; i<node_list.size(); i++){
			auto got = this->node_group_.find(node_list[i]);
			if(got!=this->node_group_.end())pt[i] = got->second;
		}
		p_elem.form_matrix(pt, &(got_mt->second), &(got_st->second));
		auto p_stif = p_elem.get_stif();
		auto p_mass = p_elem.get_mass();
		auto p_tran = p_elem.get_tran();
		auto p_rhs = p_elem.get_rhs();
		p_stif = p_tran.transpose()*p_stif*p_tran;
		p_mass = p_tran.transpose()*p_mass*p_tran;
		p_rhs = p_tran.transpose()*p_rhs;
		auto nn = p_elem.get_active_num_of_node();
		auto ndof = p_elem.get_dofs_per_node();
		for(auto ia=0; ia<nn; ia++){
			auto va = pt[ia].dof_list();
			for(auto ja=0; ja<ndof; ja++){
				p_elem.set_element_dofs(va[ja]);
				if(va[ja]<0)continue;
				auto row_ = ia*ndof+ja;
				for(auto ib=0; ib<nn; ib++){
					auto vb = pt[ib].dof_list();
					for(auto jb=0; jb<ndof; jb++){
						if(vb[jb]<0)continue;
						auto col_ = ib*ndof+jb;
						this->mat_pair_.add_matrix_data(va[ja], vb[jb],
							p_stif(row_, col_), p_mass(row_, col_), p_rhs(row_));
					}
				}
			}
			
		}
	}
};

/**
 *  \brief Solve.
 */
template <class FileReader, class T, class U>
void SolutionStatic<FileReader, T, U>::solve()
{
	this->solver_->load(
		this->mat_pair_.get_stif_ptr(), this->mat_pair_.get_coord_ptr(),
		this->mat_pair_.get_nnz(), this->mat_pair_.get_dim());
	
	bool flag = this->solver_->solve(this->mat_pair_.get_rhs_ptr(),
		this->mat_pair_.get_dim());
	if(!flag){
		fmt::print("Solve Failed.\n");
	}
	else{
		fmt::print("Solve Success.\n");
	}
};

/**
 *  \brief Post-process.
 */
template <class FileReader, class T, class U>
void SolutionStatic<FileReader, T, U>::post_process()
{
	if(!this->solver_->get_info()){
		fmt::print("Solve Static Problem is Failed.\n");
		return;
	}
	auto sol = this->solver_->get_X();
	for(auto &it: this->elem_group_){
		auto &p_elem = it.second;
		auto va = p_elem.get_element_dofs();
		vecX_<U> x = vecX_<U>::Zero(va.size());
		for(int i=0; i<x.size(); i++)x(i) = va[i]<0 ? U(0): sol(i);
		p_elem.post_stress(x);
	}
};

/**
 *  \brief Write element matrix to MAT file.
 *  \param[in] fname file name.
 */
template <class FileReader, class Scalar, class ResultScalar>
void SolutionStatic<FileReader, Scalar, ResultScalar>::write2mat(const char* fname)
{};
}