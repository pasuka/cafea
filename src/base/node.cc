#include "cafea.h"

using Eigen::AngleAxis;

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
 *  \brief 
 *  \param[in]
 *  \param[in]
 */
template <class Scalar, class ResultScalar>
void Node<Scalar, ResultScalar>::dof_accum(int *it, DofType mt)
{
	this->dof_mgr_.accum(it, mt);
}

/**
 *  \brief Initialize result variables.
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
		break;
	case SolutionType::HARMONIC_MODAL_SUPERPOSITION:
	default:
		fmt::print("Unsupported solution type definition\n");
	}
}
/**
 *
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
 *
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