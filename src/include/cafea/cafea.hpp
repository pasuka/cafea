/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#ifndef CAFEA_HPP_
#define CAFEA_HPP_

#include "cafea/utils/utils.hpp"
#include "cafea/base/enum_lib.hpp"
#include "cafea/base/base.hpp"
#include "cafea/utils/timer.hpp"
#include "cafea/base/boundary.hpp"
#include "cafea/base/node.hpp"
#include "cafea/element/element.hpp"
#include "cafea/base/material.hpp"
#include "cafea/base/section.hpp"
#include "cafea/base/load.hpp"
#include "cafea/base/sparse_matrix.hpp"
#include "cafea/fortran/fortran_wrapper.hpp"
#include "cafea/io/mesh_reader.hpp"
#include "cafea/core/solution.hpp"
#include "cafea/element/element_lib.hpp"
#include "cafea/base/eigenpair.hpp"

#ifndef PRINT_ON
#define PRINT_ON 0
#endif
#endif  // CAFEA_HPP_
