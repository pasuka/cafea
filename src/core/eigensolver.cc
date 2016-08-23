#include "cafea.h"

namespace cafea
{
/**
 *  \brief Load data to sparese matrix.
 *  \param[in] stif pointer of stiffness matrix.
 *  \param[in] mass pointer of mass matrix.
 *  \param[in] xy row and cloumn of global matrix.
 *  \param[in] nnz non-zeros of global matrix.
 */
template <class T>
void EigenSolver<T>::load(const T *stif, const T *mass, const SparseCell *xy, size_t nnz, size_t dim)
{
	this->clear();
	this->matA_.resize(dim, dim);
	this->matB_.resize(dim, dim);
	this->matA_.reserve(nnz);
	this->matB_.reserve(nnz);
	std::vector<Eigen::Triplet<T>> triList(nnz);
	for(int i=0; i<nnz; i++)triList.push_back(Eigen::Triplet<T>(xy[i].row, xy[i].col, stif[i]));
	this->matA_.setFromTriplets(triList.begin(), triList.end());
	triList.clear();
	for(int i=0; i<nnz; i++)triList[i] = Eigen::Triplet<T>(xy[i].row, xy[i].col, mass[i]);
	this->matB_.setFromTriplets(triList.begin(), triList.end());
	triList.clear();
};
/**
 *  \brief Sturm check for eigenpairs in specificy range.
 *  \param[in] right up bound of range.
 *  \param[in] left low bound of range.
 *  \return number of eigenpairs in range.
 */
template <class T>
size_t EigenSolver<T>::sturm_check(T right, T left)
{
	decltype(this->matA_) A1 = this->matA_ - left *this->matB_;
	decltype(this->matA_) A2 = this->matA_ - right*this->matB_;
	
	this->solver_.analyzePattern(this->matA_);   // for this step the numerical values of A are not used
	
	this->solver_.factorize(A1);
	vecX_<T> dd1 = this->solver_.vectorD();
	this->solver_.factorize(A2);
	vecX_<T> dd2 = this->solver_.vectorD();
	for(auto i=0; i<dd1.size(); i++)fmt::print("D1:{}\tD2:{}\n", dd1(i), dd2(i));
	auto aa1{(dd1.array()<0.0).count()};
	auto aa2{(dd2.array()<0.0).count()};
	size_t nn = (aa2-aa1)<0 ? 0 : aa2-aa1;
	fmt::print("AA2:{}\tAA1:{}\n", aa2, aa1);
	fmt::print("In range:{} to {}\t exists {} eigenpairs\n", left, right, nn);
	
	return nn;
};
/**
 *  \brief Modify Gram-Schimidt B-orthogonal algorithm.
 */
template <class T>
matrix_<T> EigenSolver<T>::mgs(matrix_<T> X, const Eigen::SparseMatrix<T> B)
{
	auto n{X.cols()};
	matrix_<T> R = matrix_<T>::Zero(n, n);
	matrix_<T> Y = X;
	Y.fill(T(0));
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
template <class T>
void EigenSolver<T>::subspace(T right, T left)
{
	size_t nn = this->sturm_check(right, left);
	this->subspace(int(nn));
};
/**
 *  \brief Subspace Iteration Method.
 */
template <class T>
void EigenSolver<T>::subspace(int nreq, T tol, T sigma)
{
	int ncv = 2*nreq<(8+nreq) ? 2*nreq : 8+nreq;
	
	assert(ncv<=this->matA_.rows());
	assert(ncv<=this->matA_.cols());
	
	Eigen::SimplicialLDLT<decltype(this->matA_)> solver;
	decltype(this->matA_) A = this->matA_ - sigma*this->matB_;
	
	solver.analyzePattern(A);   // for this step the numerical values of A are not used
	solver.factorize(A);
	
	matrix_<T> y, y0, y2;
	y = this->mgs(matrix_<T>::Random(A.rows(), ncv), this->matB_);
	y0 = y;
	y0.fill(T(0));
	const size_t max_iter{98};
	size_t iter{0};
	matrix_<T> Res, sol, ak, am, vv;
	vecX_<T> w;
	
	while(iter <= max_iter){
		sol = solver.solve(this->matB_*y);
		std::cout << sol << "\n";
		y0 = y;
		ak = sol.transpose()*A*sol;
		am = sol.transpose()*this->matB_*sol;
		Eigen::GeneralizedSelfAdjointEigenSolver<matrix_<T>> es(ak, am);
		w = es.eigenvalues();
		if(iter<2)std::cout << ak << "\n";
		vv = es.eigenvectors();
		y2 = sol*vv;
		
		for(int ij=0; ij<ncv; ij++)y2.col(ij) /= y2.col(ij).norm();
		y = this->mgs(y2, this->matB_);
	
		Res = y0.leftCols(nreq) - y.leftCols(nreq)*(y.leftCols(nreq).transpose()*(this->matB_*y0.leftCols(nreq)));
		fmt::print("Iter:{}\tRES:{}\n", iter, Res.norm());
		if(Res.norm() < tol)break;
		iter++;
	}
	
	this->lambda_.resize(nreq, 2);
	this->X_.resize(this->matA_.rows(), nreq);
	this->lambda_.col(0) = (w.head(nreq).array()+sigma).abs();
	for(int i=0; i<nreq; i++)this->lambda_(i, 1) = Res.col(i).norm();
	this->X_ = y.leftCols(nreq);
	for(auto i=0; i<nreq; i++)fmt::print("No.{}\tFrequency:{}\n", i, sqrt(this->lambda_(i, 0))/T(2)/PI<T>());
};
}