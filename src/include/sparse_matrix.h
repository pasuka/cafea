#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include <cstddef>
#include <vector>
#include <string>
#include <algorithm>

#include "matio.h"
#include "fmt/format.h"

#include "utils.h"

namespace cafea
{	
/**
 *  Struct for cell in global matrix.
 */
struct SparseCell {
	size_t row, col;//!< Row and Column index.
	//! Constructor with row and column index.
	SparseCell(size_t ir, size_t jc):row(ir), col(jc){};
	//! Compare ().
	bool operator() (const SparseCell *a, const SparseCell *b) const
	{
		return a->row==b->row && a->col==b->col;
	}
	//! Compare <.
	bool operator< (const SparseCell &a) const
	{
		return this->row==a.row && this->col < a.col;
	}
	//! Compare .equal.
	bool operator== (const SparseCell &a) const
	{
		return this->row==a.row && this->col==a.col;
	}
};

//! Sort by row index.
//! CSR format.
bool sort_by_row(const SparseCell&, const SparseCell&);
//! Sort by column index.
//! CSC format.
bool sort_by_col(const SparseCell&, const SparseCell&);
//! Compare .equal.
bool compare_pair(const SparseCell&, const SparseCell&);

/**
 *  Enum of sparse matrix format.
 */
enum struct SpFmt{
	CSC,//!< Compressed Sparse Column matrix.
	CSR,//!< Compressed Sparse Row matrix.
	COO,//!< COOrdinate sparse matrix.
};
/**
 *  Enum of storge information.
 */
enum struct SpStorage{
	FULL,//!< Whole matrix.
	UP_TRIANGLE,//!< Up triangle of matrix.
	LOW_TRIANGLE,//!< Low triangle of matrix.
};
/**
 *  Enum of sparse matrix symmetric.
 */
enum struct SpSym{
	SYMMETRIC,//!< Symmetry matrix.
	UNSYMMETRIC,//!< unsymmetry matrix.
};
/**
 *  Struct for global matrix data storge.
 */
template <class T=double>
class SparseMat{
	public:
		//! Default constructor.
		SparseMat(){};
		//! Deconstructor.
		~SparseMat(){clear();};
		//! Initialize.
		void clear()
		{
			dim_ = nnz_ = 0;
			format_ = SpFmt::CSC;
			storge_ = SpStorage::FULL;
			sym_ = SpSym::SYMMETRIC;
			if(!row_col_.empty())row_col_.clear();
			if(!aux_.empty())aux_.clear();
			if(!rhs_.empty())rhs_.clear();
			if(!stif_.empty())stif_.clear();
			if(!mass_.empty())mass_.clear();
		};
		//! Get dimension.
		size_t get_dim() const {return dim_;};
		//! Get Non-zeros.
		size_t get_nnz() const {return nnz_;};
		//! Get matrix format.
		SpFmt get_format() const {return format_;};
		//! Inquire symmetric of matrix.
		bool is_symmetric() const {return SpSym::SYMMETRIC==sym_;};
		//! Inquire full storage of matrix.
		bool is_full() const {return SpStorage::FULL==storge_;};
		//! Set matrix format.
		void set_format(SpFmt t) {format_ = t;};
		//! Set symmetric.
		void set_symmetric(bool val=true) {if(val)sym_ = SpSym::SYMMETRIC;};
		//! Add index pair.
		void append(SparseCell it){row_col_.push_back(it);};
		void append(size_t ir, size_t jc){row_col_.push_back({ir, jc});};
		//! Remove duplicated index pair.
		void unique(SpFmt t=SpFmt::CSC);
		//! Add stiffness mass and rhs value.
		void add_matrix_data(SparseCell, T, T, T);
		void add_matrix_data(size_t, size_t, T, T, T);
		//! Add stiffness and mass value.
		void add_matrix_data_KM(SparseCell, T, T);
		void add_matrix_data_KM(size_t, size_t, T, T);
		//! Add stiffness and rhs value.
		void add_matrix_data_KF(SparseCell, T, T);
		void add_matrix_data_KF(size_t, size_t, T, T);
		//! Get stiffness matrix pointer.
		const T* get_stif_ptr() const {return stif_.data();};
		//! Get mass matrix pointer.
		const T* get_mass_ptr() const {return mass_.data();};
		//! Get rhs pointer.
		const T* get_rhs_ptr() const {return rhs_.data();};
		//! Get coordinate (row, column) pointer.
		const SparseCell* get_coord_ptr() const {return row_col_.data();};
		//! Get auxility pointer.
		const size_t* get_aux_ptr() const {return aux_.data();};
		//! Export MAT sparse.
		std::unique_ptr<mat_sparse_t> get_stif_mat();
		std::unique_ptr<mat_sparse_t> get_mass_mat();
 	private:
		SpFmt format_{SpFmt::CSC};//!< Storage method.
		SpSym sym_{SpSym::SYMMETRIC};//!< Symmetry matrix.
		SpStorage storge_{SpStorage::FULL};//!< Whole matrix.
		size_t dim_{0};//!< Dimension of matrix.
		size_t nnz_{0};//!< Non-zeros of matrix.
		std::vector<SparseCell> row_col_;//!< Row and column index.
		std::vector<size_t> aux_;//!< Auxiliary index.
		std::vector<T> stif_, mass_;//!< Global mass and stiffness values.
		std::vector<T> rhs_;//!< Global RHS values.
};
//!< Specialization
template class SparseMat<REAL4>;
template class SparseMat<REAL8>;
template class SparseMat<COMPLEX4>;
template class SparseMat<COMPLEX8>;
}
#endif