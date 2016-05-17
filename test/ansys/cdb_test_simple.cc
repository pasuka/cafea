#include "cafea.h"

int main(int argc, char* argv[])
{
	using cafea::wrapper_::model_data_ptr;
	using cafea::wrapper_::model_data_clean;
	using cafea::wrapper_::load_cdb_file;
	using cafea::wrapper_::test_multi_f;
	using cafea::wrapper_::node_f;
	using cafea::wrapper_::elem_f;
	using cafea::wrapper_::matl_f;
	using std::string;
	
	for(int i=0;i<argc;i++)fmt::print("Input:{0}\n", argv[i]);
	string aa(argv[0]);
	fmt::print("{0}", aa);
	string s2("./ansys/pipe_demo_01.cdb");
	load_cdb_file(s2.c_str(), s2.size());
	node_f *p_node{nullptr};
	elem_f *p_elem{nullptr};
	matl_f *p_real{nullptr}, *p_matl{nullptr};
	int a1, a2, a3, a4;
	model_data_ptr(&p_node, &p_elem, &p_real, &p_matl, &a1, &a2, &a3, &a4);
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
	model_data_clean();
	test_multi_f();
	model_data_clean();
}