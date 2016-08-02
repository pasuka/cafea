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
}