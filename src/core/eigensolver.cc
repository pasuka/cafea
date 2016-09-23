#include "cafea.h"

using std::tuple;
using std::make_tuple;

using Eigen::Triplet;

namespace cafea
{
/**
 *  \brief Load data to sparse matrix.
 */
template <class T, class U>
void LinearSolver<T, U>::load(const T *stif, const SparseCell *xy, size_t nnz, size_t dim)
{
	this->clear();
	std::vector<Triplet<T>> triList(nnz);
	
	this->matA_.resize(dim, dim);
	this->matA_.reserve(nnz);
	for(int i=0; i<nnz; i++)triList.push_back(Triplet<T>(xy[i].row, xy[i].col, stif[i]));
	this->matA_.setFromTriplets(triList.begin(), triList.end());
	triList.clear();
};

/**
 *  \brief Load data to sparse matrix.
 *  \param[in] stif pointer of stiffness matrix.
 *  \param[in] mass pointer of mass matrix.
 *  \param[in] xy row and cloumn of global matrix.	
 *  \param[in] nnz non-zeros of global matrix.
 */
template <class T, class U>
void EigenSolver<T, U>::load(const T *stif, const T *mass, const SparseCell *xy, size_t nnz, size_t dim)
{
	this->clear();
	std::vector<Triplet<T>> triList(nnz);
	
	this->matA_.resize(dim, dim);
	this->matA_.reserve(nnz);
	for(int i=0; i<nnz; i++)triList.push_back(Triplet<T>(xy[i].row, xy[i].col, stif[i]));
	this->matA_.setFromTriplets(triList.begin(), triList.end());
	triList.clear();
	
	this->matB_.resize(dim, dim);
	this->matB_.reserve(nnz);
	for(int i=0; i<nnz; i++)triList.push_back(Triplet<T>(xy[i].row, xy[i].col, mass[i]));
	this->matB_.setFromTriplets(triList.begin(), triList.end());
	triList.clear();
};

/**
 *  \brief Sturm check for eigenpairs in specificy range.
 *  \param[in] right up bound of range.
 *  \param[in] left low bound of range.
 *  \return number of eigenpairs in range.
 */
template <class T, class U>
size_t EigenSolver<T, U>::sturm_check(T right, T left)
{
	decltype(this->matA_) A1 = this->matA_ - left *this->matB_;
	decltype(this->matA_) A2 = this->matA_ - right*this->matB_;
	
	Eigen::SimplicialLDLT<decltype(this->matA_)> solver;
	
	solver.analyzePattern(this->matA_);   // for this step the numerical values of A are not used
	
	solver.factorize(A1);
	vecX_<T> dd1 = solver.vectorD();
	auto aa1{(dd1.array()<0.0).count()};
	
	solver.factorize(A2);
	vecX_<T> dd2 = solver.vectorD();
	auto aa2{(dd2.array()<0.0).count()};
	
	size_t nn = (aa2-aa1)<0 ? 0 : aa2-aa1;
	
	fmt::print("In range:{} to {}\t exists {} eigenpairs\n", left, right, nn);
	
	return nn;
};

/**
 *  \brief Modify Gram-Schimidt B-orthogonal algorithm.
 *  \param[in] X rhs matrix or vector.
 *  \param[in] B matirx B.
 *  \return orhtogonal matrix or vector.
 */
template <class T, class U>
matrix_<T> EigenSolver<T, U>::mgs(matrix_<T> X, const Eigen::SparseMatrix<T> B)
{
	auto n{X.cols()};
	matrix_<T> R = matrix_<T>::Zero(n, n);
	matrix_<T> Y = X;
	Y.setZero();
	for(int i=0; i<n; i++){
		R(i, i) = sqrt(X.col(i).transpose()*B*X.col(i));
		Y.col(i) = X.col(i)/R(i,i);
		for(int j=i+1; j<n; j++){
			R(i,j) = Y.col(i).transpose()*B*X.col(j);
			X.col(j) -= R(i,j) * Y.col(i);
		}
	}
	return Y;
};

/**
 *  \brief Subspace Iteration Method.
 */
template <class T, class U>
tuple<matrix_<T>, matrix_<T>> EigenSolver<T, U>::subspace(T right, T left, T tol, T sigma)
{
	size_t nn = this->sturm_check(right, left);
	if(nn){
		return this->subspace(int(nn), tol, sigma);
	}
	else{
		return this->subspace(2, tol, sigma);
	}
};

/**
 *  \brief Subspace Iteration Method.
 *  \param[in] nreq required number of eigenpairs.
 *  \param[in] tol tolerance of stop criterion.
 *  \param[in] sigma shift value of eigenvalues.
 */
template <class T, class U>
tuple<matrix_<T>, matrix_<T>> EigenSolver<T, U>::subspace(int nreq, T tol, T sigma)
{
	int ncv = 2*nreq<(8+nreq) ? 2*nreq : 8+nreq;
	
	assert(ncv<=this->matA_.rows());
	assert(ncv<=this->matA_.cols());
	
	decltype(this->matA_) A = this->matA_ - sigma*this->matB_;
	
	this->solver_.analyzePattern(A);// for this step the numerical values of A are not used
	this->solver_.factorize(A);
	
	matrix_<T> y, y0, y2;
	y = this->mgs(matrix_<T>::Random(A.rows(), ncv), this->matB_);
	y0 = y;
	y0.fill(T(0));
	const size_t max_iter{98};
	size_t iter{0};
	matrix_<T> Res, sol, ak, am, vv;
	vecX_<T> w;
	
	auto rerr = [this](auto x1, auto x2){return x1-x2*(x2.transpose()*(this->matB_*x1));};
	
	fmt::print("Begin Subspace Iteration...\n");
	while(iter <= max_iter){
		sol = this->solver_.solve(this->matB_*y);
		y0 = y;
		// Projection to small matrix.
		ak = sol.transpose()*A*sol;
		am = sol.transpose()*this->matB_*sol;
		// Solve eigenpairs of projection matrix.
		Eigen::GeneralizedSelfAdjointEigenSolver<matrix_<T>> es(ak, am);
		w = es.eigenvalues();
		vv = es.eigenvectors();
		// Get new eigenvectors.
		y2 = sol*vv;
		// Normalize eigenvectors.
		for(int ij=0; ij<ncv; ij++)y2.col(ij) /= y2.col(ij).norm();
		// Gram-Schimidt B-orthogonal.
		y = this->mgs(y2, this->matB_);
		// Compute relative error.
		Res = rerr(y0.leftCols(nreq), y.leftCols(nreq));
		fmt::print("Iter:{}\tRES:{}\n", iter, Res.norm());
		// Check for tolerance.
		if(Res.norm()<tol)break;
		iter++;
	}
	// Resize.
	this->lambda_.resize(nreq, 2);
	this->X_.resize(this->matA_.rows(), nreq);
	// Get eigenvalues.
	this->lambda_.col(0) = (w.head(nreq).array()+sigma).abs();
	// Get relative error.
	for(int i=0; i<nreq; i++)this->lambda_(i, 1) = Res.col(i).norm();
	// Get eigenvectors.
	this->X_ = y.leftCols(nreq);
	if(!this->isSolved_)this->isSolved_ = true;
	return make_tuple(this->lambda_, this->X_);
};
}