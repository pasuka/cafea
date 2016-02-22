#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include <cstddef>
#include <vector>
#include <string>

namespace cafea {
	
	/**
	 *  Struct for cell in global matrix.
	 */
	struct SparseCell {
		size_t row, col;//!< Row and Column index.
		//! Default constructor.
		SparseCell(){};
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
			return this->row==a.row && this->col < a.col;
		}
		//! Compare .equal.
		bool operator== (const SparseCell *a) const
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
	 *  Struct for global matrix data storge.
	 */
	template <class T>
	struct SparseMat {
		std::string storge("CSR");
		bool is_full{true};
		bool is_sym{true};
		size_t dim{0};
		size_t nnz{0};
		std::vector<SparseCell> row_col;
		std::vector<size_t> aux;
		std::vector<T> stif, mass;
	};
}

#endif