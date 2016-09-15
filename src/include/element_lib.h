#ifndef ELEMENT_LIB_H
#define ELEMENT_LIB_H

#include <map>
#include <tuple>
#include <string>

#include "node.h"
#include "material.h"
#include "section.h"

namespace cafea
{
namespace
{
using std::map;
using std::tuple;
using std::string;

template <class T>
using varargout_basic = tuple<matrix_<T>, matrix_<T>, matrix_<T>, vecX_<T>, map<string, T>>;
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
/**
 *  \brief Interface for structural element post process.
 */
template <class T=double>
struct StructuralElementPost
{
	/**
	 *  \brief 2-node straight/elbow pipe element post process.
	 */
	static matrix_<T> pipe(const matrix_<T>, const matrix_<T>, const matrix_<T>, const map<string, T>);
	//static matrix_<COMPLEX<T>> pipe(const matrix_<T>, const matrix_<T>, const matrix_<COMPLEX<T>>, const map<string, T>);
};
//!< Specialization.
template struct StructuralElement<REAL4, REAL4>;
template struct StructuralElement<REAL4, REAL8>;
template struct StructuralElement<REAL8, REAL4>;
template struct StructuralElement<REAL8, REAL8>;

template struct StructuralElementPost<REAL4>;
template struct StructuralElementPost<REAL8>;
}
#endif