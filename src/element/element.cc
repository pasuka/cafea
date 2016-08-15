#include <cafea.h>

namespace cafea
{
/**
 *  \brief Get number of active nodes.
 *  \return number of active node.
 */
template <class T, class U>
int Element<T, U>::get_active_num_of_node()const
{
	return element_attr_::get_active_num_of_node(this->etype_);
}
/**
 *  \brief Get dofs of each node.
 *  \return dofs of one node.
 */
template <class T, class U>
int Element<T, U>::get_dofs_per_node()const
{
	return element_attr_::get_dofs_per_node(this->etype_);
}
/**
 *  \brief Get order of shape function of element.
 *  \return polyonimal order of shape function.
 */
template <class T, class U>
int Element<T, U>::get_element_order()const
{
	return element_attr_::get_element_order(this->etype_);
}
/**
 *  \brief Form element matrix.
 */
template <class T, class U>
void Element<T, U>::form_matrix(const Node<REAL4, U> p[],
	const Material<REAL4> *matl, const Section<REAL4> *sect)
{
	auto opt = this->get_option();
	switch(this->etype_){
	case ElementType::PIPE16:
		using pipe_elem_lib::pipe16;
		std::tie(this->stif_, this->mass_, this->tran_, this->rhs_) = pipe16<REAL4>(&p[0], &p[1], matl, sect);
		break;
	case ElementType::PIPE18:
		using pipe_elem_lib::pipe18;
		std::tie(this->stif_, this->mass_, this->tran_, this->rhs_) = pipe18<REAL4>(&p[0], &p[1], &p[2], matl, sect);
		break;
	case ElementType::MASS21:
		using additional_elem_lib::mass21;
		std::tie(this->stif_, this->mass_, this->tran_, this->rhs_) = mass21<REAL4>(&p[0], matl, sect, opt.data());
		break;
	case ElementType::COMBIN14:
		using additional_elem_lib::combin14;
		std::tie(this->stif_, this->mass_, this->tran_, this->rhs_) = combin14<REAL4>(&p[0], &p[1], matl, sect, opt.data());
		break;
	case ElementType::BEAM188:
	case ElementType::B31:
	case ElementType::C3D4:
	case ElementType::SOLID185:
	case ElementType::C3D8:
	case ElementType::S3R:
	case ElementType::S4R:
	case ElementType::SHELL181:
	case ElementType::BEAM189:
	case ElementType::B32:
	case ElementType::SHELL281:
	case ElementType::S8R:
	case ElementType::S9R:
	case ElementType::SOLID186:
	case ElementType::C3D20:
	case ElementType::UNKNOWN:
	default: fmt::print("Unsupported element type\n");
	}
};
/**
 *  \brief Get shape of element matrix.
 *  \return shape array of element matrix.
 */
template <class T, class U>
std::array<size_t, 2> Element<T, U>::get_matrix_shape() const
{
	std::array<size_t, 2> sz;
	sz[0] = 0 < this->stif_.rows() ? this->stif_.rows() : 0;
	sz[1] = 0 < this->stif_.cols() ? this->stif_.cols() : 0;
	return sz;
};
}