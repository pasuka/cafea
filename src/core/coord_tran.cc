#include <tuple>
#include <cmath>
#include <cassert>
#include <iostream>

#include <Eigen/Dense>

#include "main.h"

using std::tuple;
using std::make_tuple;
// using Eigen::Matrix3d;
using Eigen::MatrixXd;
using Eigen::Vector3d;

namespace cafea
{
/**
 *  \brief Coordinate transform for 2-node pipe element.
 *  \param [in] p1 Start node.
 *  \param [in] p2 End node. 
 */
tuple<double, MatrixXd> coord_tran(const Node &p1, const Node &p2)
{
	double length{0.};
	MatrixXd tran = MatrixXd::Zero(3, 3);
	Vector3d vxx, vxy, vyy, vzz;
	
	vxx = p2.get_xyz() - p1.get_xyz();
	length = vxx.norm();
	assert(length>cafea::EPS<double>());
	vxx /= length;
	
	double A{sqrt(vxx(0)*vxx(0)+vxx(1)*vxx(1))};
	if(A<1.e-6){
		tran(0, 2) = vxx(2)/fabs(vxx(2));
		tran(1, 1) = 1.;
		tran(2, 0) = -vxx(2)/fabs(vxx(2));
	}
	else{
		tran.row(0) << vxx(0), vxx(1), vxx(2);
		tran.row(1) << -vxx(1)/A, vxx(0)/A, 0.;
		tran.row(2) << -vxx(0)*vxx(2)/A, -vxx(1)*vxx(2)/A, A;
	}
	
	return make_tuple(length, tran);
}

/**
 *  \brief Coordinate transform for 2-node beam with up-axis.
 */
tuple<double, MatrixXd> coord_tran(const node &p1, const node &p2, const double up[])
{
	double length {0.};
	// MatrixXd tran(3, 3);
	MatrixXd tran = MatrixXd::Zero(3, 3);
	Vector3d vxx, vyy, vzz, vxy;
	
	vxx = p2.get_xyz() - p1.get_xyz();
	vxy << up[0], up[1], up[2];
	
	length = vxx.norm();
	
	assert(length>cafea::EPS<double>());
	
	vxx /= length;
	vxy /= vxy.norm();
	
	auto the{acos(vxx.dot(vxy))*1.8e2/cafea::PI<double>()};
	if(the<1.0){
		vxy << 1., 0., 1.;
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
tuple<double, MatrixXd, MatrixXd> coord_tran(const Node& p1, const Node& p2, const Node& p3)
{
	int flag {0};
	double area {0.};
	MatrixXd tran = MatrixXd::Zero(3, 3);
	MatrixXd xy = MatrixXd::Zero(3, 2);
	Vector3d vxx, vyy, vzz, vxy;
	
	vxx = p2.get_xyz() - p1.get_xyz();
	vxy = p3.get_xyz() - p1.get_xyz();
	
	auto L12 = vxx.norm();
	auto L13 = vxy.norm();
	
	assert(L12>cafea::EPS<double>());
	assert(L13>cafea::EPS<double>());
	
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
	
	assert(L23>cafea::EPS<double>());
	auto cos_t = (L12*L12+L13*L13-L23*L23)/(2.0*L12*L13);
	auto alpha = acos(cos_t);
	
	xy.row(1) << L12, 0.;
	xy.row(2) << L13*cos_t, L13*sin(alpha);
	area = 0.5*L12*L13*sin(alpha);
	
	return make_tuple(area, xy, tran);
}

/**
 *  \brief Coordinate transform for 4-node quadrangle element.
 */
tuple<int, double, MatrixXd, MatrixXd> coord_tran(const Node& p1, const Node &p2, const Node &p3, const Node &p4)
{
	double area {0.};
	MatrixXd tran = MatrixXd::Zero(3, 3); 
	MatrixXd xy = MatrixXd::Zero(4, 2);
	Vector3d vxx, vyy, vzz, vxy;

	vxx = p2.get_xyz() - p1.get_xyz();
	vxy = p3.get_xyz() - p1.get_xyz();
	
	auto L12 = vxx.norm();
	auto L13 = vxy.norm();
	
	assert(L12>cafea::EPS<double>());
	assert(L13>cafea::EPS<double>());
	
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
	
	assert(L23>cafea::EPS<double>());
	
	auto alpha213 = acos((L12*L12+L13*L13-L23*L23)/(2.0*L12*L13));
	
	vxy = p4.get_xyz() - p1.get_xyz();
	auto L14 = vxy.norm();
	
	assert(L14>cafea::EPS<double>());
	
	vxy = p4.get_xyz() - p3.get_xyz();
	auto L34 = vxy.norm();
	
	assert(L34>cafea::EPS<double>());
	
	auto alpha314 = acos((L13*L13+L14*L14-L34*L34)/(2.0*L13*L14));
	auto alpha41y = 0.5*cafea::PI() - alpha213 - alpha314;
	
	xy.row(0) << 0., 0.;
	xy.row(1) << L12, 0.;
	xy.row(2) << L13*cos(alpha213), L13*sin(alpha213);
	xy.row(3) << L14*sin(alpha41y), L14*cos(alpha41y);
	
	area = 0.5*(L12*L13*sin(alpha213)+L13*L14*sin(alpha314));

	return make_tuple(area, xy, tran);
}
}