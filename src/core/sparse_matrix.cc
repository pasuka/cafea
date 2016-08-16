#include "cafea.h"

namespace cafea
{
/**
 *  \brief Compare index pair by row.
 *  \param[in] a index pair.
 *  \param[in] b index pair.
 *  \return boolean value.
 */
bool sort_by_row(const SparseCell &a, const SparseCell &b)
{
	if(a.row==b.row){
		return (a.col<b.col);
	}
	else{
		return (a.row<b.row);
	}
};
/**
 *  \brief Compare index pair by column.
 *  \param[in] a index pair.
 *  \param[in] b index pair.
 *  \return boolean value.
 */
bool sort_by_col(const SparseCell &a, const SparseCell &b)
{
	if(a.col==b.col){
		return (a.row<b.row);
	}
	else{
		return (a.col<b.col);
	}
};
/**
 *  \brief Compare index pair equal.
 *  \param[in] a index pair.
 *  \param[in] b index pair.
 *  \return boolean value.
 */
bool compare_pair(const SparseCell &a, const SparseCell &b)
{
	return (a.row==b.row)&&(a.col==b.col);
};
/**
 *  \brief Remove duplicated index pair.
 *  \param[in] sf sparse format.
 */
template <class T>
void SparseMat<T>::unique(SpFmt sf)
{
	auto func = std::bind(sort_by_col, std::placeholders::_1, std::placeholders::_2);
	auto func_eq = [](const SparseCell &a, const SparseCell &b){return a.col==b.col;};
	if(sf==SpFmt::CSR){
		auto func = std::bind(sort_by_row, std::placeholders::_1, std::placeholders::_2);
		auto func_eq = [](const SparseCell &a, const SparseCell &b){return a.row==b.row;};
	}
	
	std::sort(this->row_col_.begin(), this->row_col_.end(), func);
	auto extr = std::unique(this->row_col_.begin(), this->row_col_.end(), compare_pair);
	
	this->row_col_.erase(extr, this->row_col_.end());
	this->nnz_ = this->row_col_.size();
	this->stif_.resize(this->nnz_, T(.0));
	this->mass_.resize(this->nnz_, T(.0));

	size_t iter{0};
	this->aux_.push_back(iter++);
	
	for(size_t i=1; i<this->nnz_; i++){
		if(func_eq(this->row_col_[i], this->row_col_[i-1])){
			iter++;
		}
		else{
			this->aux_.push_back(iter);
			iter = 1;
		}
	}
	this->aux_.push_back(iter);
	for(size_t i=1; i<this->aux_.size(); i++)this->aux_[i] += this->aux_[i-1];
	this->dim_ = this->aux_.size()-1;
	this->rhs_.resize(this->dim_, T(.0));
	
};
/**
 *  \brief Add stiffness mass and rhs data.
 *  \param[in] it index pair of row and column.
 *  \param[in] val_k value of stiffness.
 *  \param[in] val_m value of mass.
 *  \param[in] val_rhs value of rhs.
 */
template <class T>
void SparseMat<T>::add_matrix_data(SparseCell it, T val_k, T val_m, T val_rhs)
{
	std::array<size_t, 2> index_range;
	if(this->get_format()==SpFmt::CSR){
		index_range[0] = this->aux_[it.row];
		index_range[1] = this->aux_[it.row+1];
	}
	else{
		index_range[0] = this->aux_[it.col];
		index_range[1] = this->aux_[it.col+1];
	}
	auto got = std::find(this->row_col_.begin()+index_range[0],
		this->row_col_.begin()+index_range[1], it);
	auto nn = std::distance(this->row_col_.begin(), got);
	this->stif_[nn] += val_k;
	this->mass_[nn] += val_m;
	this->rhs_[it.row] += val_rhs;
};
/**
 *  \brief Add stiffness and massdata.
 *  \param[in] it index pair of row and column.
 *  \param[in] val_k value of stiffness.
 *  \param[in] val_m value of mass.
 */
template <class T>
void SparseMat<T>::add_matrix_data_KM(SparseCell it, T val_k, T val_m)
{
	this->add_matrix_data(it, val_k, val_m, T(0));
};
/**
 *  \brief Add stiffness and rhs data.
 *  \param[in] it index pair of row and column.
 *  \param[in] val_k value of stiffness.
 *  \param[in] val_rhs value of rhs.
 */
template <class T>
void SparseMat<T>::add_matrix_data_KF(SparseCell it, T val_k, T val_rhs)
{
	this->add_matrix_data(it, val_k, T(0), val_rhs);
};
/**
 *  \brief Add stiffness mass and rhs data.
 *  \param[in] ir row index.
 *  \param[in] jc column index.
 *  \param[in] val_k value of stiffness.
 *  \param[in] val_m value of mass.
 *  \param[in] val_rhs value of rhs.
 */
template <class T>
void SparseMat<T>::add_matrix_data(size_t ir, size_t jc, T val_k, T val_m, T val_rhs)
{
	SparseCell it{ir, jc};
	this->add_matrix_data(it, val_k, val_m, val_rhs);
};
/**
 *  \brief Add stiffness and mass data.
 *  \param[in] ir row index.
 *  \param[in] jc column index.
 *  \param[in] val_k value of stiffness.
 *  \param[in] val_m value of mass.
 */
template <class T>
void SparseMat<T>::add_matrix_data_KM(size_t ir, size_t jc, T val_k, T val_m)
{
	SparseCell it{ir, jc};
	this->add_matrix_data_KM(it, val_k, val_m);
};
/**
 *  \brief Add stiffness and rhs data.
 *  \param[in] ir row index.
 *  \param[in] jc column index.
 *  \param[in] val_k value of stiffness.
 *  \param[in] val_rhs value of rhs.
 */
template <class T>
void SparseMat<T>::add_matrix_data_KF(size_t ir, size_t jc, T val_k, T val_rhs)
{
	SparseCell it{ir, jc};
	this->add_matrix_data_KF(it, val_k, val_rhs);
};

/**
 *  \brief Get stiffness sparse matrix pointer to MAT.
 *  \return
 */
template <class T>
std::unique_ptr<mat_sparse_t> SparseMat<T>::get_stif_mat()
{
	std::unique_ptr<mat_sparse_t> stif(new mat_sparse_t);
	stif->nir = stif->ndata = stif->nzmax = this->get_nnz();
	stif->njc = this->get_dim() + 1;
	stif->ir = new int[stif->nir]();
	if(this->get_format()==SpFmt::CSR){
		for(size_t i=0; i<stif->nir; i++)stif->ir[i] = this->row_col_[i].col;
	}
	else{
		for(size_t i=0; i<stif->nir; i++)stif->ir[i] = this->row_col_[i].row;
	}
	stif->jc = new int[stif->njc]();
	for(size_t i=0; i<stif->njc; i++)stif->jc[i] = this->aux_[i];
	stif->data = (void*)(this->stif_.data());
	return std::move(stif);
}
/**
 *  \brief Get mass sparse matrix pointer to MAT.
 *  \return
 */
template <class T>
std::unique_ptr<mat_sparse_t> SparseMat<T>::get_mass_mat()
{
	std::unique_ptr<mat_sparse_t> mass(new mat_sparse_t);
	mass->nir = mass->ndata = mass->nzmax = this->get_nnz();
	mass->njc = this->get_dim() + 1;
	mass->ir = new int[mass->nir]();
	if(this->get_format()==SpFmt::CSR){
		for(size_t i=0; i<mass->nir; i++)mass->ir[i] = this->row_col_[i].col;
	}
	else{
		for(size_t i=0; i<mass->nir; i++)mass->ir[i] = this->row_col_[i].row;
	}
	mass->jc = new int[mass->njc]();
	for(size_t i=0; i<mass->njc; i++)mass->jc[i] = this->aux_[i];
	mass->data = (void*)(this->mass_.data());
	return std::move(mass);
}

}