#ifndef EIGENPAIR_H
#define EIGENPAIR_H

#include <Eigen/Eigen>

#include "sparse_matrix.h"

namespace cafea
{
/**
 *  \brief Solve eigenpair problems.
 */
template <class T=double>
class EigenSolver {
	public:
		EigenSolver(){};
		~EigenSolver(){clear();};
		void load(const T*, const T*, const SparseCell*, size_t, size_t);
		void clear()
		{
			matA_.resize(0, 0);
			matB_.resize(0, 0);
			X_.resize(0, 0);
			lambda_.resize(0, 0);
			matA_.setZero();
			matB_.setZero();
			matA_.data().squeeze();
			matB_.data().squeeze();
		};
		void subspace(int num=1, T tol=EPS<T>(), T sigma=T(-1));
		void subspace(T ubound, T lbound=T(0));
		size_t sturm_check(T ubound, T lbound=T(0));
		matrix_<T> mgs(matrix_<T> y, const Eigen::SparseMatrix<T> B);
	private:
		Eigen::SparseMatrix<T> matA_, matB_;
		matrix_<T> X_, lambda_;
		Eigen::SimplicialLDLT<Eigen::SparseMatrix<T>> solver_;
};

//!< Specialization.
template class EigenSolver<REAL4>;
template class EigenSolver<REAL8>;
}
#endif
