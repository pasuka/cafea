#ifndef MESH_READER_H
#define MESH_READER_H

#include <string>
#include <iostream>

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
		//! Destructor.
		~AnsysCdbReader() { clean_model();};
		//! Clear model data in memory.
		void clean_model() { wrapper_::model_data_clean();};
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
		}
		//! Print model information.
		friend std::ostream& operator<<(std::ostream& cout, const AnsysCdbReader &a)
		{
			cout << "File: " << a.file_ << ".\n";
			return cout;
		}
	protected:
		std::string file_;
};	
}
#endif