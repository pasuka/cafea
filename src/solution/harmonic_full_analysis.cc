#include "cafea.h"

namespace cafea
{
using namespace std::complex_literals;
/**
 *  \brief Initilize variables.
 */
template <class FileReader, class Scalar, class ResultScalar>
void SolutionHarmonicFull<FileReader, Scalar, ResultScalar>::init()
{
	this->clear();
	if(!this->node_group_.empty())this->node_group_.clear();
	if(!this->elem_group_.empty())this->elem_group_.clear();
	if(!this->matl_group_.empty())this->matl_group_.clear();
	if(!this->sect_group_.empty())this->sect_group_.clear();
	if(!this->bc_group_.empty())this->bc_group_.clear();
	if(!this->load_group_.empty())this->load_group_.clear();
	this->mat_pair_.clear();
	fmt::print("This is harmonic init.\n");
	/*fmt::print("Damping size{}\n", this->damping_.size());
	fmt::print("Frequency size{}\n", this->freq_range_.size());
	fmt::print("rhs row {} col {}\n", this->rhs_cmplx_.rows(), this->rhs_cmplx_.cols());
	fmt::print("disp row{} col{}\n", this->disp_cmplx_.rows(), this->disp_cmplx_.cols());*/
};
/**
 *  \brief Clear member variables.
 */
template <class FileReader, class Scalar, class ResultScalar>
void SolutionHarmonicFull<FileReader, Scalar, ResultScalar>::clear()
{
	if(!this->node_group_.empty())this->node_group_.clear();
	if(!this->elem_group_.empty())this->elem_group_.clear();
	if(!this->matl_group_.empty())this->matl_group_.clear();
	if(!this->sect_group_.empty())this->sect_group_.clear();
	if(!this->bc_group_.empty())this->bc_group_.clear();
	if(!this->load_group_.empty())this->load_group_.clear();
	this->mat_pair_.clear();
	
	fmt::print("This is harmonic clear.\n");
	// fmt::print("Damping size{}\n", this->damping_.size());
	// fmt::print("Frequency size{}\n", this->freq_range_.size());
	// fmt::print("rhs row {} col {}\n", this->rhs_cmplx_.rows(), this->rhs_cmplx_.cols());
	// fmt::print("disp row{} col{}\n", this->disp_cmplx_.rows(), this->disp_cmplx_.cols());
	if(0<this->damping_.size())this->damping_.resize(0);
	if(0<this->freq_range_.size())this->freq_range_.resize(0);
	
	if(0<this->rhs_cmplx_.rows()&&0<this->rhs_cmplx_.cols())this->rhs_cmplx_.resize(0, 0);
	if(0<this->disp_cmplx_.rows()&&0<this->disp_cmplx_.cols())this->disp_cmplx_.resize(0, 0);
};
/**
 *  \brief Get model info.
 *  \return array that contain model statistics.
 */
template <class FileReader, class Scalar, class ResultScalar>
std::array<size_t, 5> SolutionHarmonicFull<FileReader, Scalar, ResultScalar>::get_info()const
{
	std::array<size_t, 5> param{this->node_group_.size(), this->elem_group_.size(),
		this->matl_group_.size(), this->sect_group_.size(), this->bc_group_.size()};	
	return param;
};
/**
 *  \brief Check model.
 */
template <class FileReader, class Scalar, class ResultScalar>
void SolutionHarmonicFull<FileReader, Scalar, ResultScalar>::check()
{
	if(this->node_group_.empty())fmt::print("None node in model\n");
	if(this->elem_group_.empty())fmt::print("None element in model\n");
	if(this->matl_group_.empty())fmt::print("None material in model\n");
	if(this->sect_group_.empty())fmt::print("None section in model\n");
	if(this->bc_group_.empty())fmt::print("None boundary in model\n");
	fmt::print("Model has been checked!\n");
};

/**
 *  \brief Analyze pattern.
 */
template <class FileReader, class Scalar, class ResultScalar>
void SolutionHarmonicFull<FileReader, Scalar, ResultScalar>::analyze()
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
	
	this->rhs_cmplx_.resize(this->mat_pair_.get_dim(), this->load_group_.size());
	this->disp_cmplx_.resize(this->mat_pair_.get_dim(), this->load_group_.size());
};

/**
 *  \brief Assembly global matrix.
 */
template <class FileReader, class Scalar, class ResultScalar>
void SolutionHarmonicFull<FileReader, Scalar, ResultScalar>::assembly()
{
	// fmt::print("Assembly matrix in harmonic full analysis.\n");
	bool lumped{false};
	if(this->mass_type_==MassType::LUMPED)lumped = true;
	
	// 
	std::vector<LoadCell<Scalar>> pres;
	if(this->has_pressure_){
		for(auto p: this->load_group_){
			auto y = p.get_load_by_type(LoadType::PRES);
			if(!y.empty())pres.push_back(y[0]);
		}
	}
	for(auto &it: this->elem_group_){
		auto &p_elem = it.second;
		p_elem.set_lumped_mass(lumped);
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
		// p_elem.template form_matrix<Scalar>(pt, &(got_mt->second), &(got_st->second));
		p_elem.template form_matrix<Scalar>(pt, &(got_mt->second), &(got_st->second), pres);
		auto p_stif = p_elem.get_stif();
		auto p_mass = p_elem.get_mass();
		auto p_tran = p_elem.get_tran();
		auto p_rhs = p_elem.get_rhs();
		auto p_rhs_cmplx = p_elem.get_rhs_cmplx();
		// if(p_elem.get_element_type_id()==16)std::cout << p_rhs_cmplx << "\n";
		// if(p_elem.get_element_type_id()==18)std::cout << p_rhs_cmplx << "\n";
		p_stif = p_tran.transpose()*p_stif*p_tran;
		p_mass = p_tran.transpose()*p_mass*p_tran;
		p_rhs = p_tran.transpose()*p_rhs;
		p_rhs_cmplx = p_tran.transpose()*p_rhs_cmplx;
		
		auto nn = p_elem.get_active_num_of_node();
		auto ndof = p_elem.get_dofs_per_node();
		for(auto ia=0; ia<nn; ia++){
			auto va = pt[ia].dof_list();
			for(auto ja=0; ja<ndof; ja++){
				p_elem.set_element_dofs(va[ja]);
				if(va[ja]<0)continue;
				auto row_ = ia*ndof+ja;
				// this->mat_pair_.add_rhs_data(va[ja], p_rhs(row_));
				this->rhs_cmplx_.row(va[ja]) += p_rhs_cmplx.row(row_);
				for(auto ib=0; ib<nn; ib++){
					auto vb = pt[ib].dof_list();
					for(auto jb=0; jb<ndof; jb++){
						if(vb[jb]<0)continue;
						auto col_ = ib*ndof+jb;
						this->mat_pair_.add_matrix_data(va[ja], vb[jb],
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
template <class FileReader, class T, class U>
void SolutionHarmonicFull<FileReader, T, U>::solve()
{
	Eigen::SparseLU<Eigen::SparseMatrix<COMPLEX<U>>, Eigen::COLAMDOrdering<int>> solver;
	
	auto dim = this->mat_pair_.get_dim();
	auto nnz = this->mat_pair_.get_nnz();
	
	Eigen::SparseMatrix<COMPLEX<U>> mat_K(dim, dim), mat_M(dim, dim);
	mat_K.reserve(nnz);
	mat_M.reserve(nnz);
	
	std::vector<Eigen::Triplet<COMPLEX<U>>> triList(nnz);
	fmt::print("Form Global stiffness matrix.\n");
	for(int i=0; i<nnz; i++){
		Eigen::Triplet<COMPLEX<U>> tmp(this->mat_pair_.get_coord_ptr()[i].row,
			this->mat_pair_.get_coord_ptr()[i].col,
			this->mat_pair_.get_stif_ptr()[i]*(1.+this->damping_[0]*1.0i));
		triList.push_back(std::move(tmp));
	}
	mat_K.setFromTriplets(triList.begin(), triList.end());
	triList.clear();
	fmt::print("Form Global mass matrix.\n");
	for(int i=0; i<nnz; i++){
		Eigen::Triplet<COMPLEX<U>> tmp(this->mat_pair_.get_coord_ptr()[i].row,
			this->mat_pair_.get_coord_ptr()[i].col,
			this->mat_pair_.get_mass_ptr()[i]*(1.+0.0i));
		triList.push_back(std::move(tmp));
	}
	mat_M.setFromTriplets(triList.begin(), triList.end());
	triList.clear();
	fmt::print("Begin iteration.\n");
	for(int i=0; i<this->freq_range_.size(); i++){
		fmt::print("Iter No. {}\tFrequency: {}\n", i+1, this->freq_range_(i));
		auto omega = 2.*PI<U>()*this->freq_range_(i);
		auto omega2 = omega*omega;
		Eigen::SparseMatrix<COMPLEX<U>> mat_a = mat_K - (omega2+0.0i)*mat_M;
		solver.analyzePattern(mat_a);
		solver.factorize(mat_a);
		vecX_<COMPLEX<U>> rhs = this->rhs_cmplx_.col(i);
		auto force = this->load_group_[i].get_load_by_type(LoadType::FORCE);
		if(!force.empty()){
			for(auto const &x: force){
				auto got = this->node_group_.find(x.get_id());
				auto dof_label = static_cast<int>(x.get_dof_label());
				COMPLEX<U> force_val = x.val_cmplx_;
				if(got!=this->node_group_.end()){
					auto &pt = got->second;
					auto va = pt.dof_list();
					if(dof_label<va.size()){
						if(0<va[dof_label])rhs(va[dof_label]) += force_val;
					}
				}	
			}
		}
		auto disp = this->load_group_[i].get_load_by_type(LoadType::DISP);
		if(!disp.empty()){
			for(auto const &x: disp){
				auto got = this->node_group_.find(x.get_id());
				int dof_label = static_cast<int>(x.get_dof_label());
				COMPLEX<U> disp_val = x.val_cmplx_;
				if(got!=this->node_group_.end()){
					auto &pt = got->second;
					auto va = pt.dof_list();
					if(dof_label<va.size()){
						auto index = va[dof_label];
						if(0<index){
							mat_a.coeff(index, index) *= std::numeric_limits<U>::max();
							rhs(index) += disp_val*mat_a.coeff(index, index);
						}
					}
				}
			}
		}
		this->disp_cmplx_.col(i) = solver.solve(rhs);
		if(solver.info()!=Eigen::ComputationInfo::Success){
			fmt::print("Solve Failed!\n");
		}
		else{
			fmt::print("Solve Success!\n");
		}
	}
	for(auto &it: this->node_group_){
		auto &p_node = it.second;
		auto num_step = this->freq_range_.size();
		if(p_node.is_activated()){
			p_node.init_result(SolutionType::HARMONIC_FULL, num_step);
			auto tmp = p_node.dof_list();
			matrix_<COMPLEX<U>> x = matrix_<COMPLEX<U>>::Zero(tmp.size(), num_step) ;
			for(int i=0; i<tmp.size(); i++){
				if(tmp[i]>=0)x.row(i) = this->disp_cmplx_.row(tmp[i]);
			}	
			p_node.template set_result<COMPLEX<U>>(SolutionType::HARMONIC_FULL, LoadType::DISP, -1, x);
		}
		fmt::print("Set displace result finish.\n");
	}
	fmt::print("Harmonic full solve.\n");
};

/**
 *
 */
template <class FileReader, class T, class U>
void SolutionHarmonicFull<FileReader, T, U>::load(const char* fn)
{
	int flag = this->file_parser_.load_model(fn);
	if(0==flag){
		fmt::print("load model success!\n");
		auto info = this->file_parser_.print_info();
		assert(0<info["node"]);
		assert(0<info["element"]);
		assert(0<info["material"]);
		assert(0<info["section"]);
		assert(0<info["solution"]);
		assert(0<info["load"]);
		
		auto p_node = this->file_parser_.get_node_ptr(); 
		auto p_elem = this->file_parser_.get_element_ptr();
		auto p_matl = this->file_parser_.get_material_ptr();
		auto p_sect = this->file_parser_.get_section_ptr();
		auto p_bc = this->file_parser_.get_boundary_ptr();
		auto p_solu = this->file_parser_.get_solution_ptr();
		auto p_load = this->file_parser_.get_load_ptr();
		
		wrapper_::AdapterF2Cpp<T, U> f2cpp;
		
		for(int i=0; i<info["node"]; i++, p_node++){
			auto got = this->node_group_.find(p_node->id_);
			if(got==this->node_group_.end()){
				this->node_group_[p_node->id_] = f2cpp.bcy2node(p_node);
			}
			else{
				fmt::print("Duplicated node id:{}\n", p_node->id_);
			}
		}
		
		for(int i=0; i<info["element"]; i++, p_elem++){
			auto got = this->elem_group_.find(p_elem->id_);
			if(got==this->elem_group_.end()){
				this->elem_group_[p_elem->id_] = f2cpp.bcy2elem(p_elem);
			}
			else{
				fmt::print("Duplicated element id:{}\n", p_elem->id_);
			}
		}
			
		for(int i=0; i<info["material"]; i++, p_matl++){
			auto got = this->matl_group_.find(p_matl->id_);
			if(got==this->matl_group_.end()){
				this->matl_group_[p_matl->id_] = f2cpp.bcy2matl(p_matl);
			}
			else{
				fmt::print("Duplicated material id:{}\n", p_matl->id_);
			}
		}
		
		for(int i=0; i<info["section"]; i++, p_sect++){
			auto got = this->sect_group_.find(p_sect->id_);
			if(got==this->sect_group_.end()){
				auto st = f2cpp.bcy2sect(p_sect);
				st.set_sect_prop(SectionProp::PRESIN, T(1));
				this->sect_group_[p_sect->id_] = st;
				
			}
			else{
				fmt::print("Duplicated section id:{}\n", p_sect->id_);
			}
		}
		
		for(int i=0; i<info["boundary"]; i++, p_bc++){
			this->bc_group_.push_back(f2cpp.bcy2bndy(p_bc));
		}
		
		fmt::print("analysis type: {}\n", p_solu->antype_);
		fmt::print("Step of analysis: {}\n", p_solu->num_step_);
		fmt::print("Damp coeff: {}\n", p_solu->damp_[0]);
		this->damping_ = vecX_<U>::Zero(2);
		this->freq_range_ = vecX_<U>::Zero(p_solu->num_step_);
		this->damping_ << p_solu->damp_[0], p_solu->damp_[1];
		
		for(int i=0; i<info["load"]; i++, p_load++){
			LoadSet<T> tmp_frame = {i+1, LoadDomain::FREQ, T(0)};
			for(int j=0; j<p_load->size(); j++){
				auto &it = p_load->data()[j];
				if(0==j){
					this->freq_range_(i) = it.range_;
					tmp_frame.set_value(it.range_);
				}
				DofLabel label = DofLabel::UNKNOWN;
				
				switch(it.dof_){
				case 1: label = DofLabel::UX; break;
				case 2: label = DofLabel::UY; break;
				case 3: label = DofLabel::UZ; break;
				case 4: label = DofLabel::URX; break;
				case 5: label = DofLabel::URY; break;
				case 6: label = DofLabel::URZ; break;
				default: label = DofLabel::UNKNOWN; // fmt::print("Unsupported dof label: {}\n", it.dof_);
				}
				
				switch(it.type_){
				case 5:
					// fmt::print("Add pressure:\n");
					this->set_parameter(SolutionOption::PRESSURE_INTERNAL, true);
					tmp_frame.add_load(it.id_, LoadType::PRES, DofLabel::ALL, it.val_[0], it.val_[1]);
					break;
				case 1:
					// fmt::print("Add force:\n");
					tmp_frame.add_load(it.id_, LoadType::FORCE, label, it.val_[0], it.val_[1]);
					break;
				case 2:
					// fmt::print("Add disp:\n");
					tmp_frame.add_load(it.id_, LoadType::DISP, label, it.val_[0], it.val_[1]);
					break;
				default: fmt::print("Unsupported load type: {}\n", it.type_);
				}
			}
			this->load_group_.push_back(tmp_frame);
		}
	}
};
/**
 *  \brief Post-process.
 */
template <class FileReader, class T, class U>
void SolutionHarmonicFull<FileReader, T, U>::post_process()
{
	fmt::print("Empty\n");
	auto num_step = this->freq_range_.size();
	for(auto &it: this->elem_group_){
		auto &p_elem = it.second;
		auto va = p_elem.get_element_dofs();
		matrix_<COMPLEX<U>> x = matrix_<COMPLEX<U>>::Zero(va.size(), num_step);
		for(int i=0; i<va.size(); i++){
			if(va[i]>=0)x.row(i) = this->disp_cmplx_.row(va[i]);
		}
		// std::cout << "Element displacement: " << x.col(0) << "\n";
		p_elem.template post_stress<COMPLEX<U>>(x);
		matrix_<COMPLEX<U>> stress = p_elem.template get_result<COMPLEX<U>>();
		if(1>stress.rows())continue;
		// fmt::print("Stress Row:{} Col:{}\n", stress.rows(), stress.cols());
		auto node_list = p_elem.get_node_list();
		auto nn = p_elem.get_active_num_of_node();
		auto iter{0};
		for(auto &i: node_list){
			auto &p_node = this->node_group_[i];
			if(p_node.is_activated()){
				for(int j=0; j<num_step; j++){
					// fmt::print("Col index:{}\n", j*int(nn)+iter);
					matrix_<COMPLEX<U>> yy = stress.block(43, j*int(nn)+iter, 8, 1);
					p_node.template set_result<COMPLEX<U>>(SolutionType::HARMONIC_FULL, LoadType::STRESS, j, yy);
				}
				iter++;
			}
		}
	}
};

/**
 *  \brief Write element matrix to MAT file.
 *  \param[in] fname file name.
 */
template <class FileReader, class Scalar, class ResultScalar>
void SolutionHarmonicFull<FileReader, Scalar, ResultScalar>::write2mat(const char* fname, bool is_ver73)
{
	fmt::print("write to MAT files.\n");
	mat_t *matfp;
	if(is_ver73){
		matfp = Mat_CreateVer(fname, NULL, MAT_FT_MAT73);
	}
	else{
		matfp = Mat_CreateVer(fname, NULL, MAT_FT_MAT5);
	}
	
	if(NULL==matfp){
		fmt::print("Error creating MAT file\n");
		return;
	}
	const size_t nfields{10};
	const char *fieldnames[nfields] = {"id", "etype", "mtype", "stype", "nodes",
		"stif", "mass", "tran", "rhs", "result"};
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
		
		matrix_<COMPLEX<ResultScalar>> rhs = it.second.template get_rhs<COMPLEX<ResultScalar>>();
		// matrix_<COMPLEX<ResultScalar>> rhs = it.second.get_rhs_cmplx();
		size_t dim_rhs[2]={1, 1};
		if(1<rhs.rows())dim_rhs[0] = rhs.rows();
		if(1<rhs.cols())dim_rhs[1] = rhs.cols();
		matrix_<ResultScalar> rhs_re = rhs.real();
		matrix_<ResultScalar> rhs_im = rhs.imag();
		mat_complex_split_t c_rhs{rhs_re.data(), rhs_im.data()};
		matvar[8] = Mat_VarCreate(fieldnames[8], MAT_C_DOUBLE, MAT_T_DOUBLE, 2,
			dim_rhs, &c_rhs, MAT_F_COMPLEX);
		
		matrix_<COMPLEX<ResultScalar>> stress = it.second.template get_result<COMPLEX<ResultScalar>>();
		size_t dim_stress[2]={1, 1};
		if(1>stress.rows()&&1>stress.cols()){
			double tmp_val = double(-1);
			matvar[9] = Mat_VarCreate(fieldnames[9], MAT_C_DOUBLE, MAT_T_DOUBLE, 2, dim1x1, &tmp_val, 0);
		}
		else{
			if(1<stress.rows())dim_stress[0] = stress.rows();
			if(1<stress.cols())dim_stress[1] = stress.cols();
			matrix_<ResultScalar> stress_re = stress.real();
			matrix_<ResultScalar> stress_im = stress.imag();
			mat_complex_split_t c_stress{stress_re.data(), stress_im.data()};
			matvar[9] = Mat_VarCreate(fieldnames[9], MAT_C_DOUBLE, MAT_T_DOUBLE, 2,
				dim_stress, &c_stress, MAT_F_COMPLEX);
		}
		for(size_t i=0; i<nfields; i++){
			Mat_VarSetStructFieldByName(elem_list, fieldnames[i], num, matvar[i]);
		}
		num++;
	}
	Mat_VarWrite(matfp, elem_list, MAT_COMPRESSION_ZLIB);
	Mat_VarFree(elem_list);
	
	const size_t nfields2{4};
	const char *fieldnames2[nfields2] = {"id", "csys", "xyz", "result"};
	size_t node_dims[2] = {this->node_group_.size(), 1};
	matvar_t *node_list = Mat_VarCreateStruct("node", 2, node_dims, fieldnames2, nfields2);
	size_t dim3x1[2] = {3, 1};
	double xyz[3] = {0., 0., 0.};
	num = 0;
	for(const auto &it: this->node_group_){
		auto &p_node = it.second;
		matvar_t *matvar[nfields2];
		val[0] = double(p_node.get_id());
		val[1] = double(0);
		for(size_t i=0; i<2; i++){
			matvar[i] = Mat_VarCreate(fieldnames2[i], MAT_C_DOUBLE, MAT_T_DOUBLE, 2, dim1x1, &val[i], 0);
		}
		xyz[0] = double(p_node.get_x());
		xyz[1] = double(p_node.get_y());
		xyz[2] = double(p_node.get_z());
		matvar[2] = Mat_VarCreate(fieldnames2[2], MAT_C_DOUBLE, MAT_T_DOUBLE, 2, dim3x1, xyz, 0);
		if(p_node.is_activated()){
			// matrix_<COMPLEX<ResultScalar>> rst = p_node.template get_result<COMPLEX<ResultScalar>>(SolutionType::HARMONIC_FULL, LoadType::DISP, -1);
			matrix_<COMPLEX<ResultScalar>> rst = p_node.template get_result<COMPLEX<ResultScalar>>(SolutionType::HARMONIC_FULL, LoadType::STRESS, -1);
			size_t sz[2] = {1, 1};
			if(1<rst.rows())sz[0] = rst.rows();
			if(1<rst.cols())sz[1] = rst.cols();
			matrix_<ResultScalar> rst_re = rst.real();
			matrix_<ResultScalar> rst_im = rst.imag();
			mat_complex_split_t cs{rst_re.data(), rst_im.data()};
			matvar[3] = Mat_VarCreate(fieldnames2[3], MAT_C_DOUBLE, MAT_T_DOUBLE, 2, sz, &cs, MAT_F_COMPLEX);
		}
		else{
			val[1] = double(-1);
			matvar[3] = Mat_VarCreate(fieldnames2[3], MAT_C_DOUBLE, MAT_T_DOUBLE, 2, dim1x1, &val[1], 0);
		}
		for(auto i=0; i<nfields2; i++){
			Mat_VarSetStructFieldByName(node_list, fieldnames2[i], num, matvar[i]);
		}
		num++;
	}
	Mat_VarWrite(matfp, node_list, MAT_COMPRESSION_ZLIB);
	Mat_VarFree(node_list);
	
	{
		matvar_t *freq;
		size_t dims[2] = {1, 1};
		if(1>this->freq_range_.size()){
			double val{-1.0};
			freq = Mat_VarCreate("freq", MAT_C_DOUBLE, MAT_T_DOUBLE, 2, dims, &val, 0);
		}
		else{
			dims[0] = this->freq_range_.size();
			freq = Mat_VarCreate("freq", MAT_C_DOUBLE, MAT_T_DOUBLE, 2, dims, this->freq_range_.data(), 0);
		}
		Mat_VarWrite(matfp, freq, MAT_COMPRESSION_ZLIB);
		Mat_VarFree(freq);
	}
	
	
	
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
 *
 */
template <class FP, class T, class U>
matrix_<U> SolutionHarmonicFull<FP, T, U>::get_node_result(int node_id, LoadType res_tp, int res_span)const
{
	/*
	assert(0<node_id);
	auto got = this->node_group_.find(node_id);
	matrix_<U> res;
	if(got!=this->node_group_.end()){
		res = (got->second).get_result(SolutionType::STATIC, res_tp, 0);
	}
	else{
		res = matrix_<U>::Zero(1, 1);
	}
	return res;
	*/
	matrix_<U> res = matrix_<U>::Zero(1, 1);
	return res;
};

/**
 *  \brief Set solution parameter.
 */
template <class FP, class T, class U>
void SolutionHarmonicFull<FP, T, U>::set_parameter(SolutionOption chk, init_list_<U> val)
{
	fmt::print("Modal parameter in numeric.\n");
}

/**
 * \brief Set solution parameter.
 */
template <class FP, class T, class U>
void SolutionHarmonicFull<FP, T, U>::set_parameter(SolutionOption chk, bool val)
{
	// fmt::print("Modal parameter in boolean.\n");
	switch(chk){
	case SolutionOption::LUMPED_MASS:
		this->set_mass_lumped(val);
		break;
	case SolutionOption::PRESSURE_INTERNAL:
		this->has_pressure_ = val;
		break;
	default: fmt::print("Unsupport boolean parameter in modal analyze.\n");
	}
}
}