#include <tuple>
#include <iostream>
#include <cmath>
#include <Eigen/Dense>
#include <Eigen/Geometry>

#include "cafea.h"

using std::tie;
using std::map;
using std::tuple;
using std::string;
using std::make_tuple;

using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::Vector3d;

namespace cafea
{
namespace
{
template <class T>
using var_out4_ = tuple<T, vecX_<T>, vecX_<T>, vecX_<T>>;
}
/**
 *
 */
template <class T>
var_out4_<T> QACM<T>::shape_2d(T kesi, T yita, const matrix_<T> xy, int num)
{
	assert(4==num||8==num||9==num);
	
	T jacobi{0.0};
	vecX_<T> shp = vecX_<T>::Zero(num);
	vecX_<T> shpdx = vecX_<T>::Zero(num);
	vecX_<T> shpdy = vecX_<T>::Zero(num);
	
	auto c1 = xy(2, 0) - xy(1, 0);
	auto c2 = xy(3, 0) - xy(2, 0);
	auto c3 = xy(0, 0) - xy(3, 0);
	auto c4 = xy(1, 0) - xy(0, 0);
	
	auto b1 = xy(1, 1) - xy(2, 1);
	auto b2 = xy(2, 1) - xy(3, 1);
	auto b3 = xy(3, 1) - xy(0, 1);
	auto b4 = xy(0, 1) - xy(1, 1);
	
	matrix_<T> tmp = matrix_<T>::Zero(3, 3);
	
	tmp.row(0) << xy(0, 0), xy(0, 1), 1.;
	tmp.row(1) << xy(1, 0), xy(1, 1), 1.;
	tmp.row(2) << xy(2, 0), xy(2, 1), 1.;
	auto s123 = .5*tmp.determinant();
	
	tmp.row(2) << xy(3, 0), xy(3, 1), 1.;
	auto s124 = .5*tmp.determinant();
	
	tmp.row(1) << xy(2, 0), xy(2, 1), 1.;
	auto s134 = .5*tmp.determinant();
	
	auto area = s123 + s134;
	
	auto s234 = area - s124;

	auto g1 = s124/area;
	auto g2 = s123/area;
	auto g3 = 1. - g1;
	auto g4 = 1. - g2;
	
	// quadrilateral area coordinate method QACM-III
    auto t1 = (kesi + yita + (1. - 2.*g1)*(1. + kesi*yita))/4.;
    auto t2 = (yita - kesi + (1. - 2.*g2)*(1. - kesi*yita))/4.;
    // quadrilateral area coordinate method QACM-II
    auto z1 = kesi + (g3 - g2)*kesi*yita;
    auto z2 = yita + (g2 - g1)*kesi*yita;
    // quadrilateral area coordinate method QACM-I
    auto L1 = (1. - kesi)*(g2*(1. - yita) + g3*(1. + yita))/4.;
    auto L2 = (1. - yita)*(g4*(1. - kesi) + g3*(1. + kesi))/4.;
    auto L3 = (1. + kesi)*(g1*(1. - yita) + g4*(1. + yita))/4.;
    auto L4 = (1. + yita)*(g1*(1. - kesi) + g2*(1. + kesi))/4.;
	//
	shp(0) = t1*(t1 - g3)/g1;
	shp(1) = t2*(t2 - g4)/g2;
	shp(2) = t1*(t1 + g1)/g3;
	shp(3) = t2*(t2 + g2)/g4;
	// 
    shpdx(0) = (b3 + b4)*(2.*t1 - g3)/(g1*2.*area);
    shpdx(1) = (b1 + b4)*(2.*t2 - g4)/(g2*2.*area);
    shpdx(2) = (b3 + b4)*(2.*t1 + g1)/(g3*2.*area);
    shpdx(3) = (b1 + b4)*(2.*t2 + g2)/(g4*2.*area);
    //
    shpdy(0) = (c3 + c4)*(2.*t1 - g3)/(g1*2.*area);
    shpdy(1) = (c1 + c4)*(2.*t2 - g4)/(g2*2.*area);
    shpdy(2) = (c3 + c4)*(2.*t1 + g1)/(g3*2.*area);
    shpdy(3) = (c1 + c4)*(2.*t2 + g2)/(g4*2.*area);
	//
	jacobi = (1. + (g2 - g1)*kesi + (g3 - g2)*yita)*area/4.;
	assert(jacobi>EPS<T>());
	
	switch(num){
	case 4:
		break;
	case 8:
		shp(4) = 2.*L1*L3*(1. - z2)/(g1*g2);
		shp(5) = 2.*L2*L4*(1. + z1)/(g2*g3);
		shp(6) = 2.*L3*L1*(1. + z2)/(g3*g4);
		shp(7) = 2.*L4*L2*(1. - z1)/(g4*g1);
		
		shpdx(4) = ((1.- z2)*(b1*L3 + b3*L1) + 2.*L1*L3*(b2 - b4))/(g1*g2*area);
		shpdx(5) = ((1.+ z1)*(b2*L4 + b4*L2) + 2.*L2*L4*(b3 - b1))/(g2*g3*area);
		shpdx(6) = ((1.+ z2)*(b3*L1 + b1*L3) + 2.*L3*L1*(b4 - b2))/(g3*g4*area);
		shpdx(7) = ((1.- z1)*(b4*L2 + b2*L4) + 2.*L4*L2*(b1 - b3))/(g4*g1*area);
    
		shpdy(4) = ((1. - z2)*(c1*L3 + c3*L1) + 2.*L1*L3*(c2 - c4))/(g1*g2*area);
		shpdy(5) = ((1. + z1)*(c2*L4 + c4*L2) + 2.*L2*L4*(c3 - c1))/(g2*g3*area);
		shpdy(6) = ((1. + z2)*(c3*L1 + c1*L3) + 2.*L3*L1*(c4 - c2))/(g3*g4*area);
		shpdy(7) = ((1. - z1)*(c4*L2 + c2*L4) + 2.*L4*L2*(c1 - c3))/(g4*g1*area);
		
		shp(0) -= (1. + g3)*(shp(7) + shp(4)) + g3*g3*(shp(5) + shp(6))/(4.*g1);
        shp(1) -= (1. + g4)*(shp(4) + shp(5)) + g4*g4*(shp(6) + shp(7))/(4.*g2);
        shp(2) -= (1. + g1)*(shp(5) + shp(6)) + g1*g1*(shp(7) + shp(4))/(4.*g3);
        shp(3) -= (1. + g2)*(shp(6) + shp(7)) + g2*g2*(shp(4) + shp(5))/(4.*g4);
		
		shpdx(0) -= (1. + g3)*(shpdx(7) + shpdx(4)) + g3*g3*(shpdx(5) + shpdx(6))/(4.*g1);
        shpdx(1) -= (1. + g4)*(shpdx(4) + shpdx(5)) + g4*g4*(shpdx(6) + shpdx(7))/(4.*g2);
        shpdx(2) -= (1. + g1)*(shpdx(5) + shpdx(6)) + g1*g1*(shpdx(7) + shpdx(4))/(4.*g3);
        shpdx(3) -= (1. + g2)*(shpdx(6) + shpdx(7)) + g2*g2*(shpdx(4) + shpdx(5))/(4.*g4);
		
		shpdy(0) -= (1. + g3)*(shpdy(7) + shpdy(4)) + g3*g3*(shpdy(5) + shpdy(6))/(4.*g1);
		shpdy(1) -= (1. + g4)*(shpdy(4) + shpdy(5)) + g4*g4*(shpdy(6) + shpdy(7))/(4.*g2);
		shpdy(2) -= (1. + g1)*(shpdy(5) + shpdy(6)) + g1*g1*(shpdy(7) + shpdy(4))/(4.*g3);
		shpdy(3) -= (1. + g2)*(shpdy(6) + shpdy(7)) + g2*g2*(shpdy(4) + shpdy(5))/(4.*g4);
		break;
	case 9:
		shp(0) = L1*L2*z1*z2/( g2*g4*(g1 + g2)*(g1 + g4));
        shp(1) = L2*L3*z1*z2/(-g3*g1*(g1 + g2)*(g2 + g3));
        shp(2) = L3*L4*z1*z2/( g4*g2*(g3 + g4)*(g2 + g3));
        shp(3) = L4*L1*z1*z2/(-g1*g3*(g3 + g4)*(g1 + g4));
        shp(4) = L1*L2*L3*z2/(-g1*g2*(g3 + g4))*8.0;
        shp(5) = L2*L3*L4*z1/( g2*g3*(g4 + g1))*8.0;
        shp(6) = L3*L4*L1*z2/( g3*g4*(g1 + g2))*8.0;
        shp(7) = L4*L1*L2*z1/(-g4*g1*(g2 + g3))*8.0;
        shp(8) = L1*L2*L3*L4/((g1 + g2)*(g2 + g3)*(g3 + g4)*(g4 + g1))*256.0;
		
		shpdx(0) = (b1*L2*z1*z2 + L1*b2*z1*z2 + L1*L2*2.0*(b3 - b1)*z2 + L1*L2*z1*2.0*(b4 - b2))/( g2*g4*(g1 + g2)*(g1 + g4))/(2.*area);
        shpdx(1) = (b2*L3*z1*z2 + L2*b3*z1*z2 + L2*L3*2.0*(b3 - b1)*z2 + L2*L3*z1*2.0*(b4 - b2))/(-g3*g1*(g1 + g2)*(g2 + g3))/(2.*area);
        shpdx(2) = (b3*L4*z1*z2 + L3*b4*z1*z2 + L3*L4*2.0*(b3 - b1)*z2 + L3*L4*z1*2.0*(b4 - b2))/( g4*g2*(g3 + g4)*(g2 + g3))/(2.*area);
        shpdx(3) = (b4*L1*z1*z2 + L4*b1*z1*z2 + L4*L1*2.0*(b3 - b1)*z2 + L4*L1*z1*2.0*(b4 - b2))/(-g1*g3*(g3 + g4)*(g1 + g4))/(2.*area);
        shpdx(4) = (b1*L2*L3*z2 + L1*b2*L3*z2 + L1*L2*b3*z2 + L1*L2*L3*2.0*(b4 - b2))/(-g1*g2*(g3 + g4))*8.0/(2.*area);
        shpdx(5) = (b2*L3*L4*z1 + L2*b3*L4*z1 + L2*L3*b4*z1 + L2*L3*L4*2.0*(b3 - b1))/( g2*g3*(g4 + g1))*8.0/(2.*area);
        shpdx(6) = (b3*L4*L1*z2 + L3*b4*L1*z2 + L3*L4*b1*z2 + L3*L4*L1*2.0*(b4 - b2))/( g3*g4*(g1 + g2))*8.0/(2.*area);
        shpdx(7) = (b4*L1*L2*z1 + L4*b1*L2*z1 + L4*L1*b2*z1 + L4*L1*L2*2.0*(b3 - b1))/(-g4*g1*(g2 + g3))*8.0/(2.*area);
        shpdx(8) = (b1*L2*L3*L4 + L1*b2*L3*L4 + L1*L2*b3*L4 + L1*L2*L3*b4)/((g1 + g2)*(g2 + g3)*(g3 + g4)*(g4 + g1))*256.0/(2.*area);
        
        shpdy(0) = (c1*L2*z1*z2 + L1*c2*z1*z2 + L1*L2*2.0*(c3 - c1)*z2 + L1*L2*z1*2.0*(c4 - c2))/( g2*g4*(g1 + g2)*(g1 + g4))/(2.*area);
        shpdy(1) = (c2*L3*z1*z2 + L2*c3*z1*z2 + L2*L3*2.0*(c3 - c1)*z2 + L2*L3*z1*2.0*(c4 - c2))/(-g3*g1*(g1 + g2)*(g2 + g3))/(2.*area);
        shpdy(2) = (c3*L4*z1*z2 + L3*c4*z1*z2 + L3*L4*2.0*(c3 - c1)*z2 + L3*L4*z1*2.0*(c4 - c2))/( g4*g2*(g3 + g4)*(g2 + g3))/(2.*area);
        shpdy(3) = (c4*L1*z1*z2 + L4*c1*z1*z2 + L4*L1*2.0*(c3 - c1)*z2 + L4*L1*z1*2.0*(c4 - c2))/(-g1*g3*(g3 + g4)*(g1 + g4))/(2.*area);
        shpdy(4) = (c1*L2*L3*z2 + L1*c2*L3*z2 + L1*L2*c3*z2 + L1*L2*L3*2.0*(c4 - c2))/(-g1*g2*(g3 + g4))*8.0/(2.*area);
        shpdy(5) = (c2*L3*L4*z1 + L2*c3*L4*z1 + L2*L3*c4*z1 + L2*L3*L4*2.0*(c3 - c1))/( g2*g3*(g4 + g1))*8.0/(2.*area);
        shpdy(6) = (c3*L4*L1*z2 + L3*c4*L1*z2 + L3*L4*c1*z2 + L3*L4*L1*2.0*(c4 - c2))/( g3*g4*(g1 + g2))*8.0/(2.*area);
        shpdy(7) = (c4*L1*L2*z1 + L4*c1*L2*z1 + L4*L1*c2*z1 + L4*L1*L2*2.0*(c3 - c1))/(-g4*g1*(g2 + g3))*8.0/(2.*area);
        shpdy(8) = (c1*L2*L3*L4 + L1*c2*L3*L4 + L1*L2*c3*L4 + L1*L2*L3*c4)/((g1 + g2)*(g2 + g3)*(g3 + g4)*(g4 + g1))*256.0/(2.*area);
        break;
	default:
		fmt::print("Error in AQCM.\n");
	}
	
	return make_tuple(jacobi, shp, shpdx, shpdy);
}
}