/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#ifndef CAFEA_FORTRAN_WRAPPER_H_
#define CAFEA_FORTRAN_WRAPPER_H_

#include "cafea/utils.h"

namespace cafea {
namespace wrapper_ {
#ifdef __cplusplus
extern "C" {
#endif
//! Length of parameter list.
static const int LEN_ARRAY_{16};
static const int MAX_NODES_PER_ELEM_{27};

//! Fortran and C compatible node set.
struct node_f03 {
	int id_{-1};
	int csys_{0};//!< Coordinate system.
	REAL4 xyz_[3]{0.0f, 0.0f, 0.0f};//!< X Y Z values.
	REAL4 rot_[3]{181.0f, 181.0f, 181.0f};//!< Euler angles.
};
//! Fortran and C compatible element set.
struct elem_f03 {
	int id_{-1};
	int etype_{-1};//!< Element number.
	int prop_[2]{-1, -1};//!< Property number.
	// int mtype_{-1};//!< Material number.
	// int stype_{-1};//!< Section number.
	int opt_[LEN_ARRAY_]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};//!< Element option.
	int node_list_[MAX_NODES_PER_ELEM_]{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
};
//! Fortran and C compatible material set.
struct matl_f03 {
	int id_{-1};
	int tp_{-1};//!< Material type.
	REAL4 val_[LEN_ARRAY_]{.0f, .0f, .0f, .0f, .0f, .0f, .0f, .0f, .0f, .0f,
		.0f, .0f, .0f, .0f, .0f, .0f};
};
//! Fortran and C compatible section set.
struct sect_f03 {
	int id_{-1};
	int tp_{-1};//!< Section type.
	REAL4 val_[LEN_ARRAY_]{.0f, .0f, .0f, .0f, .0f, .0f, .0f, .0f, .0f, .0f,
		.0f, .0f, .0f, .0f, .0f, .0f};
};
//! Fortran and C compatible boundary set.
struct bndy_f03 {
	int id_{-1};
	int bc_type_{-1};//!< Boundary type.
	REAL4 val_[LEN_ARRAY_]{.0f, .0f, .0f, .0f, .0f, .0f, .0f, .0f, .0f, .0f,
		.0f, .0f, .0f, .0f, .0f, .0f};
};
//! Fortran and C compatible load set.
struct load_f03 {
	int id_{-1};
	int type_{-1};
	int dof_{-1};
	int group_{-1};
	REAL4 range_{0.0f};
	REAL4 val_[2]{0.0f, 0.0f};
};
//! Fortran and C compatible solution parameter set.
struct solu_f03 {
	int antype_{-1};
	int num_step_{-1};
	int solve_method_{-1};
	REAL4 damp_[2]{0.0f, -1.0f};//!< Damp parameter.
};
//! CDB node.
// struct cdb_node {
//     int id{-1}, csys{0};
//     REAL4 xyz[3]{0.0E0, 0.0E0, 0.0E0}, rot[3]{1.81E2, 1.81E2, 1.81E2};
// };
using cdb_node = node_f03;
//! CDB element.
// struct cdb_elem {
//     int id{-1}, etype{0}, prop[2]{0, 0}, opt[LEN_ARRAY_], node_list[MAX_NODES_PER_ELEM_];
// };
using cdb_elem = elem_f03;
//! CDB material and real constants.
struct cdb_prop {
    int id_{-1}, prop_type_{-1};
    REAL4 arrb_[LEN_ARRAY_], arrb2_[LEN_ARRAY_];
};
//! CDB boundary.
struct cdb_bc {
    int id_{-1}, dof_label_[LEN_ARRAY_];
    REAL4 val_[LEN_ARRAY_], val2_[LEN_ARRAY_];
};
//! Clear CDB model container.
void cdb_reader_init();
//! Print CDB model info.
void cdb_reader_info();
//! Read CDB model.
void cdb_reader_load(const char*, int);
//! Get CDB model data pointer.
void cdb_reader_ptr(cdb_node**, cdb_elem**, cdb_prop**, cdb_prop**, cdb_bc**, int*, int*, int*, int*, int*);
#ifdef __cplusplus
}
#endif
}  // namespace wrapper_
}  // namespace cafea
#endif  // CAFEA_FORTRAN_WRAPPER_H_
