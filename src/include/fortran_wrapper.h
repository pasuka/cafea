#ifndef FORTRAN_WRAPPER_H
#define FORTRAN_WRAPPER_H

#include "utils.h"

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

template <class Scalar, class ResultScalar>
Node<Scalar, ResultScalar> convert2node(const node_f*);
template <class Scalar, class ResultScalar>
Element<Scalar, ResultScalar> convert2elem(const elem_f*);
template <class Scalar>
Material<Scalar> convert2matl(const matl_f*);
template <class Scalar>
Section<Scalar> convert2sect(const matl_f*);

//! Specialization
extern template Node<REAL4, REAL8> convert2node(const node_f*);
extern template Element<REAL8, REAL8> convert2elem(const elem_f*);
extern template Material<REAL4> convert2matl(const matl_f*);
extern template Section<REAL4> convert2sect(const matl_f*);
}
}
#endif