#ifndef ELEMENT_LIB_H
#define ELEMENT_LIB_H

#include <map>
#include <tuple>
#include <array>
#include <vector>
#include <string>

#include "node.h"
#include "material.h"
#include "section.h"
#include "load.h"

namespace cafea
{
namespace
{
using std::map;
using std::tuple;
using std::array;
using std::vector;
using std::string;

template <class T>
using var_out_5 = tuple<matrix_<T>, matrix_<T>, matrix_<T>, vecX_<T>, map<string, T>>;
}

/**
 * \brief Interface for structural elements. 
 */
template <class T=float, class U=double>
struct StructuralElement{
	/**
	 *  \brief 2-node straight pipe element.
	 */
	static var_out_5<U> pipe16(const NodeBase<T>*, const NodeBase<T>*, const Material<T>*,
		const Section<T>*, const int*);
	
	static var_out_5<U> pipe16(const vector<Node<T, U>>, const Material<T>*, const Section<T>*,
		const int*);
	
	/**
	 *  \brief 2-node elbow pipe element.
	 */
	static var_out_5<U> pipe18(const NodeBase<T>*, const NodeBase<T>*, const NodeBase<T>*,
		const Material<T>*, const Section<T>*);
	static var_out_5<U> pipe18(const vector<Node<T, U>>, const Material<T>*, const Section<T>*);
	
	/**
	 *  \brief 1-node mass element.
	 */
	static var_out_5<U> mass21(const NodeBase<T>*, const Material<T>*, const Section<T>*,
		const int[]);
	static var_out_5<U> mass21(const vector<Node<T, U>>, const Material<T>*, const Section<T>*,
		const int[]);
	/**
	 *  \brief 2-node spring element.
	 */
	static var_out_5<U> combin14(const NodeBase<T>*, const NodeBase<T>*, const Material<T>*,
		const Section<T>*, const int[]);
	static var_out_5<U> combin14(const vector<Node<T, U>>, const Material<T>*, const Section<T>*,
		const int[]);
	/**
	 *  \brief 8-node Mindlin shell element.
	 */
	static var_out_5<U> shell8r(
		const NodeBase<T>*, const NodeBase<T>*, const NodeBase<T>*, const NodeBase<T>*,
		const NodeBase<T>*, const NodeBase<T>*, const NodeBase<T>*,	const NodeBase<T>*,
		const Material<T>*, const Section<T>*);
	static var_out_5<U> shell8r(const vector<Node<T, U>>, const Material<T>*, const Section<T>*);
	/**
	 * \ brief 9-node Mindlin shell elment.
	 */
	static var_out_5<U> shell9r(
		const NodeBase<T>*, const NodeBase<T>*, const NodeBase<T>*, const NodeBase<T>*,
		const NodeBase<T>*, const NodeBase<T>*, const NodeBase<T>*,	const NodeBase<T>*,
		const NodeBase<T>*, const Material<T>*, const Section<T>*);
	static var_out_5<U> shell9r(const vector<Node<T, U>>, const Material<T>*, const Section<T>*);
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
	static matrix_<T> pipe(const matrix_<T> stif, const matrix_<T> tran, const matrix_<T> x,
		const matrix_<T> rhs, const map<string, T> attr, bool is_pres=false, T pres_in=T(0), T pres_out=T(0));
	/**
	 *
	 */
	static matrix_<COMPLEX<T>> pipe_cmplx(const matrix_<T> stif, const matrix_<T> tran, 
		const matrix_<COMPLEX<T>> x, const matrix_<COMPLEX<T>> rhs, const matrix_<COMPLEX<T>> load,
		const map<string, T> attr);
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