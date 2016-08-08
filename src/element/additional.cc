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
using varargout = tuple<MatrixXd, MatrixXd, MatrixXd, VectorXd>;
}
template <class T>
varargout mass21(const NodeBase<T> *p, const Material<T> &prop,
	const Section<T> &sect, const int opt[])
{
	MatrixXd stif = MatrixXd::Zero(6, 6);
	MatrixXd mass = MatrixXd::Zero(6, 6);
	MatrixXd tran = MatrixXd::Identity(6, 6);
	VectorXd rhs = VectorXd::Zero(6);
	
	size_t dofs;
	auto dof_vec = p->dof_list();
	if(!dof_vec.empty()){
		// Mass on X Y Z direction
		// Notice: mass element do not need transform in most cases.
		if(opt[2]==2){
			for(size_t i: {0, 1, 2})mass(i, i) = sect.get_sect_prop(SectionProp::ADDONMASS);
		}
		else{
			dofs = 6>dof_vec.size() ? dof_vec.size(): 6;
			for(size_t i=0; i<dofs; i++)mass(i, i) = sect.get_sect_prop(SectionProp::ADDONMASS);
		}
	}
	else{}

	return make_tuple(stif, mass, tran, rhs);
}

template <class T>
varargout combin14(const NodeBase<T> *p1, const NodeBase<T> *p2,
	const Material<T> &prop, const Section<T> &sect, const int opt[])
{
	MatrixXd stif = MatrixXd::Zero(12, 12);
	MatrixXd mass = MatrixXd::Zero(12, 12);
	MatrixXd loc2gbl = MatrixXd::Zero(12, 12);
	VectorXd rhs = VectorXd::Zero(12);
	
	double l;
	MatrixXd tt = MatrixXd::Zero(3, 3);
	tie(l, tt) = coord_tran(p1, p2);
	
	if(opt[1]==0){
		for(int i: {0, 1, 2, 3})loc2gbl.block<3, 3>(i*3, i*3) = tt;
		if(opt[2]==0){
			
		}
		else if(opt[2]==1){
			
		}
		else{}
	}
	else{
		MatrixXd tmp = MatrixXd::Zero(3, 3);
		double coff{PI<>()/1.8e2};
		if(p1->get_rot(0)>1.8e2){
			loc2gbl.block<6, 6>(0, 0) = MatrixXd::Identity(6, 6);
		}
		else{
			double cb{cos(p1->get_rot(0)*coff)}, sb{sin(p1->get_rot(0)*coff)};
			double cp{cos(p1->get_rot(1)*coff)}, sp{sin(p1->get_rot(1)*coff)};
			double ch{cos(p1->get_rot(2)*coff)}, sh{sin(p1->get_rot(2)*coff)};
			
			tmp.row(0) << ch*cb+sh*sp*sb, -ch*sb+sh*sp*cb, sh*cp;
			tmp.row(1) << sb*cp, cb*cp, -sp;
			tmp.row(2) << -sh*cb+ch*sp*sb, sb*sh+ch*sp*cb, ch*cp;
			loc2gbl.block<3, 3>(0, 0) = loc2gbl.block<3, 3>(3, 3) = tmp;
		}
		if(p2->get_rot(0)>1.8e2){
			loc2gbl.block<6, 6>(6, 6) = MatrixXd::Identity(6, 6);
		}
		else{
			double cb{cos(p2->get_rot(0)*coff)}, sb{sin(p2->get_rot(0)*coff)};
			double cp{cos(p2->get_rot(1)*coff)}, sp{sin(p2->get_rot(1)*coff)};
			double ch{cos(p2->get_rot(2)*coff)}, sh{sin(p2->get_rot(2)*coff)};
			
			tmp.row(0) << ch*cb+sh*sp*sb, -ch*sb+sh*sp*cb, sh*cp;
			tmp.row(1) << sb*cp, cb*cp, -sp;
			tmp.row(2) << -sh*cb+ch*sp*sb, sb*sh+ch*sp*cb, ch*cp;
			loc2gbl.block<3, 3>(6, 6) = loc2gbl.block<3, 3>(9, 9) = tmp;
		}
		size_t m{opt[1]-1};
		assert(m<=5);
		stif(m, m) = stif(m+6, m+6) = sect.get_sect_prop(SectionProp::ADDONSPRING);
		stif(m+6, m) = stif(m, m+6) = -stif(m, m);
	}
		
	return make_tuple(stif, mass, loc2gbl, rhs);
}
template varargout mass21<REAL4>(const NodeBase<REAL4>*, const Material<REAL4>&,
	const Section<REAL4>&, const int[]);
template varargout mass21<REAL8>(const NodeBase<REAL8>*, const Material<REAL8>&,
	const Section<REAL8>&, const int[]);
template varargout combin14<REAL4>(const NodeBase<REAL4>*, const NodeBase<REAL4>*,
	const Material<REAL4>&, const Section<REAL4>&, const int[]);
template varargout combin14<REAL8>(const NodeBase<REAL8>*, const NodeBase<REAL8>*,
	const Material<REAL8>&, const Section<REAL8>&, const int[]);
}
}