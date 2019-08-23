/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#ifndef CAFEA_ELEMENT_LIB_HPP_
#define CAFEA_ELEMENT_LIB_HPP_

#include <map>
#include <tuple>
#include <string>
#include <vector>
#include <type_traits>

#include "cafea/base/node.hpp"
#include "cafea/base/load.hpp"
#include "cafea/base/section.hpp"
#include "cafea/base/material.hpp"

namespace cafea {
//! Define out variables.
template <class T>
using elem_out_5 = std::tuple<matrix_<T>, matrix_<T>, matrix_<T>, vecX_<T>, std::map<std::string, T>>;
/**
 * \brief Interface for structural elements.
 */
template <class T = REAL4, class U = REAL8>
struct StructuralElement {
	static_assert(std::is_floating_point<T>::value, "StructuralElement<T, U>: T must be floating type.");
	static_assert(std::is_floating_point<U>::value, "StructuralElement<T, U>: U must be floating type.");
	/**
	 *  \brief 2-node straight pipe element.
	 */
	static elem_out_5<U> pipe16(const NodeBase<T>*, const NodeBase<T>*, const Material<T>*, const Section<T>*, const int*);

	static elem_out_5<U> pipe16(const std::vector<Node<T, U>>, const Material<T>*, const Section<T>*, const int*);
	/**
	 *  \brief 2-node elbow pipe element.
	 */
	static elem_out_5<U> pipe18(const NodeBase<T>*, const NodeBase<T>*, const NodeBase<T>*, const Material<T>*, const Section<T>*);
	static elem_out_5<U> pipe18(const std::vector<Node<T, U>>, const Material<T>*, const Section<T>*);

	/**
	 *  \brief 1-node mass element.
	 */
	static elem_out_5<U> mass21(const NodeBase<T>*, const Material<T>*, const Section<T>*, const int[]);
	static elem_out_5<U> mass21(const std::vector<Node<T, U>>, const Material<T>*, const Section<T>*, const int[]);
	/**
	 *  \brief 2-node spring element.
	 */
	static elem_out_5<U> combin14(const NodeBase<T>*, const NodeBase<T>*, const Material<T>*, const Section<T>*, const int[]);
	static elem_out_5<U> combin14(const std::vector<Node<T, U>>, const Material<T>*, const Section<T>*, const int[]);
	/**
	 *  \brief 8-node Mindlin shell element.
	 */
	static elem_out_5<U> shell8r(const NodeBase<T>*, const NodeBase<T>*, const NodeBase<T>*, const NodeBase<T>*,
								 const NodeBase<T>*, const NodeBase<T>*, const NodeBase<T>*, const NodeBase<T>*,
								 const Material<T>*, const Section<T>*);
	static elem_out_5<U> shell8r(const std::vector<Node<T, U>>, const Material<T>*, const Section<T>*);
	/**
	 * \ brief 9-node Mindlin shell elment.
	 */
	static elem_out_5<U> shell9r(const NodeBase<T>*, const NodeBase<T>*, const NodeBase<T>*, const NodeBase<T>*,
								 const NodeBase<T>*, const NodeBase<T>*, const NodeBase<T>*, const NodeBase<T>*,
								 const NodeBase<T>*, const Material<T>*, const Section<T>*);
	static elem_out_5<U> shell9r(const std::vector<Node<T, U>>, const Material<T>*, const Section<T>*);
};
/**
 *  \brief Interface for structural element post process.
 */
template <class T = REAL8>
struct StructuralElementPost {
	static_assert(std::is_floating_point<T>::value, "StructuralElementPost<T>: T must be floating number.");
	/**
	 *  \brief 2-node straight/elbow pipe element post process.
	 */
	static matrix_<T> pipe(const matrix_<T> stif, const matrix_<T> tran, const matrix_<T> x,
						   const matrix_<T> rhs, const std::map<std::string, T> attr,
						   bool is_pres = false, T pres_in = T(0), T pres_out = T(0));
	/**
	 *  \brief 2-node straight/elbow pipe element post process in complex domain.
	 */
	static cmatrix_<T> pipe_cmplx(const matrix_<T> stif, const matrix_<T> tran,
								  const cmatrix_<T> x, const cmatrix_<T> rhs,
								  const cmatrix_<T> load, const std::map<std::string, T> attr);
};
// //!< Specialization.
template struct StructuralElement<REAL4, REAL4>;
template struct StructuralElement<REAL4, REAL8>;
template struct StructuralElement<REAL8, REAL4>;
template struct StructuralElement<REAL8, REAL8>;

template struct StructuralElementPost<REAL4>;
template struct StructuralElementPost<REAL8>;
}  // namespace cafea
#endif  // CAFEA_ELEMENT_LIB_HPP_
