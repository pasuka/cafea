#include "mesh_reader.h"

namespace cafea
{

/**
 *
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
	auto cmp = [](std::string s1, std::string s2){return 0==s1.compare(0, s2.size(), s2);};
	if(this->fp_.is_open()){
		std::string line;
		while(std::getline(this->fp_, line)){
			if(!line.empty()){
				if(cmp(line, "$NODE")){
					this->parse_node_blk();
				}
				else if(cmp(line, "$ELEMENT")){	
					this->parse_element_blk();
				}
				else if(cmp(line, "$FORMAT_INFO")){
					
				}
				else if(cmp(line, "$MATERIAL")){
					this->parse_material_blk();
				}
				else if(cmp(line, "$SECTION")){
					this->parse_section_blk();
				}
				else if(cmp(line, "$BOUNDARY")){
					this->parse_boundary_blk();
				}
				else if(cmp(line, "$SOLUTION")){
					this->parse_solution_blk();
				}
				else if(cmp(line, "$END")){
					
				}
				else if(cmp(line, "#")){
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
 *
 */
template <class T>
int FEModelReader<T>::parse_node_blk()
{
	std::string line;
	std::getline(this->fp_, line);
	auto list = this->parse_line(line);
	int num_node = std::stoi(list[0]);
	int max_node_id = std::stoi(list[1]);
	int csys = std::stoi(list[2]);
	int has_rot = std::stoi(list[3]);
	// fmt::print("Num. node: {}\tMax id: {}\t Coord. :{}\t xyz len: {}\n", num_node, max_node_id, csys, has_rot);
	for(int i=0; i<num_node; i++){
		std::getline(this->fp_, line);
		auto per_node = this->parse_line(line);
		wrapper_::node_bcy pt{std::stoi(per_node[0]), csys,
			{std::stof(per_node[1]), std::stof(per_node[2]), std::stof(per_node[3])}};
		if(6==has_rot){
			pt.rot_[0] = std::stof(per_node[4]);
			pt.rot_[1] = std::stof(per_node[5]);
			pt.rot_[2] = std::stof(per_node[6]);
		}
		this->node_list_.push_back(pt);
	}
	return 0;
};
/**
 *
 */
template <class T>
int FEModelReader<T>::parse_element_blk()
{
	std::string line;
	std::getline(this->fp_, line);
	auto list = this->parse_line(line);
	auto str2i = [](std::string a){return std::stoi(a);};
	int num_elem = str2i(list[0]);
	int max_elem_id = str2i(list[1]);
	for(int i=0; i<num_elem; i++){
		std::getline(this->fp_, line);
		auto per_elem = this->parse_line(line);
		wrapper_::elem_bcy pp{str2i(per_elem[0]), str2i(per_elem[1]), str2i(per_elem[2]), str2i(per_elem[3])};
		for(int j=0; j<str2i(per_elem[4]); j++)pp.node_list_[j] = str2i(per_elem[5+j]);
		this->elem_list_.push_back(pp);
	}
}
/**
 *
 */
template <class T>
int FEModelReader<T>::parse_material_blk()
{
	std::string line;
	std::getline(this->fp_, line);
	auto list = this->parse_line(line);
	auto str2i = [](std::string a){return std::stoi(a);};
	auto str2f = [](std::string a){return std::stof(a);};
	int num_matl = str2i(list[0]);
	int max_matl_id = str2i(list[1]);
	for(int i=0; i<num_matl; i++){
		std::getline(this->fp_, line);
		auto per_matl = this->parse_line(line);
		wrapper_::matl_bcy pm{str2i(per_matl[0]), str2i(per_matl[1])};
		for(int j=0; j<str2i(per_matl[2]); j++)pm.val_[j] = str2f(per_matl[3+j]);
		this->matl_list_.push_back(pm);
	}
}
/**
 *
 */
template <class T>
int FEModelReader<T>::parse_section_blk()
{
	std::string line;
	std::getline(this->fp_, line);
	auto list = this->parse_line(line);
	auto str2i = [](std::string a){return std::stoi(a);};
	auto str2f = [](std::string a){return std::stof(a);};
	int num_sect = str2i(list[0]);
	int max_sect_id = str2i(list[1]);
	for(int i=0; i<num_sect; i++){
		std::getline(this->fp_, line);
		auto per_sect = this->parse_line(line);
		wrapper_::sect_bcy ps{str2i(per_sect[0]), str2i(per_sect[1])};
		for(int j=0; j<str2i(per_sect[2]); j++)ps.val_[j] = str2f(per_sect[3+j]);
		this->sect_list_.push_back(ps);
	}
}
/**
 *
 */
template <class T>
int FEModelReader<T>::parse_boundary_blk()
{
	
}
/**
 *
 */
template <class T>
int FEModelReader<T>::parse_load_blk()
{
	
}
/**
 *
 */
template <class T>
int FEModelReader<T>::parse_solution_blk()
{
	
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