#include <tuple>
#include <iostream>
#include <cmath>
#include <Eigen/Dense>
#include <Eigen/Geometry>

#include "cafea.h"

using std::tuple;
using std::make_tuple;
using std::tie;
using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::Vector3d;
// using Eigen::MatrixXcd;

namespace cafea
{
namespace pipe_elem_lib
{
namespace
{
using varargout = tuple<MatrixXd, MatrixXd, MatrixXd, VectorXd>;
}

varargout pipe16(const Node plist[2], const Material& prop)
{
	return pipe16(plist[0], plist[1], prop);
}

varargout pipe18(const Node plist[3], const Material& prop)
{
	return pipe18(plist[0], plist[1], plist[2], prop);
}

/**
 *  \brief Straight Pipe Element No.16
 *  
 *  \param [in] p1 Start node of element
 *  \param [in] p2 End node of element
 *  \param [in] prop Material parameters
 *  \return info, element_stiffness, element_mass, transform, rhs
 *  
 *  \details Details
 */
varargout pipe16(const Node& p1, const Node& p2, const Material& prop)
{
	MatrixXd stif = MatrixXd::Zero(12, 12);
	MatrixXd mass = MatrixXd::Zero(12, 12);
	VectorXd rhs = VectorXd::Zero(12);
	
	// material prop2 = prop;
	// prop2.mtype =21;
	// float up[]={0.1, 0., 1.};
	// return beam_elem_lib::beam188(p1, p2, prop2, up);
	
	double Ro{0.5*prop.param2[0]}, Ri{Ro-prop.param2[1]};
	double Ax{PI()*(Ro*Ro-Ri*Ri)};
	double Jxx{0.5*PI()*(pow(Ro, 4.0)-pow(Ri, 4.0))};
	double Izz{0.5*Jxx}, Iyy{Izz};
	double v{prop.param[2]};
	double ES{prop.param[1]*Ax};
	double GJx{prop.param[1]*Jxx*.5/(1.+v)};
	
	int jj;
	double Le;
	MatrixXd tt = MatrixXd::Zero(3, 3);
	tie(jj, Le, tt) = coord_tran(p1, p2);
	
	MatrixXd loc2gbl = MatrixXd::Zero(12, 12);
	for(int i: {0, 1, 2, 3})loc2gbl.block<3, 3>(i*3, i*3) = tt;
	
	double Me{prop.param[0]*Ax*Le};
	
	stif(0, 0) = stif(6, 6) = ES/Le;
	stif(6, 0) = stif(0, 6) = -stif(0, 0);
	
	stif(3, 3) = stif(9, 9) = GJx/Le;
	stif(9, 3) = stif(3, 9) = -stif(3, 3);
	
	if(LUMP_MASS>0){
		mass(0, 0) = mass(6, 6) = Me/2.;
		mass(3, 3) = mass(9, 9) = prop.param[0]*Jxx*Le/2.;
	}
	else{
		mass(0, 0) = mass(6, 6) = Me/3.;
		mass(0, 6) = mass(6, 0) = mass(0, 0)/2.;
	
		mass(3, 3) = mass(9, 9) = prop.param[0]*Jxx*Le/3.;
		mass(3, 9) = mass(9, 3) = mass(3, 3)/2.;
	}
	
	// ANSYS set 2
	// frame3dd set .54414+2.97294*Ri/Ro-1.51899*Ri*Ri/Ro/Ro
	// The Book "Gas Pulsation And Pipe Vibration Of Piston Compressor"(in Chinese) set 1.885
	double alpha{2.};
	double Ksy{2.4e1*(1.+v)*Iyy*alpha/(Ax*Le*Le)};
	double Ksz{Ksy};
	// double Ksz{2.4e1*(1.+v)*Izz*alpha/(Ax*Le*Le)};
	double EIz{prop.param[1]*Izz}, EIy{prop.param[1]*Iyy};
	
	stif(1, 1) = stif(7, 7) = 12.*EIy/(Le*Le*Le*(1.+Ksy));
	stif(5, 5) = stif(11, 11) = (4.+Ksy)*EIy/(Le*(1.+Ksy));
	stif(5, 1) = stif(1, 5) = 6.*EIy/(Le*Le*(1.+Ksy));
	stif(11, 5) = stif(5, 11) = (2.-Ksy)*EIy/(Le*(1.+Ksy));
	stif(11, 7) = stif(7, 11) = stif(7, 5) = stif(5, 7) = -stif(5, 1);
	stif(11, 1) = stif(1, 11) = stif(5, 1);
	stif(7, 1) = stif(1, 7) = -stif(1, 1);
	
	stif(2, 2) = stif(8, 8) = 12.*EIz/(Le*Le*Le*(1.+Ksz));
	stif(4, 4) = stif(10, 10) = (4.+Ksz)*EIz/(Le*(1.+Ksz));
	stif(4, 2) = stif(2, 4) = -6.*EIz/(Le*Le*(1.+Ksz));
	stif(10, 4) = stif(4, 10) = (2.-Ksz)*EIz/(Le*(1.+Ksz));
	stif(10, 8) = stif(8, 10) = stif(8, 4) = stif(4, 8) = -stif(4, 2);
	stif(10, 2) = stif(2, 10) = stif(4, 2);
	stif(8, 2) = stif(2, 8) = -stif(2, 2);
	
	// {
		// double t{Me/pow(1.+Ksz, 2.)}, rz{prop.param[0]*Izz/pow(1.+Ksz, 2.)};
		// mass(1, 1) = mass(7, 7) = t*(13./35.+7.*Ksz/10.+Ksz*Ksz/3.) + 6.*rz/(5.*Le);
		// mass(5, 5) = mass(11, 11) = t*Le*Le*(1./105.+Ksz/60.+Ksz*Ksz/120.) + rz*(2.*Le/15.+Le*Ksy/6.+Le*Ksy*Ksy/3.);
		// mass(5, 1) = mass(1, 5) = t*(11.*Le/210.+11.*Le*Ksz/120.+Le*Ksz*Ksz/24.)+rz*(1./10.-Ksz/2.);
		// mass(5, 7) = mass(7, 5) = t*(13.*Le/420.+3.*Le*Ksz/40.+Le*Ksz*Ksz/24.)+rz*(-1./10.+Ksz/2.);
		// mass(11, 7) = mass(7, 11) = t*(11.*Le/210.-11.*Le*Ksz/120.-Le*Ksz*Ksz/24.)+rz*(-1./10.+Ksz/2.);
		// mass(1, 11) = mass(11, 1) = t*(-13.*Le/420.-3.*Le*Ksz/40.-Le*Ksz*Ksz/24.)+rz*(1./10.-Ksz/2.);
		
		// mass(1, 7) = mass(7, 1) = t*(9./70.+3./10.*Ksz+Ksz*Ksz/6.)-rz*6./(5.*Le);
		// mass(11, 5) = mass(5, 11) = t*(-Le*Le/140.-Le*Le*Ksz/60.-Le*Le*Ksz*Ksz/120.)+rz*(-Le/30.-Le*Ksz/6.+Le*Ksz*Ksz/6.);
	// }
	
	// {
		// double t{Me/pow(1.+Ksy, 2.)}, ry{prop.param[0]*Iyy/pow(1.+Ksy, 2.)};
		// mass(2, 2) = mass(8, 8) = t*(13./35.+7.*Ksy/10.+Ksy*Ksy/3.) + 6.*ry/(5.*Le);
		// mass(4, 4) = mass(10, 10) = t*Le*Le*(1./105.+Ksy/60.+Ksy*Ksy/120.) + ry*(2.*Le/15.+Le*Ksy/6.+Le*Ksy*Ksy/3.);
		// mass(4, 2) = mass(2, 4) = -t*(11.*Le/210.+11.*Le*Ksy/120.+Le*Ksy*Ksy/24.)-ry*(1./10.-Ksy/2.);
		// mass(4, 8) = mass(8, 4) = -t*(13.*Le/420.+3.*Le*Ksy/40.+Le*Ksy*Ksy/24.)-ry*(-1./10.+Ksy/2.);
		// mass(2, 10) = mass(10, 2) = -t*(-13.*Le/420.-3.*Le*Ksy/40.-Le*Ksy*Ksy/24.)-ry*(1./10.-Ksy/2.);
		// mass(10, 8) = mass(8, 10) = -t*(11.*Le/210.-11.*Le*Ksy/120.-Le*Ksy*Ksy/24.)-ry*(-1./10.+Ksy/2.);
		// mass(2, 8) = mass(8, 2) = t*(9./70.+3./10.*Ksy+Ksy*Ksy/6.)-ry*6./(5.*Le);
		// mass(10, 4) = mass(4, 10) = t*(-Le*Le/140.-Le*Le*Ksy/60.-Le*Le*Ksy*Ksy/120.)+ry*(-Le/30.-Le*Ksy/6.+Le*Ksy*Ksy/6.);
	// }
	double t{Me};
	double rz{prop.param[0]*Izz}, ry{prop.param[0]*Iyy};
	
	if(LUMP_MASS>0){
		mass(1, 1) = mass(7, 7) = mass(2, 2) = mass(8, 8) = t/2.;
		mass(5, 5) = mass(11, 11) = rz/2.;
		mass(4, 4) = mass(10, 10) = ry/2.;
	}
	else{
		mass(1, 1) = mass(7, 7) = 13.*t/35. + 6.*rz/(5.*Le);
		mass(5, 5) = mass(11, 11) = t*Le*Le/105. + 2.*Le*rz/15.;
		mass(11, 5) = mass(5, 11) = -Le*Le*t/140. - rz*Le/30.;
		mass(5, 1) = mass(1, 5) = 11.*t*Le/210. + rz/10.;
		mass(7, 5) = mass(5, 7) = 13.*t*Le/420. - rz/10.;
		mass(7, 1) = mass(1, 7) = 9.*t/70. - 6.*rz/(5.*Le);
		
		mass(11, 7) = mass(7, 11) = -mass(5, 1);
		mass(11, 1) = mass(1, 11) = -mass(5, 7);
		
		mass(2, 2) = mass(8, 8) = 13.*t/35. + 6.*ry/(5.*Le);
		mass(4, 4) = mass(10, 10) = t*Le*Le/105. + 2.*Le*ry/15.;
		mass(10, 4) = mass(4, 10) = -Le*Le*t/140. - ry*Le/30.;
		mass(4, 2) = mass(2, 4) = -11.*t*Le/210. - ry/10.;
		mass(8, 4) = mass(4, 8) = -13.*t*Le/420. + ry/10.;
		mass(8, 2) = mass(2, 8) =  9.*t/70. - 6.*ry/(5.*Le);
		
		mass(10, 2) = mass(2, 10) =  -mass(4, 8);
		mass(10, 8) = mass(8, 10) =  -mass(4, 2);
	}
	return make_tuple(stif, mass, loc2gbl, rhs);
}

varargout pipe18(const Node& p1, const Node& p2, const Node& cen, const Material& prop)
{
	MatrixXd stif = MatrixXd::Zero(12, 12);
	MatrixXd mass = MatrixXd::Zero(12, 12);
	VectorXd rhs = VectorXd::Zero(12);
	MatrixXd fij = MatrixXd::Zero(6, 6);
	MatrixXd H = MatrixXd::Zero(6, 6);
	MatrixXd sij = MatrixXd::Zero(6, 6);
	
	double Ro{0.5*prop.param2[0]}, Ri{Ro-prop.param2[1]};
	double t{prop.param2[1]}, Ax{PI()*(Ro*Ro-Ri*Ri)};
	double Jxx{0.5*PI()*(pow(Ro, 4.0)-pow(Ri, 4.0))};
	double Iyy{0.5*Jxx};
	double R{prop.param2[2]}, R2{R*R}, R3{R*R*R};
	double v{prop.param[2]};
	
	MatrixXd tt = MatrixXd::Zero(3, 3);
	
	Vector3d vxx, vyy, vzz, vxy;
	vxy = cen.get_xyz() - p1.get_xyz();
	vxx = p2.get_xyz() - p1.get_xyz();
	
	double la{vxx.norm()}, lb{vxy.norm()};
	vxx /= vxx.norm();
	vzz = vxx.cross(vxy);
	vzz /= vzz.norm();
	vyy = vzz.cross(vxx);
	vyy /= vyy.norm();
	tt.row(0) << vxx(0), vxx(1), vxx(2);
	tt.row(1) << vyy(0), vyy(1), vyy(2);
	tt.row(2) << vzz(0), vzz(1), vzz(2);
	
	double the{2.0*asin(0.5*la/R)};
	double l{R*the}, cos_the{cos(the)}, sin_the{sin(the)};
	double cos_2the{cos(2.0*the)}, sin_2the{sin(2.0*the)};
	
	// see also PIPE16
	// double alpha{.54414+2.97294*Ri/Ro-1.51899*Ri*Ri/Ro/Ro};
	// double alpha{1.885};
	double alpha{2.};
	double Me{prop.param[0]*Ax*l};
	double EA{prop.param[1]*Ax}, GA{prop.param[1]*Ax*0.5/(1.0+v)};
	double EI{prop.param[1]*Iyy}, GI{prop.param[1]*Iyy*0.5/(1.0+v)};
	double kp;
	
	{
		double r{Ro-0.5*t}, h{t*R/(r*r)}, pr{prop.param2[3]};
		double Xk;
		if(1.7>(R/r)){
			Xk = 0.;
		}
		else{
			Xk = pow(r/t, 4./3.)*pow(R/r, 1./3.);
		}
		kp = 1.65/(h*(1.+6.*pr*Xk/(prop.param[1]*t)));
		
		if(kp<1.)kp = 1.;
	}
	
	fij(0, 0) = the*(R/2.0/EA+alpha*R/2.0/GA)+sin_2the*(R/4.0/EA-alpha*R/4.0/GA)+kp*R3/4.0/EI*(2.0*the*(2.0+cos_2the)-3.0*sin_2the);
	fij(1, 1) = the*(R/2.0/EA+alpha*R/2.0/GA)-sin_2the*(R/4.0/EA-alpha*R/4.0/GA)+kp*R3/4.0/EI*(2.0*the*(2.0-cos_2the)+3.0*sin_2the-8.0*sin_the);
	fij(2, 2) = alpha*R/GA*the+R3/8.0/GI*(6.0*the+sin_2the-8.0*sin_the)+kp*R3/4.0/EI*(2.0*the-sin_2the);
	fij(3, 3) = the*(R/4.0/GI+kp*R/2.0/EI)+sin_2the*(R/8.0/GI-kp*R/4.0/EI);
	fij(4, 4) = the*(R/4.0/GI+kp*R/2.0/EI)-sin_2the*(R/8.0/GI-kp*R/4.0/EI);
	fij(5, 5) = kp*R*the/EI;
	
	fij(1, 0) = fij(0, 1) = pow(sin_the, 2.0)*(R/2.0/EA-alpha*R/2.0/GA)+kp*R3/4.0/EI*(1.0+3.0*cos_2the+2.0*the*sin_2the-4.0*cos_the);
	fij(5, 0) = fij(0, 5) = -kp*R2/EI*(sin_the-the*cos_the);
	fij(5, 1) = fij(1, 5) = kp*R2/EI*(the*sin_the+cos_the-1.0);
	fij(3, 2) = fij(2, 3) = (R2/4.0/GI+kp*R2/2.0/EI)*(sin_the-the*cos_the);
	fij(4, 2) = fij(2, 4) = R2/4.0/GI*(2.0-2.0*cos_the-the*sin_the)-kp*R2/2.0/EI*the*sin_the;
	fij(3, 4) = fij(4, 3) = (R/8.0/GI-kp*R/4.0/EI)*(1.0-cos_2the);
	
	for(int i=0; i<6; i++)H(i, i) = -1.0;
	H(3, 2) = -R*(1.0-cos_the);
	H(4, 2) = R*sin_the;
	H(5, 0) = -R*(cos_the-1.0);
	H(5, 1) = -R*sin_the; 
	sij = fij.inverse();
	stif.block<6, 6>(0, 0) = H*sij*H.transpose();
	stif.block<6, 6>(0, 6) = H*sij;
	stif.block<6, 6>(6, 0) = sij*H.transpose();
	stif.block<6, 6>(6, 6) = sij;

	for(int i: {0, 1, 2, 6, 7, 8})mass(i, i) = 0.5*Me;
	//! From Code-Aster reference formulation.
	// mass(3, 3) = mass(9, 9) = prop.param[0]*Iyy*R*the;
	// mass(4, 4) = mass(10, 10) = 2.*prop.param[0]*Iyy*R*the/15.+prop.param[0]*Ax*R*R*the*the*fmin(R*the/105., 1./48.);
	// mass(5, 5) = mass(11, 11) = 2.*prop.param[0]*Iyy*R*the/15.+prop.param[0]*Ax*R*R*the*the*fmin(R*the/105., 1./48.);
	MatrixXd loc2gbl = MatrixXd::Zero(12, 12);
	for(int i: {0, 1, 2, 3})loc2gbl.block<3, 3>(i*3, i*3) = tt;
	
	MatrixXd tmp = MatrixXd::Zero(3, 3);
	MatrixXd t2 = MatrixXd::Zero(12, 12);
	double cos_b{cos(0.5*the)}, sin_b{sin(0.5*the)};
	tmp.row(0) << cos_b, -sin_b, 0.;
	tmp.row(1) << sin_b, cos_b, 0.;
	tmp.row(2) << 0., 0., 1.;	
	for(int i: {0, 1, 2, 3})t2.block<3, 3>(i*3, i*3) = tmp;
	stif = t2.transpose()*stif*t2;
	mass = t2.transpose()*mass*t2;
	
	return make_tuple(stif, mass, loc2gbl, rhs);
}
}
}