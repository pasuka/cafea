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
/*template <class T, class U>
void Node<T, U>::init_result(SolutionType sol, int n)
{
	if(!this->is_activated())return;
	auto m = this->dof_mgr_.get_num_dofs();
	switch(sol){
	case SolutionType::STATIC:
		this->disp_ = matrix_<U>::Zero(m, 1);
	case SolutionType::MODAL:
		this->disp_ = matrix_<U>::Zero(m, n);
	case SolutionType::HARMONIC:
	
	case SolutionType::UNKNOWN:
	default:
		fmt::print("Unsupported solution type definition\n");
	}
}*/
}