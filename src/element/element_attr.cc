/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#include "cafea/element_attr.h"

namespace cafea {
/**
 *  \brief Get dofs on each node.
 *  \return number of dofs on a node.
 */
size_t ElementAttr::get_dofs_per_node(ElementType et) {
	switch (et) {
		case ElementType::MASS21:
		case ElementType::COMBIN14:
		case ElementType::PIPE16:
		case ElementType::BEAM188:
		case ElementType::B31:
		case ElementType::PIPE18:
		case ElementType::BEAM189:
		case ElementType::B32:
		case ElementType::S3R:
		case ElementType::S4R:
		case ElementType::SHELL181:
		case ElementType::SHELL281:
		case ElementType::S8R:
		case ElementType::S9R:   return 6;
		case ElementType::C3D4:
		case ElementType::SOLID185:
		case ElementType::C3D8:
		case ElementType::SOLID186:
		case ElementType::C3D20: return 3;
		case ElementType::UNKNOWN:
		default:                 return 0;
	}
}
/**
 *  \brief Get number of active node.
 *  \return number of active node.
 */
size_t ElementAttr::get_active_num_of_node(ElementType et) {
	switch (et) {
		case ElementType::MASS21: return 1;
		case ElementType::COMBIN14:
		case ElementType::PIPE16:
		case ElementType::BEAM188:
		case ElementType::B31:
		case ElementType::PIPE18: return 2;
		case ElementType::BEAM189:
		case ElementType::B32:
		case ElementType::S3R:    return 3;
		case ElementType::C3D4:
		case ElementType::SHELL181:
		case ElementType::S4R:    return 4;
		case ElementType::SOLID185:
		case ElementType::C3D8:
		case ElementType::SHELL281:
		case ElementType::S8R:    return 8;
		case ElementType::S9R:    return 9;
		case ElementType::SOLID186:
		case ElementType::C3D20:  return 20;
		case ElementType::UNKNOWN:
		default:                  return 0;
	}
}
/**
 *  \brief Get order of element.
 *  \return order of element.
 */
size_t ElementAttr::get_element_order(ElementType et) {
	switch (et) {
		case ElementType::PIPE16:
		case ElementType::BEAM188:
		case ElementType::B31:
		case ElementType::PIPE18:
		case ElementType::C3D4:
		case ElementType::SOLID185:
		case ElementType::C3D8:
		case ElementType::S3R:
		case ElementType::S4R:
		case ElementType::SHELL181: return 1;
		case ElementType::BEAM189:
		case ElementType::B32:
		case ElementType::SHELL281:
		case ElementType::S8R:
		case ElementType::S9R:
		case ElementType::SOLID186:
		case ElementType::C3D20:    return 2;
		case ElementType::MASS21:
		case ElementType::COMBIN14:
		case ElementType::UNKNOWN:
		default:                    return 0;
	}
}
/**
 *  \brief Get id of element type.
 *  \return id of element type.
 */
size_t ElementAttr::get_element_type_id(ElementType et) {
	switch (et) {
		case ElementType::PIPE16:   return 16;
		case ElementType::BEAM188:
		case ElementType::B31:      return 188;
		case ElementType::PIPE18:   return 18;
		case ElementType::C3D8:
		case ElementType::SOLID185: return 185;
		case ElementType::S4R:
		case ElementType::SHELL181: return 181;
		case ElementType::BEAM189:
		case ElementType::B32:      return 189;
		case ElementType::S8R:
		case ElementType::SHELL281: return 281;
		case ElementType::SOLID186: return 186;
		case ElementType::C3D20:    return 20;
		case ElementType::MASS21:   return 21;
		case ElementType::COMBIN14: return 14;
		case ElementType::S3R:
		case ElementType::S9R:
		case ElementType::C3D4:
		case ElementType::UNKNOWN:
		default:                    return 0;
	}
}
}  // namespace cafea
