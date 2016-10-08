#include <tuple>
#include <cmath>
#include <cstddef>

#include <Eigen/Dense>
#include <Eigen/Geometry>

#include "cafea.h"

using std::tie;
using std::map;
using std::tuple;
using std::make_tuple;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::Vector3d;
using Eigen::Vector3f;
using Eigen::AngleAxisf;
using Eigen::Matrix3f;

namespace cafea
{
namespace
{
template <class T>
using varargout = tuple<matrix_<T>, matrix_<T>, matrix_<T>, vecX_<T>, map<string, T>>;
}
/**
 *  \brief Mass.
 */
template <class T, class U>
varargout<U> StructuralElement<T, U>::mass21(const NodeBase<T> *p,
	const Material<T> *prop, const Section<T> *sect, const int opt[])
{
	matrix_<U> stif = matrix_<U>::Zero(6, 6);
	matrix_<U> mass = matrix_<U>::Zero(6, 6);
	matrix_<U> tran = matrix_<U>::Identity(6, 6);
	vecX_<U> rhs = vecX_<U>::Zero(6);
	map<string, U> attr{{"Length", U(0)}, {"Area", U(0)}, {"Volume", U(0)}, 
		{"Mass", sect->get_sect_prop(SectionProp::ADDONMASS)}};
	
	//! Mass on X Y Z direction.
	//! Notice: mass element do not need transform in most cases.
	for(size_t i: {0, 1, 2})mass(i, i) = sect->get_sect_prop(SectionProp::ADDONMASS);
	
	return make_tuple(stif, mass, tran, rhs, attr);
}

/**
 *  \brief Spring.
 */
template <class T, class U>
varargout<U> StructuralElement<T, U>::combin14(const NodeBase<T> *p1,
	const NodeBase<T> *p2, const Material<T> *prop, const Section<T> *sect,
	const int opt[])
{
	matrix_<U> stif = matrix_<U>::Zero(12, 12);
	matrix_<U> mass = matrix_<U>::Zero(12, 12);
	matrix_<U> loc2gbl = matrix_<U>::Zero(12, 12);
	vecX_<U> rhs = vecX_<U>::Zero(12);
	map<string, U> attr{{"Length", U(0)}, {"Area", U(0)}, {"Volume", U(0)},}; 
	
	Eigen::Matrix<T, 3, 3> euler_tran;
	U l;
	matrix_<U> tt = matrix_<U>::Zero(3, 3);
	tie(l, tt) = NodeFunc<T, U>::coord_tran(p1, p2);
	
	if(opt[1]==0){
		for(size_t i: {0, 1, 2, 3})loc2gbl.block(i*3, i*3, 3, 3) = tt;
		if(opt[2]==0){
			
		}
		else if(opt[2]==1){
			
		}
		else{}
	}
	else{
		if(p1->get_rot(0)>1.8e2){
			loc2gbl.block(0, 0, 6, 6).setIdentity();
		}
		else{
			/* 
			matrix_<U> tmp = matrix_<U>::Zero(3, 3);
			U cb = cos(p1->get_rot_rad(0)), sb = sin(p1->get_rot_rad(0));
			U cp = cos(p1->get_rot_rad(1)), sp = sin(p1->get_rot_rad(1));
			U ch = cos(p1->get_rot_rad(2)), sh = sin(p1->get_rot_rad(2));
			
			tmp.row(0) << ch*cb+sh*sp*sb, -ch*sb+sh*sp*cb, sh*cp;
			tmp.row(1) << sb*cp, cb*cp, -sp;
			tmp.row(2) << -sh*cb+ch*sp*sb, sb*sh+ch*sp*cb, ch*cp;
			loc2gbl.block(0, 0, 3, 3) = loc2gbl.block(3, 3, 3, 3) = tmp;
			
			
			Matrix3f tmp;
			tmp  = AngleAxisf(p1->get_rot_rad(2), Vector3f::UnitY())*AngleAxisf(p1->get_rot_rad(1), Vector3f::UnitX())*AngleAxisf(p1->get_rot_rad(0), Vector3f::UnitZ());
			*/
			
			euler_tran = p1->get_euler_tran();
			loc2gbl.block(0, 0, 3, 3) = loc2gbl.block(3, 3, 3, 3) = euler_tran.cast<U>();
		}
		if(p2->get_rot(0)>1.8e2){
			loc2gbl.block(6, 6, 6, 6).setIdentity();
		}
		else{
			/* 
			matrix_<U> tmp = matrix_<U>::Zero(3, 3);
			U cb = cos(p2->get_rot_rad(0)), sb = sin(p2->get_rot_rad(0));
			U cp = cos(p2->get_rot_rad(1)), sp = sin(p2->get_rot_rad(1));
			U ch = cos(p2->get_rot_rad(2)), sh = sin(p2->get_rot_rad(2));
			
			tmp.row(0) << ch*cb+sh*sp*sb, -ch*sb+sh*sp*cb, sh*cp;
			tmp.row(1) << sb*cp, cb*cp, -sp;
			tmp.row(2) << -sh*cb+ch*sp*sb, sb*sh+ch*sp*cb, ch*cp;
			loc2gbl.block(6, 6, 3, 3) = loc2gbl.block(9, 9, 3, 3) = tmp;
			
			
			Matrix3f tmp;
			tmp  = AngleAxisf(p1->get_rot_rad(2), Vector3f::UnitY())*AngleAxisf(p1->get_rot_rad(1), Vector3f::UnitX())*AngleAxisf(p1->get_rot_rad(0), Vector3f::UnitZ());
			*/
			
			euler_tran = p2->get_euler_tran();
			loc2gbl.block(6, 6, 3, 3) = loc2gbl.block(9, 9, 3, 3) = euler_tran.cast<U>();
		}
		int m{opt[1]-1};
		assert(0<=m&&m<=5);
		stif(m, m) = stif(m+6, m+6) = sect->get_sect_prop(SectionProp::ADDONSPRING);
		stif(m+6, m) = stif(m, m+6) = -stif(m, m);
	}
	
	return make_tuple(stif, mass, loc2gbl, rhs, attr);
}
}