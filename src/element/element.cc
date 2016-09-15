#include <cafea.h>

namespace cafea
{
/**
 *  \brief Get number of active nodes.
 *  \return number of active node.
 */
template <class T>
size_t Element<T>::get_active_num_of_node()const
{
	return ElementAttr::get_active_num_of_node(this->etype_);
}
/**
 *  \brief Get dofs of each node.
 *  \return dofs of one node.
 */
template <class T>
size_t Element<T>::get_dofs_per_node()const
{
	return ElementAttr::get_dofs_per_node(this->etype_);
}
/**
 *  \brief Get order of shape function of element.
 *  \return polyonimal order of shape function.
 */
template <class T>
size_t Element<T>::get_element_order()const
{
	return ElementAttr::get_element_order(this->etype_);
}
/**
 *  \brief Form element matrix.
 */
template <class T>
void Element<T>::form_matrix(const Node<REAL4, T> p[], const Material<REAL4> *matl, const Section<REAL4> *sect)
{
	auto opt = this->get_option();
	switch(this->etype_){
	case ElementType::PIPE16:
		std::tie(this->stif_, this->mass_, this->tran_, this->rhs_, this->attr_) = StructuralElement<REAL4, T>::pipe16(&p[0], &p[1], matl, sect);
		break;
	case ElementType::PIPE18:
		std::tie(this->stif_, this->mass_, this->tran_, this->rhs_, this->attr_) = StructuralElement<REAL4, T>::pipe18(&p[0], &p[1], &p[2], matl, sect);
		break;
	case ElementType::MASS21:
		std::tie(this->stif_, this->mass_, this->tran_, this->rhs_, this->attr_) = StructuralElement<REAL4, T>::mass21(&p[0], matl, sect, opt.data());
		break;
	case ElementType::COMBIN14:
		std::tie(this->stif_, this->mass_, this->tran_, this->rhs_, this->attr_) = StructuralElement<REAL4, T>::combin14(&p[0], &p[1], matl, sect, opt.data());
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
 *  \brief Post process.
 *  \param[in] x solution of result.
 */
template <class T>
void Element<T>::post_stress(const vecX_<T> x)
{
	
	switch(this->etype_){
	case ElementType::PIPE16:
	case ElementType::PIPE18:
		// fmt::print("Not yet finish stress result.\n");
		this->result_ = StructuralElementPost<T>::pipe(this->stif_, this->tran_, x, this->attr_);
		break;
	case ElementType::MASS21:
	case ElementType::COMBIN14:
		fmt::print("No stress equation for spring and mass element.\n");
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
template <class T>
std::array<size_t, 2> Element<T>::get_matrix_shape() const
{
	std::array<size_t, 2> sz;
	sz[0] = 0 < this->stif_.rows() ? this->stif_.rows() : 0;
	sz[1] = 0 < this->stif_.cols() ? this->stif_.cols() : 0;
	return sz;
};
/**
 *  \brief Get id of element type.
 */
template <class T>
size_t Element<T>::get_element_type_id() const
{
	return ElementAttr::get_element_type_id(this->etype_);
}
}