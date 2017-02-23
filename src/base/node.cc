#include "node.h"

namespace cafea
{
/**
 *  \brief Init dof container.
 *  \param[in] et element type enum.
 */
template <class Scalar, class ResultScalar>
void Node<Scalar, ResultScalar>::dof_init(ElementType et)
{
	auto num = ElementAttr::get_dofs_per_node(et);
	this->dof_mgr_.set_num_dofs(num);
}
/**
 *  \brief Apply boundary.
 *  \param[in] bc boundary type.
 */
template <class Scalar, class ResultScalar>
void Node<Scalar, ResultScalar>::dof_apply(Boundary<Scalar> bc)
{
	switch(bc.get_boundary_type()){
	case BoundaryType::FIXED:
		this->dof_mgr_.set_constraint(bc.get_dof_label(), DofType::ELIMINATE);
		break;
	case BoundaryType::INIT_VAL:
		this->dof_mgr_.set_constraint(bc.get_dof_label(), DofType::CONSTRAINT);
		break;
	case BoundaryType::UNKNOWN:
	default:
		fmt::print("Unsupported boundary type\n");
	}
}
/**
 *  \brief Initialize result variables.
 *  \param [in] sol type of solution.
 *  \param [in] n i-th column of result.
 */
template <class T, class U>
void Node<T, U>::init_result(SolutionType sol, int n)
{
	if(!this->is_activated())return;
	auto m = this->dof_mgr_.get_num_dofs();
	switch(sol){
	case SolutionType::STATIC:
		this->disp_ = matrix_<U>::Zero(m, 1);
		break;
	case SolutionType::MODAL:
		this->disp_ = matrix_<U>::Zero(m, n);
		break;
	case SolutionType::HARMONIC_FULL:
		this->disp_cmplx_ = matrix_<COMPLEX<U>>::Zero(m, n);
		this->stress_cmplx_ = matrix_<COMPLEX<U>>::Zero(8, n);
		break;
	case SolutionType::HARMONIC_MODAL_SUPERPOSITION:
	default:
		fmt::print("Unsupported solution type definition\n");
	}
}
/**
 *  \brief Set result in complex domain.
 *  \param [in] sol type of solution.
 *  \param [in] lt type of load.
 *  \param [in] n i-th column of result.
 *  \param [in] rst result matrix.
 */
template <class T, class U>
void Node<T, U>::set_result(SolutionType sol, LoadType lt, int n, matrix_<U> rst)
{
	if(!this->is_activated())return;

	switch(sol){
	case SolutionType::STATIC:
		switch(lt){
		case LoadType::DISP:
			this->disp_ = rst;
			break;
		default:
			fmt::print("Unsupported result type\n");
		}
		break;
	case SolutionType::MODAL:
		switch(lt){
		case LoadType::DISP:
			this->disp_ = rst;
			break;
		default:
			fmt::print("Unsupported result type\n");
		}
		break;
	case SolutionType::HARMONIC_FULL:
	case SolutionType::HARMONIC_MODAL_SUPERPOSITION:
	default:
		fmt::print("Unsupported solution type definition\n");
	}
}
/**
 *  \brief Get result matrix or vectors.
 *  \param [in] sol type of solution.
 *  \param [in] lt type of load.
 *  \param [in] n i-th column of result.
 *  \return result matrix or vector.
 */
template <class T, class U>
matrix_<U> Node<T, U>::get_result(SolutionType sol, LoadType lt, int n) const
{
	matrix_<U> tmp;

	if(!this->is_activated())return tmp;

	switch(sol){
	case SolutionType::STATIC:
		tmp = this->disp_;
		break;
	case SolutionType::MODAL:
		if(0>n){
			tmp = this->disp_;
		}
		else{
			tmp = this->disp_.col(n);
		}
		break;
	case SolutionType::HARMONIC_FULL:
	case SolutionType::HARMONIC_MODAL_SUPERPOSITION:
	default:
		fmt::print("Unsupported solution type definition\n");
	}
	return tmp;
}
}
