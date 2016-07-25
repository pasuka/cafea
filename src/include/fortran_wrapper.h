#ifndef FORTRAN_WRAPPER_H
#define FORTRAN_WRAPPER_H

namespace cafea
{
namespace wrapper_ 
{
#ifdef __cplusplus
extern "C" {
#endif
const int LEN_ARRAY_{16};
const int MAX_NODES_PER_ELEM_{27};

struct node_f {
	int id_{-1};
	int csys_{0};
	// int boundary_[LEN_ARRAY_]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	REAL4 xyz_[3]={0E0, 0E0, 0E0};
	REAL4 rot_[3]={1.81E2, 1.81E2, 1.81E2};
};

struct elem_f {
	int id_{-1};
	int etype_{0};
	int prop_[2]={0, 0};
	int opt_[LEN_ARRAY_]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int node_list_[MAX_NODES_PER_ELEM_]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
};

struct matl_f {
	int id_{-1};
	int sect_{0};
	REAL4 val_[LEN_ARRAY_]={0E0,0E0,0E0,0E0,0E0,0E0,0E0,0E0,0E0,0E0,0E0,0E0,0E0,0E0,0E0,0E0};
};
void load_cdb_file(const char*, int);
void model_data_clean();
void model_data_ptr(node_f**, elem_f**, matl_f**, matl_f**, int*, int*, int*, int*);
void test_multi_f();
#ifdef __cplusplus
}
#endif
}
}
#endif