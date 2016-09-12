#ifndef EIGENPAIR_H
#define EIGENPAIR_H

#include <Eigen/Eigen>

#include "sparse_matrix.h"

namespace cafea
{
/**
 *  \brief Linear solver for static solution.
 */
template <class T=double, class Solver=Eigen::SimplicialLDLT<Eigen::SparseMatrix<T>>>
class LinearSolver{
	public:
		//! Destructor.
		virtual ~LinearSolver() {clear();};
		//! Load K.
		void load(const T*, const SparseCell*, size_t, size_t);
		//! Analyze pattern.
		void analyze() {solver_.analyzePattern(matA_); isAnalyzed_ = true;};
		//! Factorize matirx K.
		void factorize()
		{
			if(!isAnalyzed_)analyze();
			solver_.factorize(matA_);
			isFactorized_ = true;
		};
		//! Analyze and factorize.
		void analyze_factorize()
		{
			if(!isAnalyzed_)analyze();
			if(!isFactorized_)factorize();
		};
		//! Solve.
		vecX_<T> solve(const T* rhs, size_t n)
		{
			analyze_factorize();
			bb_.resize(n);
			bb_.setZero();
			for(int i=0; i<n; i++)bb_(i) = rhs[i];
			xx_ = solver_.solve(bb_);
			return xx_;
		};
		//! Clear variables.
		virtual void clear()
		{
			matA_.resize(0, 0);
			matA_.setZero();
			matA_.data().squeeze();
			xx_.resize(0);
			bb_.resize(0);
			isAnalyzed_ = isFactorized_ = false;
		};
	protected:
		Eigen::SparseMatrix<T> matA_;//!< Global stiffness matrix.
		vecX_<T> xx_, bb_;//!< Result and RHS vector.
		Solver solver_;//!< Solver of sparse matrix.
		bool isAnalyzed_{false}, isFactorized_{false};
};

/**
 *  \brief Solve eigenpair problems.
 */
template <class T=double, class Solver=Eigen::SimplicialLDLT<Eigen::SparseMatrix<T>>>
class EigenSolver: public LinearSolver<T, Solver> {
	public:
		//! Destructor.
		~EigenSolver() {clear();};
		//! Load K M matrix.
		void load(const T*, const T*, const SparseCell*, size_t, size_t);
		//! Clear variables.
		void clear() override
		{
			LinearSolver<T, Solver>::matA_.resize(0, 0);
			LinearSolver<T, Solver>::matA_.setZero();
			LinearSolver<T, Solver>::matA_.data().squeeze();
			
			matB_.resize(0, 0);
			matB_.setZero();
			matB_.data().squeeze();
			
			X_.resize(0, 0);
			lambda_.resize(0, 0);
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
		Eigen::SparseMatrix<T> matB_;//!< Global mass matrix.
		matrix_<T> X_, lambda_;//!< Eigenvectors and eigenvalues.
};

//!< Specialization.
template class LinearSolver<REAL4, Eigen::SimplicialLDLT<Eigen::SparseMatrix<REAL4>>>;
template class LinearSolver<REAL8, Eigen::SimplicialLDLT<Eigen::SparseMatrix<REAL8>>>;

template class LinearSolver<REAL4, Eigen::SparseLU<Eigen::SparseMatrix<REAL4>>>;
template class LinearSolver<REAL8, Eigen::SparseLU<Eigen::SparseMatrix<REAL8>>>;

template class EigenSolver<REAL4, Eigen::SimplicialLDLT<Eigen::SparseMatrix<REAL4>>>;
template class EigenSolver<REAL8, Eigen::SimplicialLDLT<Eigen::SparseMatrix<REAL8>>>;

template class EigenSolver<REAL4, Eigen::SparseLU<Eigen::SparseMatrix<REAL4>>>;
template class EigenSolver<REAL8, Eigen::SparseLU<Eigen::SparseMatrix<REAL8>>>;

template class EigenSolver<REAL4, Eigen::SparseQR<Eigen::SparseMatrix<REAL4>, Eigen::COLAMDOrdering<int>>>;
template class EigenSolver<REAL8, Eigen::SparseQR<Eigen::SparseMatrix<REAL8>, Eigen::COLAMDOrdering<int>>>;
}
#endif
