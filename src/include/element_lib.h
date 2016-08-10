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
using varargout = varargout_basic<REAL8>;
}
namespace pipe_elem_lib
{
template <class T> varargout pipe16(const NodeBase<T>*, const NodeBase<T>*,
	const Material<T>*, const Section<T>*);
template <class T> varargout pipe18(const NodeBase<T>*, const NodeBase<T>*,
	const NodeBase<T>*, const Material<T>*, const Section<T>*);
//! Specialization.
extern template varargout pipe16<REAL4>(const NodeBase<REAL4>*, const NodeBase<REAL4>*,
	const Material<REAL4>*, const Section<REAL4>*);
extern template varargout pipe16<REAL8>(const NodeBase<REAL8>*, const NodeBase<REAL8>*,
	const Material<REAL8>*, const Section<REAL8>*);
extern template varargout pipe18<REAL4>(const NodeBase<REAL4>*, const NodeBase<REAL4>*,
	const NodeBase<REAL4>*, const Material<REAL4>*, const Section<REAL4>*);
extern template varargout pipe18<REAL8>(const NodeBase<REAL8>*, const NodeBase<REAL8>*,
	const NodeBase<REAL8>*, const Material<REAL8>*, const Section<REAL8>*);
}
namespace additional_elem_lib
{
template <class T> varargout mass21(const NodeBase<T>*, const Material<T>*,
	const Section<T>*, const int[]);
template <class T> varargout combin14(const NodeBase<T>*, const NodeBase<T>*,
	const Material<T>*, const Section<T>*, const int[]);
extern template varargout mass21<REAL4>(const NodeBase<REAL4>*, const Material<REAL4>*,
	const Section<REAL4>*, const int[]);
extern template varargout mass21<REAL8>(const NodeBase<REAL8>*, const Material<REAL8>*,
	const Section<REAL8>*, const int[]);
extern template varargout combin14<REAL4>(const NodeBase<REAL4>*, const NodeBase<REAL4>*,
	const Material<REAL4>*, const Section<REAL4>*, const int[]);
extern template varargout combin14<REAL8>(const NodeBase<REAL8>*, const NodeBase<REAL8>*,
	const Material<REAL8>*, const Section<REAL8>*, const int[]);
}	
}
#endif