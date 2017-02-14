#ifndef ELEMENT_ATTR_H
#define ELEMENT_ATTR_H

#include <cstddef>

#include "enum_lib.h"

namespace cafea
{
/**
 *  \brief Attribution of element.
 */
struct ElementAttr
{
    //! Get dofs per node.
	static size_t get_dofs_per_node(ElementType);
	//! Get activated number of nodes.
	static size_t get_active_num_of_node(ElementType);
	//! Get shape function order.
	static size_t get_element_order(ElementType);
	//! Get element type in Ansys rules.
	static size_t get_element_type_id(ElementType);
};
}
#endif
