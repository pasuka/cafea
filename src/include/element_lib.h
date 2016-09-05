#ifndef ELEMENT_LIB_H
#define ELEMENT_LIB_H

#include "node.h"
#include "material.h"
#include "section.h"

namespace cafea
{
namespace
{
template <class T>
using varargout_basic = std::tuple<matrix_<T>, matrix_<T>, matrix_<T>, vecX_<T>>;
}

/**
 * \brief Interface for structural elements. 
 */
template <class T=float, class U=double>
struct StructuralElement{
	/**
	 *  \brief 2-node straight pipe element.
	 */
	static varargout_basic<U> pipe16(const NodeBase<T>*, const NodeBase<T>*,
		const Material<T>*, const Section<T>*);
	/**
	 *  \brief 2-node elbow pipe element.
	 */
	static varargout_basic<U> pipe18(const NodeBase<T>*, const NodeBase<T>*,
		const NodeBase<T>*, const Material<T>*, const Section<T>*);
	/**
	 *  \brief 1-node mass element.
	 */
	static varargout_basic<U> mass21(const NodeBase<T>*, const Material<T>*,
		const Section<T>*, const int[]);
	/**
	 *  \brief 2-node spring element.
	 */
	static varargout_basic<U> combin14(const NodeBase<T>*, const NodeBase<T>*, 
		const Material<T>*, const Section<T>*, const int[]);
};

//!< Specialization.
template struct StructuralElement<REAL4, REAL4>;
template struct StructuralElement<REAL4, REAL8>;
template struct StructuralElement<REAL8, REAL4>;
template struct StructuralElement<REAL8, REAL8>;
}
#endif