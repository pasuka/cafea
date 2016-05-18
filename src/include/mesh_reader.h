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
		int load_model();
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