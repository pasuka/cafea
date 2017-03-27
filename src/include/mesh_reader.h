#ifndef MESH_READER_H
#define MESH_READER_H

#include <map>
#include <string>
#include <fstream>
#include <utility>
#include <iostream>
#include <algorithm>
#include <functional>
// #include <experimental/filesystem>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include "fmt/format.h"

#include "fortran_wrapper.h"

namespace fs = boost::filesystem;//!< C++17 not ready yet.

namespace cafea
{
/**
 *  \class FE model reader.
 */
template <class T=REAL4>
class FEModelReader{
	public:
		//! A constructor.
		FEModelReader() {};
		//! Destructor.
		~FEModelReader() { clean_model();};
		//! Load bcy file.
		int load_model(const std::string fn);
		//! Load bcy file.
		int load_model(const char* fn)
		{
			const std::string fn2(fn);
			return load_model(fn2);
		};
		//! Clear model data in memory.
		void clean_model()
		{
			file_.clear();
			if(fp_.is_open())fp_.close();
			if(!node_list_.empty())node_list_.clear();
			if(!elem_list_.empty())elem_list_.clear();
			if(!matl_list_.empty())matl_list_.clear();
			if(!sect_list_.empty())sect_list_.clear();
			if(!solu_list_.empty())solu_list_.clear();
			if(!bc_list_.empty())bc_list_.clear();

			if(!load_list_.empty()) {
				for(auto &x: load_list_) {
					if(!x.empty())x.clear();
				}
				load_list_.clear();
			}
		};
		//! Check and print.
		std::map<std::string, int> print_info()
		{
			int num_node = node_list_.size();
			int num_elem = elem_list_.size();
			int num_matl = matl_list_.size();
			int num_sect = sect_list_.size();
			int num_load = load_list_.size();
			int num_solu = solu_list_.size();
			int num_bc = bc_list_.size();
			fmt::print("Num. node: {}\n", num_node);
			fmt::print("Num. element: {}\n", num_elem);
			fmt::print("Num. material: {}\n", num_matl);
			fmt::print("Num. section: {}\n", num_sect);
			fmt::print("Num. boundary: {}\n", num_bc);
			fmt::print("Num. load: {}\n", num_load);
			fmt::print("Num. solution: {}\n", num_solu);

			std::map<std::string, int>  tmp{{"node", num_node}, {"element", num_elem},
				{"material", num_matl}, {"section", num_sect}, {"load", num_load},
				{"solution", num_solu}, {"boundary", num_bc}};
			return tmp;
		};
		using namespace wrapper_;
		//! Get node list pointer.
		const node_f03* get_node_ptr() const { return node_list_.data();};
		//! Get element list pointer.
		const elem_f03* get_element_ptr() const { return elem_list_.data();};
		//! Get material list pointer.
		const matl_f03* get_material_ptr() const { return matl_list_.data();};
		//! Get section list pointer.
		const sect_f03* get_section_ptr() const { return sect_list_.data();};
		//! Get boundary list pointer.
		const bndy_f03* get_boundary_ptr() const { return bc_list_.data();};
		//! Get solution list pointer.
		const solu_f03* get_solution_ptr() const { return solu_list_.data();};
		//! Get load list pointer.
		const std::vector<load_f03>* get_load_ptr() const { return load_list_.data();};
	private:
		fs::path file_;//!< input file path.
		std::ifstream fp_;//!< file handler.
		std::vector<node_f03> node_list_;
		std::vector<elem_f03> elem_list_;
		std::vector<matl_f03> matl_list_;
		std::vector<sect_f03> sect_list_;
		std::vector<solu_f03> solu_list_;
		std::vector<bndy_f03> bc_list_;
		std::vector<std::vector<load_f03>> load_list_;

		//! Split line by delimer.
		std::vector<std::string> parse_line(std::string line, std::string delim=",")
		{
			std::vector<std::string> list;
			boost::trim(line);
			boost::split(list, line, boost::is_any_of(delim));
			return list;
		};
		//! Read node block.
		int parse_node_blk();
		//! Read element block.
		int parse_element_blk();
		//! Read material block.
		int parse_material_blk();
		//! Read section block.
		int parse_section_blk();
		//! Read boundary block.
		int parse_boundary_blk();
		//! Read solution block.
		int parse_solution_blk();
		//! Read load block.
		int parse_load_blk();
};
//! Specialization.
template class FEModelReader<REAL4>;
template class FEModelReader<REAL8>;
}
#endif
