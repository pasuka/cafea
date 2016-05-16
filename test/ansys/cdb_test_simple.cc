#include "cafea.h"

int main(int argc, char* argv[])
{
	using cafea::test_f_interface;
	using cafea::get_model_data;
	using cafea::node_f;
	using cafea::elem_f;
	using cafea::matl_f;
	
	for(int i=0;i<argc;i++)fmt::print("Input:{0}\n", argv[i]);
	
	test_f_interface("This is a test case!", 15);
	node_f *p_node{nullptr};
	elem_f *p_elem{nullptr};
	matl_f *p_real{nullptr}, *p_matl{nullptr};
	int a1, a2, a3, a4;
	get_model_data(&p_node, &p_elem, &p_real, &p_matl, &a1, &a2, &a3, &a4);
	fmt::print("*************************************************\n");
	fmt::print("Number of nodes:{0}\nNumber of elements:{1}\n", a1, a2);
	for(int i=0; i<a1; i++, p_node++)fmt::printf("Node id:%d xyz:%f %f %f\n", p_node->id_, p_node->xyz_[0], p_node->xyz_[1], p_node->xyz_[2]);
	
}