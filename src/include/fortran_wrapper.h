#ifndef FORTRAN_WRAPPER_H
#define FORTRAN_WRAPPER_H

#include "base.h"
#include "boundary.h"
#include "node.h"
#include "element.h"
#include "material.h"
#include "section.h"
#include "load.h"

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
	int boundary_[LEN_ARRAY_]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	REAL4 xyz_[3]={0E0, 0E0, 0E0};
	REAL4 rot_[3]={1.81E2, 1.81E2, 1.81E2};
};

struct elem_f {
	int id_{-1};
	int etype_{0};
	int prop_[2]={0, 0};
	int opt_[LEN_ARRAY_]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int node_list_[MAX_NODES_PER_ELEM_]{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
};

struct matl_f {
	int id_{-1};
	int sect_{0};
	REAL4 val_[LEN_ARRAY_]{0E0, 0E0, 0E0, 0E0, 0E0, 0E0, 0E0, 0E0, 0E0, 0E0,
		0E0, 0E0, 0E0, 0E0, 0E0, 0E0};
};
void load_cdb_file(const char*, int);
void model_data_clean();
void model_data_ptr(node_f**, elem_f**, matl_f**, matl_f**, int*, int*, int*,
	int*);
void test_multi_f();

//! Definition for BCY format file.
struct node_bcy {
	int id_{-1};
	int csys_{0};
	REAL4 xyz_[3]{0E0, 0E0, 0E0};
	REAL4 rot_[3]{1.81E2, 1.81E2, 1.81E2};
};
struct elem_bcy {
	int id_{-1};
	int etype_{-1};
	int mtype_{-1};
	int stype_{-1};
	int node_list_[MAX_NODES_PER_ELEM_]{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
};
struct matl_bcy {
	int id_{-1};
	int tp_{-1};
	REAL4 val_[LEN_ARRAY_]={0E0, 0E0, 0E0, 0E0, 0E0, 0E0, 0E0, 0E0, 0E0, 0E0,
		0E0, 0E0, 0E0, 0E0, 0E0, 0E0}; 
};
struct sect_bcy {
	int id_{-1};
	int tp_{-1};
	REAL4 val_[LEN_ARRAY_]={0E0, 0E0, 0E0, 0E0, 0E0, 0E0, 0E0, 0E0, 0E0, 0E0,
		0E0, 0E0, 0E0, 0E0, 0E0, 0E0};
};
struct bndy_bcy {
	int id_{-1};
	int bc_type_{-1};
	REAL4 val_[LEN_ARRAY_]={0E0, 0E0, 0E0, 0E0, 0E0, 0E0, 0E0, 0E0, 0E0, 0E0,
		0E0, 0E0, 0E0, 0E0, 0E0, 0E0};
};
struct load_bcy {
	int id_{-1};
	int load_type_{-1};
	REAL4 val_[LEN_ARRAY_]={0E0, 0E0, 0E0, 0E0, 0E0, 0E0, 0E0, 0E0, 0E0, 0E0,
		0E0, 0E0, 0E0, 0E0, 0E0, 0E0};
};
void load_bcy_file(const char*, int);
void model_data_clean_bcy();
void model_data_ptr_bcy(node_bcy**, elem_bcy**, matl_bcy**, sect_bcy**,
	bndy_bcy**, load_bcy**, int*, int*, int*, int*, int*, int*);

#ifdef __cplusplus
}
#endif
template <class Scalar=float, class ResultScalar=double>
struct AdapterF2Cpp{
	static Node<Scalar, ResultScalar> bcy2node(const node_bcy*);
	static Element<ResultScalar> bcy2elem(const elem_bcy*);
	static Material<Scalar> bcy2matl(const matl_bcy*);
	static Section<Scalar> bcy2sect(const sect_bcy*);
	static Boundary<Scalar> bcy2bndy(const bndy_bcy*);
	static Load<Scalar> bcy2load(const load_bcy*);
	
	static Node<Scalar, ResultScalar> cdb2node(const node_f*);
	static Element<ResultScalar> cdb2elem(const elem_f*);
	static Material<Scalar> cdb2matl(const matl_f*);
	static Section<Scalar> cdb2sect(const matl_f*);
};

//! Specialization.
template struct AdapterF2Cpp<REAL4, REAL4>;
template struct AdapterF2Cpp<REAL4, REAL8>;
template struct AdapterF2Cpp<REAL8, REAL8>;
template struct AdapterF2Cpp<REAL8, REAL4>;
}
}
#endif