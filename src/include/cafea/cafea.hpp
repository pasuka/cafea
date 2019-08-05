/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#ifndef CAFEA_H_
#define CAFEA_H_

#include "cafea/utils.h"
#include "cafea/enum_lib.h"
#include "cafea/base.h"
#include "cafea/timer.h"
#include "cafea/boundary.h"
#include "cafea/node.h"
#include "cafea/element.h"
#include "cafea/material.h"
#include "cafea/section.h"
#include "cafea/load.h"
#include "cafea/sparse_matrix.h"
#include "cafea/fortran_wrapper.h"
#include "cafea/mesh_reader.h"
#include "cafea/solution.h"
#include "cafea/element_lib.h"
#include "cafea/eigenpair.h"

#ifndef PRINT_ON
#define PRINT_ON 0
#endif
#endif  // CAFEA_H_
