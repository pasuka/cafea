#include "mesh_reader.h"

namespace cafea
{

/**
 * \brief Load input files.
 */
template <class T>
int FEModelReader<T>::load_model(const std::string fn)
{
	fs::path p = fn;
	if(fs::exists(p)){
		if(!p.empty()){
			this->file_ = p;
		}
		else{
			fmt::print("Input file: {} is empty!\n", fn);
			return -1;
		}
	}
	else{
		fmt::print("Input file: {} is not exists!\n", fn);
		return -2;
	}
	if(this->fp_.is_open())this->fp_.close();
	this->fp_.open(fn);
	
	if(this->fp_.is_open()){
		std::string line;
		fmt::print("Begin to read file: {}\n", fn);
		while(std::getline(this->fp_, line)){
			if(!line.empty()){
				if(startswith(line, "$NODE")){
					this->parse_node_blk();
				}
				else if(startswith(line, "$ELEMENT")){	
					this->parse_element_blk();
				}
				else if(startswith(line, "$FORMAT_INFO")){
					
				}
				else if(startswith(line, "$MATERIAL")){
					this->parse_material_blk();
				}
				else if(startswith(line, "$SECTION")){
					this->parse_section_blk();
				}
				else if(startswith(line, "$BOUNDARY")){
					this->parse_boundary_blk();
				}
				else if(startswith(line, "$SOLUTION")){
					this->parse_solution_blk();
				}
				else if(startswith(line, "$END")){
					
				}
				else if(startswith(line, "#")){
					fmt::print("Comment line.\n");
				}
				else{
					fmt::print("Data block.\n");
				}
			}
		}
	}
	return 0;
	
};

/**
 * \brief Parse block of nodes.
 */
template <class T>
int FEModelReader<T>::parse_node_blk()
{
	std::string line;
	std::getline(this->fp_, line);
	auto list = this->parse_line(line);
	int num_node = str2int(list[0]);
	int max_node_id = str2int(list[1]);
	int csys = str2int(list[2]);
	int has_rot = str2int(list[3]);
	fmt::print("Num. node: {} Max id: {} Coord. :{} xyz len: {}\n", num_node, max_node_id, csys, has_rot);
	for(int i=0; i<num_node; i++){
		std::getline(this->fp_, line);
		auto per_node = this->parse_line(line);
		wrapper_::node_bcy pt{str2int(per_node[0]), csys,
			{str2float(per_node[1]), str2float(per_node[2]), str2float(per_node[3])}};
		if(6==has_rot){
			pt.rot_[0] = str2float(per_node[4]);
			pt.rot_[1] = str2float(per_node[5]);
			pt.rot_[2] = str2float(per_node[6]);
		}
		this->node_list_.push_back(std::move(pt));
	}
	return 0;
};

/**
 * \brief Parse block of elements.
 */
template <class T>
int FEModelReader<T>::parse_element_blk()
{
	std::string line;
	std::getline(this->fp_, line);
	auto list = this->parse_line(line);
	int num_elem = str2int(list[0]);
	int max_elem_id = str2int(list[1]);
	fmt::print("Num. element: {} Max id: {}\n", num_elem, max_elem_id);
	for(int i=0; i<num_elem; i++){
		std::getline(this->fp_, line);
		auto per_elem = this->parse_line(line);
		wrapper_::elem_bcy pp{str2int(per_elem[0]), str2int(per_elem[1]), str2int(per_elem[2]), str2int(per_elem[3])};
		for(int j=0; j<str2int(per_elem[4]); j++)pp.node_list_[j] = str2int(per_elem[5+j]);
		this->elem_list_.push_back(std::move(pp));
	}
	return 0;
};

/**
 * \brief Parse block of materials.
 */
template <class T>
int FEModelReader<T>::parse_material_blk()
{
	std::string line;
	std::getline(this->fp_, line);
	auto list = this->parse_line(line);
	
	int num_matl = str2int(list[0]);
	int max_matl_id = str2int(list[1]);
	fmt::print("Num. material: {} Max id: {}\n", num_matl, max_matl_id);
	for(int i=0; i<num_matl; i++){
		std::getline(this->fp_, line);
		auto per_matl = this->parse_line(line);
		wrapper_::matl_bcy pm{str2int(per_matl[0]), str2int(per_matl[1])};
		for(int j=0; j<str2int(per_matl[2]); j++)pm.val_[j] = str2float(per_matl[3+j]);
		this->matl_list_.push_back(std::move(pm));
	}
	return 0;
};

/**
 * \brief Parse block of sections.
 */
template <class T>
int FEModelReader<T>::parse_section_blk()
{
	std::string line;
	std::getline(this->fp_, line);
	auto list = this->parse_line(line);
	
	int num_sect = str2int(list[0]);
	int max_sect_id = str2int(list[1]);
	fmt::print("Num. section: {} Max id: {}\n", num_sect, max_sect_id);
	for(int i=0; i<num_sect; i++){
		std::getline(this->fp_, line);
		auto per_sect = this->parse_line(line);
		wrapper_::sect_bcy ps{str2int(per_sect[0]), str2int(per_sect[1])};
		for(int j=0; j<str2int(per_sect[2]); j++)ps.val_[j] = str2float(per_sect[3+j]);
		this->sect_list_.push_back(std::move(ps));
	}
	return 0;
};

/**
 * \brief Parse block of boundary.
 */
template <class T>
int FEModelReader<T>::parse_boundary_blk()
{
	std::string line;
	std::getline(this->fp_, line);
	auto list = this->parse_line(line);
	
	int num_bc = str2int(list[0]);
	int num_dof_per = str2int(list[1]);
	fmt::print("Num. boundary: {} Max dof per node: {}\n", num_bc, num_dof_per);
	for(int i=0; i<num_bc; i++){
		std::getline(this->fp_, line);
		auto per_bc = this->parse_line(line);
		wrapper_::bndy_bcy bc{str2int(per_bc[0]), str2int(per_bc[1])};
		for(int j=0; j<str2int(per_bc[2]); j++)bc.val_[j] = str2int(per_bc[3+j]);
		this->bc_list_.push_back(std::move(bc));
	}
	return 0;
};

/**
 * \brief Parse block of loads.
 */
template <class T>
int FEModelReader<T>::parse_load_blk()
{
	std::string line;
	std::getline(this->fp_, line);
	if(startswith(line, "$FREQ")){
		auto list = this->parse_line(line);
		int step_id = str2int(list[1]);
		float freq = str2float(list[2]);
		std::vector<load_simple> tmp;
		while(true){
			std::getline(this->fp_, line);
			if(startswith(line, "$END_FREQ")){
				break;
			}
			else if(startswith(line, "$NUMPRES")){
				auto list = this->parse_line(line);
				int num_pres = str2int(list[1]);
				for(int i=0; i<num_pres; i++){
					std::getline(this->fp_, line);
					auto va = this->parse_line(line);
					load_simple pres_load{str2int(va[1]), str2int(va[3]), -1, str2int(va[2]), freq, {str2float(va[4]), str2float(va[5])}};
					tmp.push_back(std::move(pres_load));
				}
			}
			else if(startswith(line, "$NUMFORCE")){
				auto list = this->parse_line(line);
				int num_force = str2int(list[1]);
				for(int i=0; i<num_force; i++){
					std::getline(this->fp_, line);
					auto va = this->parse_line(line);
					load_simple force_load{str2int(va[1]), 1, str2int(va[2]), -1, freq, {str2float(va[3]), str2float(va[4])}};
					tmp.push_back(std::move(force_load));
				}
			}
			else if(startswith(line, "$NUMDISP")){
				auto list = this->parse_line(line);
				int num_disp = str2int(list[1]);
				for(int i=0; i<num_disp; i++){
					std::getline(this->fp_, line);
					auto va = this->parse_line(line);
					load_simple disp_load{str2int(va[1]), 2, str2int(va[2]), -1, freq, {str2float(va[3]), str2float(va[4])}};
					tmp.push_back(std::move(disp_load));
				}
			}
			else{
				
			}
		}
		if(!tmp.empty())this->load_list_.push_back(tmp);
	}
	else{
		return -1;
	}
	return 0;
};

/**
 * \brief Parse block of solution.
 */
template <class T>
int FEModelReader<T>::parse_solution_blk()
{
	std::string line;
	std::getline(this->fp_, line);
	int num_param = str2int(line);
	
	int antype{0};
	int numfreq{0};
	float damp{0.0f};
	
	for(int i=0; i<num_param; i++){
		std::getline(this->fp_, line);
		auto list = this->parse_line(line);
		if(startswith(line, "$ANTYPE")){
			antype = str2int(list[1]);
			fmt::print("Analysis type: {}\n", antype);
		}
		else if(startswith(line, "$DMPSTR")){
			damp = str2float(list[1]);
			fmt::print("Damp coeff: {}\n", damp);
		}
		else if(startswith(line, "$NUMFREQ")){
			numfreq = str2int(list[1]);
			fmt::print("Num. Freq: {}\n", numfreq);
		}
		else{
			
		}
	}
	for(int i=0; i<numfreq; i++){
		this->parse_load_blk();
	}
	solu_simple solu{antype, numfreq, {damp, -1.0f}};
	this->solu_list_.push_back(std::move(solu));
	return 0;
} 
}

// #include <vector>
// #include <string>
// #include <fstream>
// #include <iostream>
// #include <algorithm>
// #include <boost/algorithm/string.hpp>

// int main(int argc, char* argv[])
// {
	// if(1<argc){
		// std::ifstream infile(argv[1]);
		// if(infile.is_open()){
			// std::string line;
			// while(std::getline(infile, line)){
			// boost::trim(line);
			// if(!line.empty()){
				// if(std::equal(line.begin(), line.begin() + 4, "$END")){
					// std::cout << "This is END\n";
				// }
				// else if(std::equal(line.begin(), line.begin()+5, "$NODE")){
					// std::cout << "This is keyword NODE begin\n";
					// std::getline(infile, line);
					// std::vector<std::string> list;
					// boost::split(list, line, boost::is_any_of(","));
					// if(2<list.size())std::cout << "Node Num: " << list[0] << " Max Id. " << list[1] << "\n";
				// }
				// else{
					// std::cout << line << "\n";
				// }
				
				
			// }
		// }
		// }
		// else{
			// std::cout << "Could not find file: " << argv[1] << "!\n";
		// }
		
		
	// }
	// return 0;
// }