#include <cassert>

#include <Eigen/Dense>

#include "cafea.h"

namespace cafea
{
/**
 *  
 */
Eigen::Vector3i Element::get_element_extra_info()
{
	Eigen::Vector3i val = Eigen::Vector3i::Zero();
	ElementType et = this->get_element_type();
	switch(et) {
		case ElementType::MASS21:
			val <<;
		case ElementType::PIPE16:
		case ElementType::BEAM188:
		case ElementType::B31:
		
	}
	
}

/**
 *  \brief Analyze sparse pattern of FEA model.
 */
void SolutionFrequencyResponse::analyze_pattern()
{
	for(const auto &it: m_elem_dict){
		auto &tmp = it.second;
		auto node_list = tmp.get_node_list();
		assert(!node_list.empty());
	}
}
}