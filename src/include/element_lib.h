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
using varargout_D = varargout_basic<REAL8>;
using varargout_F = varargout_basic<REAL4>;
}
namespace pipe_elem_lib
{
template <class T, class U> varargout_basic<U> pipe16(const NodeBase<T>*,
	const NodeBase<T>*, const Material<T>*, const Section<T>*);
template <class T, class U> varargout_basic<U> pipe18(const NodeBase<T>*,
	const NodeBase<T>*, const NodeBase<T>*, const Material<T>*, 
	const Section<T>*);
//! Specialization.
extern template varargout_D pipe16<REAL4, REAL8>(const NodeBase<REAL4>*, 
	const NodeBase<REAL4>*, const Material<REAL4>*, const Section<REAL4>*);
extern template varargout_D pipe16<REAL8, REAL8>(const NodeBase<REAL8>*,
	const NodeBase<REAL8>*, const Material<REAL8>*, const Section<REAL8>*);
extern template varargout_D pipe18<REAL4, REAL8>(const NodeBase<REAL4>*, 
	const NodeBase<REAL4>*, const NodeBase<REAL4>*, const Material<REAL4>*, 
	const Section<REAL4>*);
extern template varargout_D pipe18<REAL8, REAL8>(const NodeBase<REAL8>*, 
	const NodeBase<REAL8>*, const NodeBase<REAL8>*, const Material<REAL8>*, 
	const Section<REAL8>*);
	
extern template varargout_F pipe16<REAL4, REAL4>(const NodeBase<REAL4>*, 
	const NodeBase<REAL4>*, const Material<REAL4>*, const Section<REAL4>*);
extern template varargout_F pipe16<REAL8, REAL4>(const NodeBase<REAL8>*, 
	const NodeBase<REAL8>*, const Material<REAL8>*, const Section<REAL8>*);
extern template varargout_F pipe18<REAL4, REAL4>(const NodeBase<REAL4>*, 
	const NodeBase<REAL4>*, const NodeBase<REAL4>*, const Material<REAL4>*, 
	const Section<REAL4>*);
extern template varargout_F pipe18<REAL8, REAL4>(const NodeBase<REAL8>*, 
	const NodeBase<REAL8>*, const NodeBase<REAL8>*, const Material<REAL8>*,
	const Section<REAL8>*);
}
namespace additional_elem_lib
{
template <class T, class U> varargout_basic<U> mass21(const NodeBase<T>*, 
	const Material<T>*, const Section<T>*, const int[]);
template <class T, class U> varargout_basic<U> combin14(const NodeBase<T>*,
	const NodeBase<T>*, const Material<T>*, const Section<T>*, const int[]);
//! Specialization.
extern template varargout_D mass21<REAL4, REAL8>(const NodeBase<REAL4>*,
	const Material<REAL4>*, const Section<REAL4>*, const int[]);
extern template varargout_D mass21<REAL8, REAL8>(const NodeBase<REAL8>*,
	const Material<REAL8>*, const Section<REAL8>*, const int[]);
extern template varargout_D combin14<REAL4, REAL8>(const NodeBase<REAL4>*,
	const NodeBase<REAL4>*, const Material<REAL4>*, const Section<REAL4>*,
	const int[]);
extern template varargout_D combin14<REAL8, REAL8>(const NodeBase<REAL8>*,
	const NodeBase<REAL8>*, const Material<REAL8>*, const Section<REAL8>*,
	const int[]);

extern template varargout_F mass21<REAL4, REAL4>(const NodeBase<REAL4>*,
	const Material<REAL4>*, const Section<REAL4>*, const int[]);
extern template varargout_F mass21<REAL8, REAL4>(const NodeBase<REAL8>*,
	const Material<REAL8>*, const Section<REAL8>*, const int[]);
extern template varargout_F combin14<REAL4, REAL4>(const NodeBase<REAL4>*,
	const NodeBase<REAL4>*, const Material<REAL4>*, const Section<REAL4>*,
	const int[]);
extern template varargout_F combin14<REAL8, REAL4>(const NodeBase<REAL8>*,
	const NodeBase<REAL8>*, const Material<REAL8>*, const Section<REAL8>*,
	const int[]);
}	
}
#endif