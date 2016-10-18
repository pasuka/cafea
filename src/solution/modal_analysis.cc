#include "cafea.h"

namespace cafea
{
/**
 *  \brief Initilize variables.
 */
template <class FileReader, class Scalar, class ResultScalar>
void SolutionModal<FileReader, Scalar, ResultScalar>::init()
{
	this->clear();
	std::unique_ptr<EigenSolver<ResultScalar>> p(new EigenSolver<ResultScalar>);
	this->solver_ = std::move(p);
};
/**
 *  \brief Clear member variables.
 */
template <class FileReader, class Scalar, class ResultScalar>
void SolutionModal<FileReader, Scalar, ResultScalar>::clear()
{
	if(!this->node_group_.empty())this->node_group_.clear();
	if(!this->elem_group_.empty())this->elem_group_.clear();
	if(!this->matl_group_.empty())this->matl_group_.clear();
	if(!this->sect_group_.empty())this->sect_group_.clear();
	if(!this->bc_group_.empty())this->bc_group_.clear();
	this->mat_pair_.clear();
	(*this).mode_shape_.resize(0, 0);
	(*this).natural_freq_.resize(0, 0);
	if(this->solver_)this->solver_.reset(nullptr);
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
	if(this->node_group_.empty())fmt::print("None node in model\n");
	if(this->elem_group_.empty())fmt::print("None element in model\n");
	if(this->matl_group_.empty())fmt::print("None material in model\n");
	if(this->sect_group_.empty())fmt::print("None section in model\n");
	if(this->bc_group_.empty())fmt::print("None boundary in model\n");
	fmt::print("Model has been checked!\n");
};

/**
 *  \brief Load input file.
 *  \param[in] fn file path.
 */
/* template <class FileReader, class Scalar, class ResultScalar>
void SolutionModal<FileReader, Scalar, ResultScalar>::load(const char* fn)
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
}; */

/**
 *  \brief Analyze pattern.
 */
template <class FileReader, class Scalar, class ResultScalar>
void SolutionModal<FileReader, Scalar, ResultScalar>::analyze()
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
void SolutionModal<FileReader, Scalar, ResultScalar>::assembly()
{
	bool lumped{false};
	if(this->mass_type_==MassType::LUMPED)lumped = true;
	for(auto &it: this->elem_group_){
		auto node_list = it.second.get_node_list();
		it.second.set_lumped_mass(lumped);
		auto mt = it.second.get_material_id();
		auto st = it.second.get_section_id();
		auto got_mt = this->matl_group_.find(mt);
		auto got_st = this->sect_group_.find(st);
		assert(got_mt!=this->matl_group_.end());
		assert(got_st!=this->sect_group_.end());
		Node<Scalar, ResultScalar> pt[node_list.size()];
		for(size_t i=0; i<node_list.size(); i++){
			auto got = this->node_group_.find(node_list[i]);
			if(got!=this->node_group_.end())pt[i] = got->second;
		}
		it.second.form_matrix(pt, &(got_mt->second), &(got_st->second));
		auto p_stif = it.second.get_stif();
		auto p_mass = it.second.get_mass();
		auto p_tran = it.second.get_tran();
		p_stif = p_tran.transpose()*p_stif*p_tran;
		p_mass = p_tran.transpose()*p_mass*p_tran;
		auto nn = it.second.get_active_num_of_node();
		auto ndof = it.second.get_dofs_per_node();
		for(size_t ia=0; ia<nn; ia++){
			auto va = pt[ia].dof_list();
			for(auto ja=0; ja<ndof; ja++){
				if(va[ja]<0)continue;
				for(size_t ib=0; ib<nn; ib++){
					auto vb = pt[ib].dof_list();
					for(auto jb=0; jb<ndof; jb++){
						if(vb[jb]<0)continue;
						auto row_ = ia*ndof+ja;
						auto col_ = ib*ndof+jb;
						this->mat_pair_.add_matrix_data_KM(va[ja], vb[jb],
							p_stif(row_, col_), p_mass(row_, col_));
					}
				}
			}
			
		}
	}
}

/**
 *  \brief Solve.
 */
template <class FileReader, class Scalar, class ResultScalar>
void SolutionModal<FileReader, Scalar, ResultScalar>::solve()
{
	matrix_<ResultScalar> val, shp;
	auto dim = this->mat_pair_.get_dim();
	auto nnz = this->mat_pair_.get_nnz();
	auto eps = EPS<ResultScalar>();
	auto hz2rad = [=](ResultScalar x) {return ResultScalar(pow(x*2.0*PI<>(), 2.));};
	
	this->solver_->load(this->mat_pair_.get_stif_ptr(), this->mat_pair_.get_mass_ptr(),
		this->mat_pair_.get_coord_ptr(), nnz, dim);
	
	if(0<this->freq_num_ && dim>=this->freq_num_){
		std::tie(val, shp) = this->solver_->subspace(this->freq_num_);
	}
	else if(eps<this->freq_range_[1]){
		std::tie(val, shp) = this->solver_->subspace(hz2rad(this->freq_range_[1]), hz2rad(this->freq_range_[0]));
	}
	else{
		std::tie(val, shp) = this->solver_->subspace(1);
	}
	if(val.rows()){
		for(int i=0; i<val.rows(); i++)val(i, 0) = sqrt(val(i, 0))/2./PI<>();
		this->natural_freq_ = val;
		this->mode_shape_ = shp;
		fmt::print("Solve finished.\n");
	}
	// ResultScalar fspan[2] = {ResultScalar(0), ResultScalar(0)};
	// fspan[0] = PI<ResultScalar>()*PI<ResultScalar>()*ResultScalar(1e10);
	// this->solver_.subspace(fspan[0], fspan[1]);
	// matrix_<ResultScalar> val, shp;
	// ResultScalar fspan[2] = {ResultScalar(0), ResultScalar(0)};
	// fspan[0] = pow(PI<ResultScalar>()*ResultScalar(2000), 2.0);
	// std::tie(val, shp) = this->solver_->subspace(fspan[0], fspan[1]);
	// std::tie(val, shp) = this->solver_->subspace(15);
	// fmt::print("\n");
	// for(int i=0; i<val.rows(); i++)fmt::print("No.{}\tFreq.:{}Hz Error:{}\n", i+1, sqrt(val(i, 0))/2/PI<ResultScalar>(), val(i, 1));
	// auto nn = this->solver_.sturm_check(1e30);
};

/**
 *  \brief Post-process.
 */
template <class FileReader, class Scalar, class ResultScalar>
void SolutionModal<FileReader, Scalar, ResultScalar>::post_process()
{
	fmt::print("Empty\n");
};

/**
 *  \brief Write element matrix to MAT file.
 *  \param[in] fname file name.
 */
template <class FileReader, class Scalar, class ResultScalar>
void SolutionModal<FileReader, Scalar, ResultScalar>::write2mat(const char* fname)
{
	mat_t *matfp;
	matfp = Mat_CreateVer(fname, NULL, MAT_FT_MAT73);
	if(NULL==matfp){
		fmt::print("Error creating MAT file\n");
		return;
	}
	const size_t nfields{8};
	const char *fieldnames[nfields] = {"id", "etype", "mtype", "stype", "nodes",
		"stif", "mass", "tran"};
	size_t elem_dims[2] = {this->elem_group_.size(), 1};
	matvar_t *elem_list = Mat_VarCreateStruct("elem", 2, elem_dims, fieldnames, nfields);
	
	size_t num{0}, dim_vec[2] = {1, 1}, dim1x1[2] = {1, 1};
	double val[4];
	for(const auto &it: this->elem_group_){
		matvar_t *matvar[nfields];
		val[0] = double(it.second.get_element_id());
		val[1] = double(it.second.get_element_type_id());
		val[2] = double(it.second.get_material_id());
		val[3] = double(it.second.get_section_id());
		for(size_t i=0; i<4; i++){
			matvar[i] = Mat_VarCreate(fieldnames[i], MAT_C_DOUBLE, MAT_T_DOUBLE,
				2, dim1x1, &val[i], 0);
		}
		auto nodes = it.second.get_node_list();
		dim_vec[0] = nodes.size();
		double nodes_d[dim_vec[0]];
		for(size_t i=0; i<dim_vec[0]; i++)nodes_d[i] = double(nodes[i]);
		matvar[4] = Mat_VarCreate(fieldnames[4], MAT_C_DOUBLE, MAT_T_DOUBLE, 2,
			dim_vec, nodes_d, 0);
		auto sz  = it.second.get_matrix_shape();
		matvar[5] = Mat_VarCreate(fieldnames[5], MAT_C_DOUBLE, MAT_T_DOUBLE, 2,
			sz.data(), const_cast<ResultScalar*>(it.second.get_stif_ptr()), MAT_F_DONT_COPY_DATA);
		matvar[6] = Mat_VarCreate(fieldnames[6], MAT_C_DOUBLE, MAT_T_DOUBLE, 2,
			sz.data(), const_cast<ResultScalar*>(it.second.get_mass_ptr()), MAT_F_DONT_COPY_DATA);
		matvar[7] = Mat_VarCreate(fieldnames[7], MAT_C_DOUBLE, MAT_T_DOUBLE, 2,
			sz.data(), const_cast<ResultScalar*>(it.second.get_tran_ptr()), MAT_F_DONT_COPY_DATA);
		for(size_t i=0; i<nfields; i++){
			Mat_VarSetStructFieldByName(elem_list, fieldnames[i], num, matvar[i]);
		}
		num++;
	}
	Mat_VarWrite(matfp, elem_list, MAT_COMPRESSION_ZLIB);
	Mat_VarFree(elem_list);
	auto coord = this->mat_pair_.get_coord_ptr();
	auto pk = this->mat_pair_.get_stif_ptr();
	auto pm = this->mat_pair_.get_mass_ptr();
	size_t nn = this->mat_pair_.get_nnz();
	size_t dims[2] = {nn, 4};
	double xy[nn*4];
	for(size_t i=0; i<nn; i++){
		xy[i] = 1.0 + coord[i].row;
		xy[nn+i] = 1.0 + coord[i].col;
		xy[nn*2+i] = pk[i];
		xy[nn*3+i] = pm[i];
	}
	matvar_t *ind = Mat_VarCreate("KM", MAT_C_DOUBLE, MAT_T_DOUBLE, 2, dims, xy, MAT_F_DONT_COPY_DATA);
	Mat_VarWrite(matfp, ind, MAT_COMPRESSION_ZLIB);
	Mat_VarFree(ind);
	auto global_stif = this->mat_pair_.get_stif_mat();
	dim_vec[0] = dim_vec[1] = this->mat_pair_.get_dim();
	// fmt::print("Global matrix dimension:{}x{}\n", dim_vec[0], dim_vec[1]);
	matvar_t *gk = Mat_VarCreate("stif", MAT_C_SPARSE, MAT_T_DOUBLE, 2, dim_vec,
		global_stif.get(), MAT_F_DONT_COPY_DATA);
	Mat_VarWrite(matfp, gk, MAT_COMPRESSION_ZLIB);
	Mat_VarFree(gk);
	global_stif.reset(nullptr);
	auto global_mass = this->mat_pair_.get_mass_mat();
	matvar_t *gm = Mat_VarCreate("mass", MAT_C_SPARSE, MAT_T_DOUBLE, 2, dim_vec,
		global_mass.get(), MAT_F_DONT_COPY_DATA);
	Mat_VarWrite(matfp, gm, MAT_COMPRESSION_ZLIB);
	Mat_VarFree(gm);
	global_mass.reset(nullptr);
	Mat_Close(matfp);
	return;
};

/**
 *  \brief Set solution parameter.
 */
template <class FP, class T, class U>
void SolutionModal<FP, T, U>::set_parameter(SolutionOption chk, init_list_<U> val)
{
	// fmt::print("Modal parameter in numeric.\n");
	int flag{0};
	switch(chk){
	case SolutionOption::MODAL_NUMBER:
		for(auto p: val)this->freq_num_ = int(p);
		this->freq_range_[0] = U(0);
		this->freq_range_[1] = U(-1);
		break;
	case SolutionOption::MODAL_FREQ_RANGE:
		this->freq_num_ = -1;
		for(auto p: val){
			if(1>=flag){
				this->freq_range_[flag] = p;
			}
			flag++;
		}
		break;
	case SolutionOption::PRESSURE_INTERNAL:
		for(auto p: val){
			for(auto &it: this->sect_group_){
				it.second.set_sect_prop(SectionProp::PRESIN, T(p));
			}
		}
		break;
	default: fmt::print("Unsupport numeric parameter in modal analyze");
	}
	
}

/**
 * \brief Set solution parameter.
 */
template <class FP, class T, class U>
void SolutionModal<FP, T, U>::set_parameter(SolutionOption chk, bool val)
{
	// fmt::print("Modal parameter in boolean.\n");
	switch(chk){
	case SolutionOption::LUMPED_MASS:
		this->set_mass_lumped(val);
		break;
	default: fmt::print("Unsupport boolean parameter in modal analyze.\n");
	}
}
}