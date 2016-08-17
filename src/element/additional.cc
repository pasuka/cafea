#include <tuple>
#include <cmath>
#include <cstddef>

#include <Eigen/Dense>

#include "cafea.h"

using std::tie;
using std::tuple;
using std::make_tuple;
using Eigen::MatrixXd;
using Eigen::VectorXd;

namespace cafea
{
namespace additional_elem_lib
{
namespace
{
template <class T>
using varargout = tuple<matrix_<T>, matrix_<T>, matrix_<T>, vecX_<T>>;
}
template <class T, class U>
varargout<U> mass21(const NodeBase<T> *p, const Material<T> *prop,
	const Section<T> *sect, const int opt[])
{
	matrix_<U> stif = matrix_<U>::Zero(6, 6);
	matrix_<U> mass = matrix_<U>::Zero(6, 6);
	matrix_<U> tran = matrix_<U>::Identity(6, 6);
	vecX_<U> rhs = vecX_<U>::Zero(6);
	
	//! Mass on X Y Z direction.
	//! Notice: mass element do not need transform in most cases.
	for(size_t i: {0, 1, 2})mass(i, i) = sect->get_sect_prop(SectionProp::ADDONMASS);
	
	return make_tuple(stif, mass, tran, rhs);
}

template <class T, class U>
varargout<U> combin14(const NodeBase<T> *p1, const NodeBase<T> *p2,
	const Material<T> *prop, const Section<T> *sect, const int opt[])
{
	matrix_<U> stif = matrix_<U>::Zero(12, 12);
	matrix_<U> mass = matrix_<U>::Zero(12, 12);
	matrix_<U> loc2gbl = matrix_<U>::Zero(12, 12);
	vecX_<U> rhs = vecX_<U>::Zero(12);
	
	U l;
	matrix_<U> tt = matrix_<U>::Zero(3, 3);
	tie(l, tt) = coord_tran<T, U>(p1, p2);
	
	if(opt[1]==0){
		for(size_t i: {0, 1, 2, 3})loc2gbl.block(i*3, i*3, 3, 3) = tt;
		if(opt[2]==0){
			
		}
		else if(opt[2]==1){
			
		}
		else{}
	}
	else{
		matrix_<U> tmp = matrix_<U>::Zero(3, 3);
		decltype(l) coff{PI<U>()/1.8e2};
		if(p1->get_rot(0)>1.8e2){
			loc2gbl.block(0, 0, 6, 6).setIdentity();
		}
		else{
			U cb = cos(p1->get_rot(0)*coff), sb = sin(p1->get_rot(0)*coff);
			U cp = cos(p1->get_rot(1)*coff), sp = sin(p1->get_rot(1)*coff);
			U ch = cos(p1->get_rot(2)*coff), sh = sin(p1->get_rot(2)*coff);
			
			tmp.row(0) << ch*cb+sh*sp*sb, -ch*sb+sh*sp*cb, sh*cp;
			tmp.row(1) << sb*cp, cb*cp, -sp;
			tmp.row(2) << -sh*cb+ch*sp*sb, sb*sh+ch*sp*cb, ch*cp;
			loc2gbl.block(0, 0, 3, 3) = loc2gbl.block(3, 3, 3, 3) = tmp;
		}
		if(p2->get_rot(0)>1.8e2){
			loc2gbl.block(6, 6, 6, 6).setIdentity();
		}
		else{
			U cb = cos(p2->get_rot(0)*coff), sb = sin(p2->get_rot(0)*coff);
			U cp = cos(p2->get_rot(1)*coff), sp = sin(p2->get_rot(1)*coff);
			U ch = cos(p2->get_rot(2)*coff), sh = sin(p2->get_rot(2)*coff);
			
			tmp.row(0) << ch*cb+sh*sp*sb, -ch*sb+sh*sp*cb, sh*cp;
			tmp.row(1) << sb*cp, cb*cp, -sp;
			tmp.row(2) << -sh*cb+ch*sp*sb, sb*sh+ch*sp*cb, ch*cp;
			loc2gbl.block(6, 6, 3, 3) = loc2gbl.block(9, 9, 3, 3) = tmp;
		}
		int m{opt[1]-1};
		assert(0<=m&&m<=5);
		stif(m, m) = stif(m+6, m+6) = sect->get_sect_prop(SectionProp::ADDONSPRING);
		stif(m+6, m) = stif(m, m+6) = -stif(m, m);
	}
		
	return make_tuple(stif, mass, loc2gbl, rhs);
}

template varargout<REAL8> mass21<REAL4, REAL8>(const NodeBase<REAL4>*,
	const Material<REAL4>*, const Section<REAL4>*, const int[]);
template varargout<REAL8> mass21<REAL8, REAL8>(const NodeBase<REAL8>*,
	const Material<REAL8>*, const Section<REAL8>*, const int[]);
template varargout<REAL4> mass21<REAL4, REAL4>(const NodeBase<REAL4>*,
	const Material<REAL4>*, const Section<REAL4>*, const int[]);
template varargout<REAL4> mass21<REAL8, REAL4>(const NodeBase<REAL8>*,
	const Material<REAL8>*, const Section<REAL8>*, const int[]);	
	
template varargout<REAL8> combin14<REAL4, REAL8>(const NodeBase<REAL4>*,
	const NodeBase<REAL4>*, const Material<REAL4>*, const Section<REAL4>*,
	const int[]);
template varargout<REAL8> combin14<REAL8, REAL8>(const NodeBase<REAL8>*,
	const NodeBase<REAL8>*, const Material<REAL8>*, const Section<REAL8>*,
	const int[]);
template varargout<REAL4> combin14<REAL4, REAL4>(const NodeBase<REAL4>*,
	const NodeBase<REAL4>*, const Material<REAL4>*, const Section<REAL4>*,
	const int[]);
template varargout<REAL4> combin14<REAL8, REAL4>(const NodeBase<REAL8>*,
	const NodeBase<REAL8>*, const Material<REAL8>*, const Section<REAL8>*,
	const int[]);
}
}