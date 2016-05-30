#include "cafea.h"

int main(int argc, char* argv[])
{
	using std::string;
	//! Import fortran wrapper.
	using cafea::wrapper_::model_data_ptr;
	using cafea::wrapper_::model_data_clean;
	using cafea::wrapper_::load_cdb_file;
	using cafea::wrapper_::test_multi_f;
	using cafea::wrapper_::node_f;
	using cafea::wrapper_::elem_f;
	using cafea::wrapper_::matl_f;
	
	using cafea::AnsysCdbReader;
	
	AnsysCdbReader<float> cdb_reader;
	cdb_reader.load_model("./ansys/pipe_demo_02.cdb");
	cdb_reader.clean_model();
	std::cout << cdb_reader;
	AnsysCdbReader<> cdb2;
	cdb2.load_model(std::string("./ansys/pipe_demo_03.cdb"));
	cdb2.clean_model();
	std::cout << cdb2;
	//! Check cmd input.
	for(int i=0;i<argc;i++)fmt::print("Input:{0}\n", argv[i]);
	string aa(argv[0]);
	fmt::print("{0}", aa);
	//! Define example file name.
	string s2("./ansys/pipe_demo_01.cdb");
	//! Call fortran subroutine.
	load_cdb_file(s2.c_str(), s2.size());
	//! Define local variables.
	node_f *p_node{nullptr};
	elem_f *p_elem{nullptr};
	matl_f *p_real{nullptr}, *p_matl{nullptr};
	int a1, a2, a3, a4;
	//! Get data raw pointer.
	model_data_ptr(&p_node, &p_elem, &p_real, &p_matl, &a1, &a2, &a3, &a4);
	//! Print necessary info.
	fmt::print("*************************************************\n");
	fmt::print("Number of nodes:{}\nNumber of elements:{}\n", a1, a2);
	fmt::print("Number of materials:{}\nNumber of real constants:{}\n", a3, a4);
	fmt::print("*************************************************\n");
#ifndef PRINT_ON
#define PRINT_ON 0
#endif
#if(PRINT_ON==1)	
	for(int i=0; i<a1; i++, p_node++){
		fmt::printf("Node id:%d xyz: %f %f %f\n",
			p_node->id_, p_node->xyz_[0], p_node->xyz_[1], p_node->xyz_[2]);
	}
#endif
#if(PRINT_ON==2)
	for(int i=0; i<a2; i++, p_elem++){
		fmt::printf("Element id:%d type: %d\n", p_elem->id_, p_elem->etype_);
	}
#endif
	//! Clear.
	model_data_clean();
	//! Run another case.
	test_multi_f();
	//! Clear.
	model_data_clean();
}