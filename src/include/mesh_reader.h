#ifndef MESH_READER_H
#define MESH_READER_H

#include <string>
#include <iostream>

#include "fmt/format.h"

#include "node.h"
#include "element.h"
#include "material.h"
#include "boundary.h"
#include "fortran_wrapper.h"

namespace cafea
{
class FeaModel;
struct FileReaderBase {
	//! Load input file.
	virtual int load_model(std::string fn, FeaModel &arch) = 0;
	//! Print.
	friend std::ostream& operator<<(std::ostream& cout, const FileReaderBase &a)
	{
		cout << "This is base of mesh file reader.\n";
		return cout;
	}
};

template <class T=float>
class AnsysCdbReader{
	public:
		//! A constructor.
		AnsysCdbReader(){};
		//! Destructor.
		~AnsysCdbReader(){};
		//! Clear model data in memory.
		void clean_model(){ wrapper_::model_data_clean();};
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
			cout << fmt::format("File: {}\n", a.file_);
			return cout;
		}
	protected:
		std::string file_;
};	
}

#endif