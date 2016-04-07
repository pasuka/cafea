#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include <cstddef>
#include <vector>
#include <string>

namespace cafea
{	
/**
 *  Struct for cell in global matrix.
 */
struct SparseCell {
	size_t row, col;//!< Row and Column index.
	//! Default constructor.
	SparseCell(){};
	//! Default deconstructor.
	~SparseCell(){};
	//! Constructor with row and column index.
	SparseCell(size_t ir, size_t jc):row(ir), col(jc){};
	//! Compare ().
	bool operator() (const SparseCell *a, const SparseCell *b) const
	{
		return a->row==b->row && a->col==b->col;
	}
	//! Compare <.
	bool operator< (const SparseCell *a) const
	{
		return this->row==a->row && this->col < a->col;
	}
	//! Compare .equal.
	bool operator== (const SparseCell *a) const
	{
		return this->row==a->row && this->col==a->col;
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
 *  Struct for global matrix data storge.
 */
template <class T=double>
struct SparseMat {
	std::string storge("CSR");//!< Storage method.
	bool is_sym{true};//!< Symmetry matrix.
	size_t dim{0};//!< Dimension of matrix.
	size_t nnz{0};//!< Non-zeros of matrix.
	std::vector<SparseCell> row_col;//!< Row and column index.
	std::vector<size_t> aux;//!< Auxiliary index.
	std::vector<T> stif, mass;//!< Global mass and stiffness values.
	std::vector<T> rhs;//!< Global RHS values.
	//! Default constructor.
	SparseMat(){};
	//! Deconstructor.
	~SparseMat() {init_mat();};
	//! Initialize.
	void init_mat()
	{
		if(!row_col.empty())row_col.clear();
		if(!aux.empty())aux.clear();
		if(!stif.empty())stif.clear();
		if(!mass.empty())mass.clear();
		if(!rhs.empty())rhs.clear();
	};
};
}
#endif