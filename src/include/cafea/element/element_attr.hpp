/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#ifndef CAFEA_ELEMENT_ATTR_HPP_
#define CAFEA_ELEMENT_ATTR_HPP_

#include <cstddef>

#include "cafea/base/enum_lib.hpp"

namespace cafea {
/**
 *  \brief Attribution of element.
 */
struct ElementAttr {
    //! Get dofs per node.
	static size_t get_dofs_per_node(ElementType);
	//! Get activated number of nodes.
	static size_t get_active_num_of_node(ElementType);
	//! Get shape function order.
	static size_t get_element_order(ElementType);
	//! Get element type in Ansys rules.
	static size_t get_element_type_id(ElementType);
};
}  // namespace cafea
#endif  // CAFEA_ELEMENT_ATTR_HPP_
