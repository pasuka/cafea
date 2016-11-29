#ifndef MESH_READER_H
#define MESH_READER_H

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
#include "fmt/printf.h"

#include "fortran_wrapper.h"

namespace fs = boost::filesystem;

namespace cafea
{
/**
 *  CDB file reader
 */
template <class T=float>
class AnsysCdbReader{
	public:
		//! A constructor.
		AnsysCdbReader(){};
		/**
		 *  \brief Initilize with file path.
		 *  \param[in] fn cdb file path.
		 */
		AnsysCdbReader(const std::string fn):file_(fn){};
		//! Initilize with file path.
		AnsysCdbReader(const char *fn):file_(fn){};
		//! Destructor.
		~AnsysCdbReader() { clean_model();};
		//! Check model data.
		int check_model();
		//! Load cdb file.
		int load_model(const std::string fn) 
		{
			file_ = fn;
			wrapper_::load_cdb_file(fn.c_str(), fn.size());
			return 0;
		};
		//! Load cdb file.
		int load_model(const char* fn)
		{
			file_ = std::string(fn);
			wrapper_::load_cdb_file(file_.c_str(), file_.size());
			return 0;
		};
		void print_info() {fmt::print("This ANSYS cdb file reader.\n");};
		//! Print model information.
		friend std::ostream& operator<<(std::ostream& cout, const AnsysCdbReader &a)
		{
			cout << "File: " << a.file_;
			return cout << ".\n";
		};
		// //! Clear model data in memory.
		// void clean_model() { wrapper_::model_data_clean();};
		//! Clear model data in memory.
		std::function<void()> clean_model = std::bind(wrapper_::model_data_clean);
		//! Get model data pointer.
		std::function<void(wrapper_::node_f**, wrapper_::elem_f**,
			wrapper_::matl_f**, wrapper_::matl_f**, int*, int*, int*,
			int*)> get_data_ptr = std::bind(wrapper_::model_data_ptr,
			std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
			std::placeholders::_4, std::placeholders::_5, std::placeholders::_6,
			std::placeholders::_7, std::placeholders::_8);
	protected:
		std::string file_;
};	
/**
 *  \brief BCY format file reader.
 */
template <class T=float>
class BcyReader{
	public:
		//! A constructor.
		BcyReader(){};
		//! Destructor.
		~BcyReader() {clean_model();};
		//! Load bcy file.
		int load_model(const std::string fn) 
		{
			file_ = fn;
			wrapper_::load_bcy_file(fn.c_str(), fn.size());
			return 0;
		};
		//! Load bcy file.
		int load_model(const char* fn)
		{
			file_ = std::string(fn);
			wrapper_::load_bcy_file(file_.c_str(), file_.size());
			return 0;
		};
		//! 
		void print_info() {fmt::print("This is Fortran bcy file reader.\n");};
		//! Clear model data in memory.
		std::function<void()> clean_model = std::bind(wrapper_::model_data_clean_bcy);
		//! Get model data pointer.
		std::function<void(wrapper_::node_bcy**, wrapper_::elem_bcy**,
			wrapper_::matl_bcy**, wrapper_::sect_bcy**, wrapper_::bndy_bcy**,
			wrapper_::load_bcy**, int*, int*, int*, int*, int*,
			int*)> get_data_ptr = std::bind(wrapper_::model_data_ptr_bcy,
			std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
			std::placeholders::_4, std::placeholders::_5, std::placeholders::_6,
			std::placeholders::_7, std::placeholders::_8, std::placeholders::_9,
			std::placeholders::_10, std::placeholders::_11, std::placeholders::_12);
	private:
		std::string file_;
};
//! simplest load set.
struct load_simple{
	int id_{-1};
	int type_{-1};
	int dof_{-1};
	int group_{-1};
	float range_{0.0f};
	float val_[2]={0.0f, 0.0f};
};

//! Simplest solution parameter set.
struct solu_simple{
	int antype_{-1};
	int num_step{-1};
	float damp_[2]={0.0f, -1.0f};
};
/**
 *
 */
template <class T=float>
class FEModelReader{
	public:
		//! A constructor.
		FEModelReader(){};
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
			if(!bc_list_.empty())bc_list_.clear();
			
			if(!solu_list_.empty())solu_list_.clear();
			if(!load_list_.empty()){
				for(auto &x: load_list_){
					if(!x.empty())x.clear();
				}
				load_list_.clear();
			}
		};
		//! Check and print.
		void print_info()
		{
			fmt::print("Num. node: {}\n", node_list_.size());
			fmt::print("Num. element: {}\n", elem_list_.size());
			fmt::print("Num. material: {}\n", matl_list_.size());
			fmt::print("Num. section: {}\n", sect_list_.size());
			fmt::print("Num. boundary: {}\n", bc_list_.size());
			fmt::print("Num. load: {}\n", load_list_.size());
			fmt::print("Num. solution: {}\n", solu_list_.size());
		};
	private:
		fs::path file_;
		std::ifstream fp_;
		std::vector<wrapper_::node_bcy> node_list_;
		std::vector<wrapper_::elem_bcy> elem_list_;
		std::vector<wrapper_::matl_bcy> matl_list_;
		std::vector<wrapper_::sect_bcy> sect_list_;
		std::vector<wrapper_::bndy_bcy> bc_list_;
		
		std::vector<std::vector<load_simple>> load_list_;
		std::vector<solu_simple> solu_list_;
		//! Split line by delimer.
		std::vector<std::string> parse_line(std::string line, std::string delim = ",")
		{
			std::vector<std::string> list;
			boost::trim(line);
			boost::split(list, line, boost::is_any_of(delim));
			return list;
		};
		//! 
		int parse_node_blk();
		//!
		int parse_element_blk();
		//!
		int parse_material_blk();
		//!
		int parse_section_blk();
		//!
		int parse_boundary_blk();
		//!
		int parse_solution_blk();
		//!
		int parse_load_blk();
};
//! Specialization.
template class FEModelReader<REAL4>;
template class FEModelReader<REAL8>;
}
#endif