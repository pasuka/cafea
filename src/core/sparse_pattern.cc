#include <cassert>
#include <iostream>

#include <Eigen/Dense>

#include "cafea.h"

namespace cafea
{
/**
 *  \brief Analyze sparse pattern of FEA model.
 */
void SolutionFrequencyResponse::analyze_pattern()
{
	for(const auto &it: m_elem_dict){
		auto &tmp = it.second;
		auto node_list = tmp.get_node_list();
		assert(tmp.get_element_type()!=ElementType::UNKNOWN);
		assert(!node_list.empty());
		auto ndof = tmp.get_dofs_per_node();
		assert(ndof>0);
		auto nact = tmp.get_active_num_of_nodes();
		for(auto i=0; i<nact; i++){
			auto pt = m_node_dict.find(node_list[i]);
			if(pt!=m_node_dict.end()){
				auto &x = pt->second;
				auto len = x.get_dofs_size();
				assert(len>0);
				if(len<ndof){
					for(auto j=len; j<ndof; j++)x.append_dofs(0);
				}
			}
		}
	}
	if(m_boundary_dict.size()>0){
		for(const auto &it: m_boundary_dict){
			auto pair = m_node_dict.find(it.get_id());
			auto bc = it.get_boundary_type();
			assert(bc!=BoundaryType::UNKNOWN);
			if(pair!=m_node_dict.end()){
				auto &pt = pair->second;
				pt.constraint_dofs(bc);
			}
		}
	}
	else{
		std::cout << "None boundary!\n";
	}
	int dim{0};
	for(auto &it: m_node_dict){
		auto &pt = it.second;
		if(!pt.is_active())continue;
		int *p = pt.get_dofs_ptr();
		for(auto i=0; i<pt.get_dofs_size(); i++){
			if(p[i]==0)p[i] = dim++;
		}
	}
	std::cout << "Dimension of model: " << dim << "\n";
}
}