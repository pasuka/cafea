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
	switch(sf){
	case SpFmt::COO:
	case SpFmt::CSC:
		std::sort(this->row_col_.begin(), this->row_col_.end(), sort_by_col);
		break;
	case SpFmt::CSR:
	default:
		std::sort(this->row_col_.begin(), this->row_col_.end(), sort_by_row);
	}
	auto extr = std::unique(this->row_col_.begin(), this->row_col_.end(), compare_pair);
	this->row_col_.erase(extr, this->row_col_.end());
	this->nnz_ = this->row_col_.size();
	this->stif_.resize(this->nnz_, T(.0));
	this->mass_.resize(this->nnz_, T(.0));
	
	if(sf==SpFmt::CSR){
		auto func = std::bind(sort_by_row, std::placeholders::_1, std::placeholders::_2);
	}
	else{
		auto func = std::bind(sort_by_col, std::placeholders::_1, std::placeholders::_2);
	}
	this->aux_.push_back(0);
	size_t iter{0};
	for(size_t i=1; i<this->nnz_; i++){
		if(sort_by_col(this->row_col_[i], this->row_col_[i-1])){
			iter++;
		}
		else{
			this->aux_.push_back(iter);
			iter = 1;
		}
	}
	this->aux_.push_back(iter);
	for(size_t i=1; i<this->aux_.size(); i++)this->aux_[i] += this->aux_[i-1];
	
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
}