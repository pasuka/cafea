#include <cassert>
#include <iostream>

#include <cafea.h>

namespace cafea
{
/**
 *  Get number of active nodes.
 */
template <class T, class U>
int Element<T, U>::get_active_num_of_nodes()const
{
	switch(this->etype_) {
	case ElementType::MASS21: return 1;
	case ElementType::COMBIN14:
	case ElementType::PIPE16:
	case ElementType::BEAM188:
	case ElementType::B31:
	case ElementType::PIPE18: return 2;
	case ElementType::BEAM189:
	case ElementType::B32:
	case ElementType::S3R: return 3;
	case ElementType::C3D4:
	case ElementType::SHELL181:
	case ElementType::S4R: return 4;
	case ElementType::SOLID185:
	case ElementType::C3D8:
	case ElementType::SHELL281:
	case ElementType::S8R: return 8;
	case ElementType::S9R: return 9;
	case ElementType::SOLID186:
	case ElementType::C3D20: return 20;
	case ElementType::UNKNOWN:
	default: return 0;
	}
}
/**
 *  Get dofs of each node.
 */
template <class T, class U>
int Element<T, U>::get_dofs_per_node()const
{
	switch(this->etype_){
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
	case ElementType::S9R: return 6;
	case ElementType::C3D4:
	case ElementType::SOLID185:
	case ElementType::C3D8:
	case ElementType::SOLID186:
	case ElementType::C3D20: return 3;
	case ElementType::UNKNOWN:
	default: return 0;
	}	
}
/**
 *  Get order of shape function of element.
 */
template <class T, class U>
int Element<T, U>::get_element_order()const
{
	switch(this->etype_){
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
	case ElementType::C3D20: return 2;
	case ElementType::MASS21:
	case ElementType::COMBIN14:
	case ElementType::UNKNOWN:
	default: return 0;
	}	
}
}