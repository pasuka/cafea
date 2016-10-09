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
		for(int i: {0, 1, 2, 3})loc2gbl.block(i*3, i*3, 3, 3) = tt;
		if(opt[2]==0){}
		else if(opt[2]==1){}
		else{}
	}
	else{
		euler_tran = p1->get_euler_tran();
		for(int i=0; i<3; i++){
			for(int j=0; j<3; j++)loc2gbl(i, j) = loc2gbl(i+3, j+3) = euler_tran(i, j);
		}
		euler_tran = p2->get_euler_tran();
		for(int i=0; i<3; i++){
			for(int j=0; j<3; j++)loc2gbl(i+6, j+6) = loc2gbl(i+9, j+9) = euler_tran(i, j);
		}
		int m{opt[1]-1};
		assert(0<=m&&m<=5);
		stif(m, m) = stif(m+6, m+6) = sect->get_sect_prop(SectionProp::ADDONSPRING);
		stif(m+6, m) = stif(m, m+6) = -stif(m, m);
	}
	
	return make_tuple(stif, mass, loc2gbl, rhs, attr);
}
}