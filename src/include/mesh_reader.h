#ifndef MESH_READER_H
#define MESH_READER_H

#include <string>
#include <iostream>
#include <functional>

#include "fortran_wrapper.h"

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
		//! Clear model data in memory.
		std::function<void()> clean_model = std::bind(wrapper_::model_data_clean_bcy);
		//! Get model data pointer.
		std::function<void(wrapper_::node_bcy**, wrapper_::elem_bcy**,
			wrapper_::matl_bcy**, wrapper_::sect_bcy**, wrapper_::bndy_bcy**,
			wrapper_::load_bcy**, int*, int*, int*, int*, int*,
			int*)> get_data_ptr_bcy = std::bind(wrapper_::model_data_ptr_bcy,
			std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
			std::placeholders::_4, std::placeholders::_5, std::placeholders::_6,
			std::placeholders::_7, std::placeholders::_8, std::placeholders::_9,
			std::placeholders::_10, std::placeholders::_11, std::placeholders::_12);
	private:
		std::string file_;
};
}
#endif