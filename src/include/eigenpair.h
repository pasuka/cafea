/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#ifndef _CAFEA_EIGENPAIR_H_
#define _CAFEA_EIGENPAIR_H_

#include <Eigen/Eigen>

#include "./sparse_matrix.h"

namespace cafea {
/**
 *  \brief Linear solver for static solution.
 */
template <class T = double, class Solver = Eigen::SimplicialLDLT<Eigen::SparseMatrix<T>>>
class LinearSolver {
	public:
		//! Destructor.
		virtual ~LinearSolver() { clear();};
		//! Load K.
		void load(const T*, const SparseCell*, size_t, size_t);
		//! Analyze pattern.
		void analyze() { solver_.analyzePattern(matA_); isAnalyzed_ = true;};
		//! Factorize matirx K.
		void factorize() {
			if (!isAnalyzed_) analyze();
			solver_.factorize(matA_);
			isFactorized_ = true;
		}
		//! Analyze and factorize.
		void analyze_factorize() {
			if (!isAnalyzed_) analyze();
			if (!isFactorized_) factorize();
		}
		//! Solve.
		bool solve(const T* rhs, size_t n) {
			analyze_factorize();
			vecX_<T> bb = vecX_<T>::Zero(n);
			for (int i = 0; i < n; i++) bb(i) = rhs[i];
			// std::cout << "RHS:\n" << bb << "\n";
			xx_ = solver_.solve(bb);
			if (solver_.info() != Eigen::ComputationInfo::Success) {
				isSolved_ = false;
			} else {
				isSolved_ = true;
			}
			return  isSolved_;
		}
		//! Clear variables.
		virtual void clear() {
			matA_.resize(0, 0);
			matA_.setZero();
			matA_.data().squeeze();
			xx_.resize(0);
			isAnalyzed_ = isFactorized_ = isSolved_ = false;
		}
		//! Get result.
		vecX_<T> get_X() const { return xx_;}
		//! Get flag of solver status.
		bool get_info() const { return isSolved_;}

	protected:
		Eigen::SparseMatrix<T> matA_;//!< Global stiffness matrix.
		vecX_<T> xx_;//!< Result vector.
		Solver solver_;//!< Solver of sparse matrix.
		bool isAnalyzed_{false}, isFactorized_{false};//!< Flag of solver status.
		bool isSolved_{false};//!< Flag of solver status.
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
			LinearSolver<T, Solver>::clear();

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
		//! Get eigenvalues.
		matrix_<T> get_eigenvalues() const { return lambda_;};
		//! Get eigenvectors.
		matrix_<T> get_eigenvectors() const { return X_;};
		//! Get eigenpairs.
		std::tuple<matrix_<T>, matrix_<T>> get_eigenpairs() const { return std::make_tuple(lambda_, X_);};
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
