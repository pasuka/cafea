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
const int LEN_ARRAY_{16};
const int MAX_NODES_PER_ELEM_{27};

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
	int mtype_{-1};//!< Material number.
	int stype_{-1};//!< Section number.
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
	REAL4 damp_[2]{0.0f, -1.0f};//!< Damp parameter.
};
#ifdef __cplusplus
}
#endif
}  // namespace wrapper_
}  // namespace cafea
#endif  // CAFEA_FORTRAN_WRAPPER_H_
