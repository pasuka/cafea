#include <tuple>
#include <cmath>
#include <cassert>
#include <iostream>

#include <Eigen/Dense>

#include "cafea.h"

using std::tuple;
using std::make_tuple;

namespace cafea
{
/**
 *  \brief Coordinate transform for 2-node pipe element.
 *  \param [in] p1 Start node.
 *  \param [in] p2 End node. 
 */
template <class Scalar, class T>
tuple<Scalar, matrix_<Scalar> > coord_tran(
	const Node<Scalar, T> &p1,
	const Node<Scalar, T> &p2,)
{
	Scalar length{0};
	matrix_<Scalar> tran = matrix_<Scalar>::Zero(3, 3);
	vec3_<Scalar> vxx, vxy, vyy, vzz;
	
	vxx = p2.get_xyz() - p1.get_xyz();
	length = vxx.norm();
	assert(length>(cafea::EPS<Scalar>()));
	vxx /= length;
	
	auto A{sqrt(vxx(0)*vxx(0)+vxx(1)*vxx(1))};
	if(A<Scalar(1.e-6)){
		tran(0, 2) = vxx(2)/fabs(vxx(2));
		tran(1, 1) = Scalar(1);
		tran(2, 0) = -vxx(2)/fabs(vxx(2));
	}
	else{
		tran.row(0) << vxx(0), vxx(1), vxx(2);
		tran.row(1) << -vxx(1)/A, vxx(0)/A, Scalar(0);
		tran.row(2) << -vxx(0)*vxx(2)/A, -vxx(1)*vxx(2)/A, A;
	}
	
	return make_tuple(length, tran);
}

/**
 *  \brief Coordinate transform for 2-node beam with up-axis.
 */
template <class Scalar, class T>
tuple<Scalar, matrix_<Scalar> > coord_tran(
	const node<Scalar, T> &p1,
	const node<Scalar, T> &p2,
	const double up[])
{
	Scalar length{0};
	matrix_<Scalar> tran = matrix_<Scalar>::Zero(3, 3);
	vec3_<Scalar> vxx, vyy, vzz, vxy;
	
	vxx = p2.get_xyz() - p1.get_xyz();
	vxy << up[0], up[1], up[2];
	
	length = vxx.norm();
	
	assert(length>(cafea::EPS<double>()));
	
	vxx /= length;
	vxy /= vxy.norm();
	
	auto the{acos(vxx.dot(vxy))*Scalar(180)/cafea::PI<Scalar>()};
	if(the<Scalar(1)){
		vxy << Scalar(1), Scalar(0), Scalar(1);
	}
	vzz = vxx.cross(vxy);
	vyy = vzz.cross(vxx);
	
	vyy /= vyy.norm();
	vzz /= vzz.norm();
	
	tran.row(0) << vxx(0), vxx(1), vxx(2),
	tran.row(1) << vyy(0), vyy(1), vyy(2),
	tran.row(2) << vzz(0), vzz(1), vzz(2);
	
	return make_tuple(length, tran);
}

/**
 *  \brief Coordinate transform for 3-node triangle element.
 */
template <class Scalar, class T>
tuple<Scalar, matrix_<Scalar>, matrix_<Scalar> > coord_tran(
	const Node<Scalar, T>& p1, 
	const Node<Scalar, T>& p2,
	const Node<Scalar, T>& p3)
{
	Scalar area{0};
	matrix_<Scalar> tran = matrix_<Scalar>::Zero(3, 3);
	matrix_<Scalar> xy = matrix_<Scalar>::Zero(3, 2);
	vec3_<Scalar> vxx, vyy, vzz, vxy;
	
	vxx = p2.get_xyz() - p1.get_xyz();
	vxy = p3.get_xyz() - p1.get_xyz();
	
	auto L12 = vxx.norm();
	auto L13 = vxy.norm();
	
	assert(L12>(cafea::EPS<Scalar>()));
	assert(L13>(cafea::EPS<Scalar>()));
	
	vxx /= L12;
	vxy /= L13;
	
	vzz = vxx.cross(vxy);
	
	vyy = vzz.cross(vxx);
	
	vyy /= vyy.norm();
	vzz /= vzz.norm();
	tran.row(0) << vxx(0), vxx(1), vxx(2);
	tran.row(1) << vyy(0), vyy(1), vyy(2);
	tran.row(2) << vzz(0), vzz(1), vzz(2);
	vxy = p3.get_xyz() - p2.get_xyz();
	auto L23 = vxy.norm();
	
	assert(L23>(cafea::EPS<Scalar>()));
	auto cos_t = (L12*L12+L13*L13-L23*L23)/(Scalar(2)*L12*L13);
	auto alpha = acos(cos_t);
	
	xy.row(1) << L12, Scalar(0);
	xy.row(2) << L13*cos_t, L13*sin(alpha);
	area = L12*L13*sin(alpha)/Scalar(2);
	
	return make_tuple(area, xy, tran);
}

/**
 *  \brief Coordinate transform for 4-node quadrangle element.
 */
template <class Scalar, class T>
tuple<Scalar, matrix_<Scalar>, matrix_<Scalar> > coord_tran(
	const Node<Scalar, T>& p1,
	const Node<Scalar, T>& p2,
	const Node<Scalar, T>& p3,
	const Node<Scalar, T>& p4)
{
	Scalar area{0};
	matrix_<Scalar> tran = matrix_<Scalar>::Zero(3, 3); 
	matrix_<Scalar> xy = matrix_<Scalar>::Zero(4, 2);
	vec3_<Scalar> vxx, vyy, vzz, vxy;

	vxx = p2.get_xyz() - p1.get_xyz();
	vxy = p3.get_xyz() - p1.get_xyz();
	
	auto L12 = vxx.norm();
	auto L13 = vxy.norm();
	
	assert(L12>(cafea::EPS<Scalar>()));
	assert(L13>(cafea::EPS<Scalar>()));
	
	vxx /= L12;
	vxy /= L13;
	
	vzz = vxx.cross(vxy);
	vyy = vzz.cross(vxx);
	vyy /= vyy.norm();
	vzz /= vzz.norm();
	tran.row(0) << vxx(0), vxx(1), vxx(2);
	tran.row(1) << vyy(0), vyy(1), vyy(2);
	tran.row(2) << vzz(0), vzz(1), vzz(2);
	
	vxy = p3.get_xyz() - p2.get_xyz();
	auto L23 = vxy.norm();
	
	assert(L23>(cafea::EPS<Scalar>()));
	
	auto alpha213 = acos((L12*L12+L13*L13-L23*L23)/(Scalar(2)*L12*L13));
	
	vxy = p4.get_xyz() - p1.get_xyz();
	auto L14 = vxy.norm();
	
	assert(L14>(cafea::EPS<Scalar>()));
	
	vxy = p4.get_xyz() - p3.get_xyz();
	auto L34 = vxy.norm();
	
	assert(L34>(cafea::EPS<Scalar>()));
	
	auto alpha314 = acos((L13*L13+L14*L14-L34*L34)/(Scalar(2)*L13*L14));
	auto alpha41y = cafea::PI<Scalar>()/Scalar(2) - alpha213 - alpha314;
	
	xy.row(0) << Scalar(0), Scalar(0);
	xy.row(1) << L12, Scalar(0);
	xy.row(2) << L13*cos(alpha213), L13*sin(alpha213);
	xy.row(3) << L14*sin(alpha41y), L14*cos(alpha41y);
	
	area = (L12*L13*sin(alpha213)+L13*L14*sin(alpha314))/Scalar(2);

	return make_tuple(area, xy, tran);
}
}