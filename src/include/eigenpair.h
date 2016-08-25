#ifndef EIGENPAIR_H
#define EIGENPAIR_H

#include <Eigen/Eigen>

#include "sparse_matrix.h"

namespace cafea
{
/**
 *  \brief Solve eigenpair problems.
 */
template <class T=double, class Solver=Eigen::SimplicialLDLT<Eigen::SparseMatrix<T>>>
class EigenSolver {
	public:
		//! Default constructor.
		EigenSolver(){};
		//! Destructor.
		~EigenSolver(){clear();};
		//! Load K M matrix.
		void load(const T*, const T*, const SparseCell*, size_t, size_t);
		//! Clear variables.
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
		//! Subspace iteration method in eigenpair number.
		std::tuple<matrix_<T>, matrix_<T>> subspace(int num=1, T tol=sqrt(EPS<T>()), T sigma=T(-1));
		//! Subspace iteration method in specified range.
		std::tuple<matrix_<T>, matrix_<T>> subspace(T ubound, T lbound=T(0), T tol=sqrt(EPS<T>()), T sigma=T(-1));
		//! Sturm check in specified range.
		size_t sturm_check(T ubound, T lbound=T(0));
		//! Modify Gram-Schimidt B-orthogonal method.
		matrix_<T> mgs(matrix_<T> y, const Eigen::SparseMatrix<T> B);
	private:
		Eigen::SparseMatrix<T> matA_, matB_;//!< Global matrix A and B.
		matrix_<T> X_, lambda_;//!< Eigenvectors and eigenvalues.
		Solver solver_;//!< Solver of sparse matrix.
};

//!< Specialization.
template class EigenSolver<REAL4, Eigen::SimplicialLDLT<Eigen::SparseMatrix<REAL4>>>;
template class EigenSolver<REAL8, Eigen::SimplicialLDLT<Eigen::SparseMatrix<REAL8>>>;

template class EigenSolver<REAL4, Eigen::SparseLU<Eigen::SparseMatrix<REAL4>>>;
template class EigenSolver<REAL8, Eigen::SparseLU<Eigen::SparseMatrix<REAL8>>>;

template class EigenSolver<REAL4, Eigen::SparseQR<Eigen::SparseMatrix<REAL4>, Eigen::COLAMDOrdering<int>>>;
template class EigenSolver<REAL8, Eigen::SparseQR<Eigen::SparseMatrix<REAL8>, Eigen::COLAMDOrdering<int>>>;
}
#endif
