#include <iostream>

#include "cafea.h"

namespace cafea
{
/**
 *  Constraint dofs of node.
 */
void Node::constraint_dofs(BoundaryType bc)
{
	auto len = this->get_dofs_size();
	switch(bt){
	case BoundaryType::FIX_ALL:
		assert(len>0);
		for(auto &x: dofs)x = -1;
		break;
	case BoundaryType::FIX_U_ALL:
		assert(len>2);
		for(auto i: {0, 1, 2})dofs[i] = -1;
		break;
	case BoundaryType::FIX_UR_ALL:
		assert(len>5);
		for(auto i: {3, 4, 5})dofs[i] = -1;
		break;
	case BoundaryType::FIX_UX:
		assert(len>0);
		dofs[0] = -1;
		break;
	case BoundaryType::FIX_UY: 
		assert(len>1);
		dofs[1] = -1;
		break;
	case BoundaryType::FIX_UZ:
		assert(len>2);
		dofs[2] = -1;
		break;
	case BoundaryType::FIX_URX:
		assert(len>3);
		dofs[3] = -1;
		break;
	case BoundaryType::FIX_URY: 
		assert(len>4);
		dofs[4] = -1;
		break;
	case BoundaryType::FIX_URZ:
		assert(len>5);
		dofs[5] = -1;
		break;
	case BoundaryType::UNKNOWN:
	case default:
		std::cout << "UNKNOWN boundary type!\n";
	}
}
}