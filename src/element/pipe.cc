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
using varargout = tuple<matrix_<T>, matrix_<T>, matrix_<T>, vecX_<T>, map<string, T>>;
}
/**
 *  \brief Straight Pipe Element No.16
 *
 */
template <class T, class U>
varargout<U> StructuralElement<T, U>::pipe16(const vector<Node<T, U>> pt, const Material<T> *mp,
	const Section<T> *sect, const int *opt)
{
	assert(2==pt.size());
	return StructuralElement<T, U>::pipe16(&pt[0], &pt[1], mp, sect, opt);
}
/**
 *  \brief Straight Pipe Element No.18
 *
 */
template <class T, class U>
varargout<U> StructuralElement<T, U>::pipe18(const vector<Node<T, U>> pt, const Material<T> *mp,
	const Section<T> *sect)
{
	assert(3==pt.size());
	return StructuralElement<T, U>::pipe18(&pt[0], &pt[1], &pt[2], mp, sect);
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
template <class T, class U>
varargout<U> StructuralElement<T, U>::pipe16(
	const NodeBase<T> *p1, const NodeBase<T> *p2,
	const Material<T> *prop, const Section<T> *sect, const int *opt)
{
	matrix_<U> stif = matrix_<U>::Zero(12, 12);
	matrix_<U> mass = matrix_<U>::Zero(12, 12);
	vecX_<U> rhs = vecX_<U>::Zero(12);
	
	U Ro = 0.5*sect->get_sect_prop(SectionProp::OD);
	const decltype(Ro) Ri = Ro - sect->get_sect_prop(SectionProp::TKWALL);
	const decltype(Ro) Ax = PI<U>()*(Ro*Ro-Ri*Ri);
	const decltype(Ro) Jxx = 0.5*PI<U>()*(pow(Ro, 4.0)-pow(Ri, 4.0));
	const decltype(Ro) Izz = 0.5*Jxx;
	const decltype(Ro) Iyy{Izz};
	const decltype(Ro) v = prop->get_material_prop(MaterialProp::PRXY);
	const decltype(Ro) ES = prop->get_material_prop(MaterialProp::YOUNG)*Ax;
	const decltype(Ro) GJx = prop->get_material_prop(MaterialProp::YOUNG)*Jxx*0.5/(1.+v);
	
	decltype(Ro) Le;
	matrix_<U> tt = matrix_<U>::Zero(3, 3);
	tie(Le, tt) = NodeFunc<T, U>::coord_tran(p1, p2);
	
	matrix_<U> loc2gbl = matrix_<U>::Zero(12, 12);
	for(int i: {0, 1, 2, 3})loc2gbl.block(i*3, i*3, 3, 3) = tt;
	// Axial.
	stif(0, 0) = stif(6, 6) = ES/Le;
	stif(6, 0) = stif(0, 6) = -stif(0, 0);
	// Torque. 
	stif(3, 3) = stif(9, 9) = GJx/Le;
	stif(9, 3) = stif(3, 9) = -stif(3, 3);
	
	// ANSYS set 2
	// frame3dd set .54414+2.97294*Ri/Ro-1.51899*Ri*Ri/Ro/Ro
	// The Book "Gas Pulsation And Pipe Vibration Of Piston Compressor"(in Chinese) set 1.885
	const decltype(Ro) alpha = 2.0;
	const decltype(Ro) Ksy = 2.4e1*(1.+v)*Iyy*alpha/(Ax*Le*Le);
	const decltype(Ro) Ksz = Ksy;
	// double Ksz{2.4e1*(1.+v)*Izz*alpha/(Ax*Le*Le)};
	const decltype(Ro) EIz = prop->get_material_prop(MaterialProp::YOUNG)*Izz;
	const decltype(Ro) EIy = prop->get_material_prop(MaterialProp::YOUNG)*Iyy;
	
	// Bending.
	stif( 1, 1) = stif( 7,  7) = 12.*EIy/(Le*Le*Le*(1.+Ksy));
	stif( 5, 5) = stif(11, 11) = (4.+Ksy)*EIy/(Le*(1.+Ksy));
	stif( 5, 1) = stif( 1,  5) = 6.*EIy/(Le*Le*(1.+Ksy));
	stif(11, 5) = stif( 5, 11) = (2.-Ksy)*EIy/(Le*(1.+Ksy));
	stif(11, 7) = stif( 7, 11) = stif(7, 5) = stif(5, 7) = -stif(5, 1);
	stif(11, 1) = stif( 1, 11) = stif(5, 1);
	stif( 7, 1) = stif( 1,  7) = -stif(1, 1);
	// Bending.
	stif( 2, 2) = stif( 8,  8) = 12.*EIz/(Le*Le*Le*(1.+Ksz));
	stif( 4, 4) = stif(10, 10) = (4.+Ksz)*EIz/(Le*(1.+Ksz));
	stif( 4, 2) = stif( 2,  4) = -6.*EIz/(Le*Le*(1.+Ksz));
	stif(10, 4) = stif( 4, 10) = (2.-Ksz)*EIz/(Le*(1.+Ksz));
	stif(10, 8) = stif( 8, 10) = stif(8, 4) = stif(4, 8) = -stif(4, 2);
	stif(10, 2) = stif( 2, 10) = stif(4, 2);
	stif( 8, 2) = stif( 2,  8) = -stif(2, 2);
	
	decltype(Ro) Me = prop->get_material_prop(MaterialProp::DENS)*Ax*Le;
	const decltype(Ro) Je = prop->get_material_prop(MaterialProp::DENS)*Jxx*Le;
	const decltype(Ro) rz = prop->get_material_prop(MaterialProp::DENS)*Izz;
	const decltype(Ro) ry = prop->get_material_prop(MaterialProp::DENS)*Iyy;
	
	// Lumped Mass.
	if(0<opt[0]){
 		mass(0, 0) = mass(6, 6) = Me/2.;
		mass(1, 1) = mass(7, 7) = mass(2, 2) = mass(8, 8) = Me/2.;
		mass(3, 3) = mass(9, 9) = Je/2.;
		mass(4, 4) = mass(10, 10) = ry/2.;
		mass(5, 5) = mass(11, 11) = rz/2.;
	}
    else{ 
		mass( 0, 0) = mass( 6,  6) = Me/3.;
		mass( 0, 6) = mass( 6,  0) = mass(0, 0)/2.;
		
		mass( 3, 3) = mass( 9,  9) = Je/3.;
		mass( 3, 9) = mass( 9,  3) = mass(3, 3)/2.;
		
		mass( 1, 1) = mass( 7,  7) = 13.*Me/35. + 6.*rz/(5.*Le);
		mass( 5, 5) = mass(11, 11) = Me*Le*Le/105. + 2.*Le*rz/15.;
		mass(11, 5) = mass( 5, 11) = -Me*Le*Le/140. - rz*Le/30.;
		mass( 5, 1) = mass( 1,  5) = 11.*Me*Le/210. + rz/10.;
		mass( 7, 5) = mass( 5,  7) = 13.*Me*Le/420. - rz/10.;
		mass( 7, 1) = mass( 1,  7) = 9.*Me/70. - 6.*rz/(5.*Le);
		
		mass(11, 7) = mass( 7, 11) = -mass(5, 1);
		mass(11, 1) = mass( 1, 11) = -mass(5, 7);
		
		mass( 2, 2) = mass( 8,  8) = 13.*Me/35. + 6.*ry/(5.*Le);
		mass( 4, 4) = mass(10, 10) = Me*Le*Le/105. + 2.*Le*ry/15.;
		mass(10, 4) = mass( 4, 10) = -Me*Le*Le/140. - ry*Le/30.;
		mass( 4, 2) = mass( 2,  4) = -11.*Me*Le/210. - ry/10.;
		mass( 8, 4) = mass( 4,  8) = -13.*Me*Le/420. + ry/10.;
		mass( 8, 2) = mass( 2,  8) = 9.*Me/70. - 6.*ry/(5.*Le);
		
		mass(10, 2) = mass( 2, 10) = -mass(4, 8);
		mass(10, 8) = mass( 8, 10) = -mass(4, 2);
	}
	{
		decltype(Ro) fluid_dens = sect->get_sect_prop(SectionProp::DENSFL);
		if(fluid_dens>EPS<>()){
			for(int i: {0, 1, 2, 6, 7, 8}){
				mass(i, i) += fluid_dens*PI<U>()*Ri*Ri*Le*0.5;
			}
			fmt::print("Pipe16 Fluid mass:{}\n", fluid_dens*PI<U>()*Ri*Ri*Le);
		}
	}
	rhs(0) = -PI<U>()*Ri*Ri*(1.-2.*v)*sect->get_sect_prop(SectionProp::PRESIN);
	rhs(6) = -rhs(0);
	
	decltype(Ro) SIF = U(1);
	
	map<string, U> attr{{"Length", Le}, {"Area", Ax}, {"Volume", Ax*Le}, {"Mass", Me}, {"Aw", Ax},
		{"Thick", Ro-Ri}, {"OuterDiameter", U(2)*Ro}, {"InnerDiameter", U(2)*Ri}, {"Iy", Iyy},
		{"Iz", Izz}, {"Jx", Jxx}, {"InternalPressure", sect->get_sect_prop(SectionProp::PRESIN)},
		{"StressIntensificationFactor", SIF},
		{"CurvatureRadius", U(-1)}, {"Angle", U(-1)},};
	
	return make_tuple(stif, mass, loc2gbl, rhs, attr);
}

/**
 *	\brief Curved Pipe Element No.18  
 */
template <class T, class U>
varargout<U> StructuralElement<T, U>::pipe18(
	const NodeBase<T> *p1, const NodeBase<T> *p2, const NodeBase<T> *cen,
	const Material<T> *prop, const Section<T> *sect)
{
	matrix_<U> stif = matrix_<U>::Zero(12, 12);
	matrix_<U> mass = matrix_<U>::Zero(12, 12);
	vecX_<U> rhs = vecX_<U>::Zero(12);
	
	matrix_<U> fij = matrix_<U>::Zero(6, 6);
	matrix_<U> sij = matrix_<U>::Zero(6, 6);
	matrix_<U> H = matrix_<U>::Zero(6, 6);
	
	U Ro = 0.5*sect->get_sect_prop(SectionProp::OD);
	const decltype(Ro) Ri = Ro-sect->get_sect_prop(SectionProp::TKWALL);
	const decltype(Ro) t = sect->get_sect_prop(SectionProp::TKWALL);
	const decltype(Ro) Ax = PI<U>()*(Ro*Ro-Ri*Ri);
	const decltype(Ro) Jxx = 0.5*PI<U>()*(pow(Ro, 4.0)-pow(Ri, 4.0));
	const decltype(Ro) Iyy = 0.5*Jxx;
	const decltype(Ro) R = sect->get_sect_prop(SectionProp::RADCUR);
	const decltype(Ro) R2 = R*R;
	const decltype(Ro) R3 = R2*R;
	const decltype(Ro) v = prop->get_material_prop(MaterialProp::PRXY);
	
	matrix_<U> tt = matrix_<U>::Zero(3, 3);
	
	vec3_<T> vxx, vyy, vzz, vxy;
	vxy = cen->get_xyz() - p1->get_xyz();
	vxx = p2->get_xyz() - p1->get_xyz();
	
	const decltype(Ro) la = vxx.norm(), lb = vxy.norm();
	vxx /= vxx.norm();
	vzz = vxx.cross(vxy);
	vzz /= vzz.norm();
	vyy = vzz.cross(vxx);
	vyy /= vyy.norm();
	
	tt.row(0) << vxx(0), vxx(1), vxx(2);
	tt.row(1) << vyy(0), vyy(1), vyy(2);
	tt.row(2) << vzz(0), vzz(1), vzz(2);
	
	matrix_<U> loc2gbl = matrix_<U>::Zero(12, 12);
	for(int i: {0, 1, 2, 3})loc2gbl.block(i*3, i*3, 3, 3) = tt;
	
	const decltype(Ro) the = 2.0*asin(.5*la/R);
	const decltype(Ro) l = R*the, cos_the = cos(the), sin_the = sin(the);
	const decltype(Ro) cos_2the = cos(2.0*the), sin_2the = sin(2.0*the);
	
	// see also PIPE16
	// double alpha{.54414+2.97294*Ri/Ro-1.51899*Ri*Ri/Ro/Ro};
	// double alpha{1.885};
	const decltype(Ro) alpha = 2.;
	
	const decltype(Ro) EA = prop->get_material_prop(MaterialProp::YOUNG)*Ax;
	const decltype(Ro) GA = prop->get_material_prop(MaterialProp::YOUNG)*Ax*.5/(1.0+v);
	const decltype(Ro) EI = prop->get_material_prop(MaterialProp::YOUNG)*Iyy;
	const decltype(Ro) GI = prop->get_material_prop(MaterialProp::YOUNG)*Iyy*.5/(1.0+v);
	
	decltype(Ro) Me = prop->get_material_prop(MaterialProp::DENS)*Ax*l;
	{
		decltype(Ro) fluid_dens = sect->get_sect_prop(SectionProp::DENSFL);
		if(fluid_dens>EPS<>())Me += fluid_dens*PI<U>()*Ri*Ri*l;
		if(fluid_dens>EPS<>())fmt::print("PIPE18 Fluid mass:{}\n", fluid_dens*PI<U>()*Ri*Ri*l);
	}
	
	decltype(Ro) kp;
	{
		decltype(Ro) r = Ro-.5*t, h = t*R/(r*r), pr = sect->get_sect_prop(SectionProp::PRESIN);
		decltype(Ro) Xk;
		if(1.7>(R/r)){
			Xk = 0.;
		}
		else{
			Xk = pow(r/t, 4./3.)*pow(R/r, 1./3.);
		}
		// kp = 1.65/(h*(1.+6.*pr*Xk/(prop->get_material_prop(MaterialProp::YOUNG)*t)));
		kp = 1.65/h;		
		if(kp<1.)kp = 1.;
	}
	
	// Flexibility sub-matrix.
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
	stif.block(0, 0, 6, 6) = H*sij*H.transpose();
	stif.block(0, 6, 6, 6) = H*sij;
	stif.block(6, 0, 6, 6) = sij*H.transpose();
	stif.block(6, 6, 6, 6) = sij;
	{
		auto r = Ro-0.5*t;
		auto pres = sect->get_sect_prop(SectionProp::PRESIN);
		auto young = prop->get_material_prop(MaterialProp::YOUNG);
		
		vecX_<U> B = vecX_<U>::Zero(6);
		// ANSYS PIPE 18 element ignore angle changes.
		/*
		auto DU2 = r/R;
		auto BTA = DUM*(2.-2.*v+(3.+1.5*v)*pow(DU2, 2.0));
		auto DUM = PI<U>()*pres*the*pow(r, 4.0)*0.5/EI;
		BTA = pres*r*the/(2.*t*young+(2.-v)*pres*r);
		DUM = R*BTA/the;
		B(0) = DUM*(sin_the - the*cos_the);
		B(1) = DUM*(1.0 - cos_the - the*sin_the);
		B(5) = -BTA;
		*/

		auto DUM = pres*(1.-2.*v)*Ri*Ri*R/(Ro+Ri)/(young*t);
		B(0) += DUM*sin_the;
		B(1) += DUM*(1.0-cos_the);
		rhs = stif.block(0, 6, 12, 6)*B;
	}	
	matrix_<U> tmp = matrix_<U>::Zero(3, 3);
	matrix_<U> t2 = matrix_<U>::Zero(12, 12);
	const decltype(Ro) cos_b = cos(.5*the), sin_b = sin(.5*the);
	
	tmp.row(0) << cos_b, -sin_b, 0.;
	tmp.row(1) << sin_b,  cos_b, 0.;
	tmp.row(2) <<    0.,     0., 1.;	
	
	for(int i: {0, 1, 2, 3})t2.block(i*3, i*3, 3, 3) = tmp;
	stif = t2.transpose()*stif*t2;
	{
		rhs = t2.transpose()*rhs;
	}
	
	for(int i: {0, 1, 2, 6, 7, 8})mass(i, i) = 0.5*Me;
	//! From Code-Aster reference formulation.
	// mass(3, 3) = mass(9, 9) = prop.param[0]*Iyy*R*the;
	// mass(4, 4) = mass(10, 10) = 2.*prop.param[0]*Iyy*R*the/15.+prop.param[0]*Ax*R*R*the*the*fmin(R*the/105., 1./48.);
	// mass(5, 5) = mass(11, 11) = 2.*prop.param[0]*Iyy*R*the/15.+prop.param[0]*Ax*R*R*the*the*fmin(R*the/105., 1./48.);
	
	decltype(Ro) SIF = U(1);
	{
		decltype(Ro) he = U(4)*t*R/pow(Ro+Ri, 2.0);
		decltype(Ro) val = U(0.9)/pow(he, 2./3.);
		if(val>1.0)SIF = val;
	}
	map<string, U> attr{{"Length", l}, {"Area", Ax}, {"Volume", Ax*l}, {"Mass", Me}, {"Aw", Ax}, 
		{"Thick", t}, {"OuterDiameter", U(2)*Ro}, {"InnerDiameter", U(2)*Ri}, {"Iy", Iyy},
		{"Iz", Iyy}, {"Jx", Jxx}, {"CurvatureRadius", R}, {"Angle", the},
		{"InternalPressure", sect->get_sect_prop(SectionProp::PRESIN)},
		{"StressIntensificationFactor", SIF},};
	
	return make_tuple(stif, mass, loc2gbl, rhs, attr);
}

/**
 * \brief Post-Process of element pipe. 
 */
template <class T>
matrix_<T> StructuralElementPost<T>::pipe(const matrix_<T> stif, const matrix_<T> tran,
	const matrix_<T> x, const matrix_<T> rhs, const map<string, T> attr)
{
	fmt::print("This is for pipe post process in real domain.\n");
	matrix_<T> esol = matrix_<T>::Zero(99, 2);
	matrix_<T> tmp = stif*tran*x;
	// Element force in local.
	tmp.col(0) = rhs.col(0) - tmp.col(0);
	
	auto get_val = [=](auto k){auto p = attr.find(k); return p!=attr.end() ? p->second: EPS<T>();};
	
	// Elbow pipe.
	auto the = get_val("Angle");
	if(the>EPS<T>()){
		auto cb = cos(0.5*the);
		auto sb = sin(0.5*the);
		matrix_<T> tran = matrix_<T>::Zero(12, 12);
		for(int i: {2, 5, 8, 11})tran(i, i) = 1.0;
		for(int i: {0, 1, 3, 4, 6, 7, 9, 10})tran(i, i) = cb;
		tran(1, 0) = tran(4, 3) = tran(6, 7) = tran(9, 10) =  sb;
		tran(0, 1) = tran(3, 4) = tran(7, 6) = tran(10, 9) = -sb;
		tmp = tran*tmp;
	}
	
	// Member force and moment at node I and J in local coordinate.
	esol.block(0, 0, 6, 1) = tmp.block(0, 0, 6, 1);
	esol.block(0, 1, 6, 1) = tmp.block(6, 0, 6, 1);
	
	auto Aw   = get_val("Aw");
	auto Pres = get_val("InternalPressure");
	auto Dout = get_val("OuterDiameter");
	auto Din  = get_val("InnerDiameter");
	auto Ir   = get_val("Iy");
	auto SIF  = get_val("StressIntensificationFactor");
	
	auto Ro = Dout/T(2); 
	auto Ri = Din/T(2);
	auto t = Ro - Ri;
	auto PresOut = T(0);
	auto Fe = PI<T>()*(Pres*Ri*Ri-PresOut*Ro*Ro);
	auto Jx = T(2)*Ir;
	/*
	ANSYS stress result.
	|	    |:   pipe #16 :|:   pipe #18 :|
	|:SDIR: |:SMISC 13, 15:|:SMISC 13, 15:|
	|:SBEND:|:NMISC 90, 92:|:NMISC 91, 93:|
	|:ST:   |:SMISC 14, 16:|:SMISC 14, 16:|
	|:SSF:  |:NMISC 91, 93:|:NMISC 92, 94:|
	*/
	for(auto i: {0, 1}){
		// SDIR: Direct stress.
		// esol(6, i) = esol(0, i)/Aw;
		esol(6, i) = (esol(0, i) + Fe)/Aw;
		// SBEND: Bending stress.
		esol(7, i) = SIF*hypot(esol(4, i), esol(5, i))*Ro/Ir;
		// ST: Torsional shear stress.
		esol(8, i) = fabs(esol(3, i))*Ro/Jx;
		// SH: hoop pressure stress at the outside surface of the pipe.
		esol(9, i) = (T(2)*Pres*Din*Din - PresOut*(Dout*Dout + Din*Din))/(Dout*Dout - Din*Din);
		// SSF: lateral force shear stress.
		esol(10, i) = T(2)*hypot(esol(1, i), esol(2, i))/Aw;
		for(int j=0; j<8; j++){
			auto phi = .25*T(j)*PI<T>();
			// SAXL: axial stress on outside surface.
			esol(11+j, i) = esol(6, i) + sin(phi)*esol(7, i);
			// SXH: hoop stress on outside surface.
			esol(19+j, i) = esol(8, i) + cos(phi)*esol(10, i);
			// Center of Mohr circle.
			auto xc = .5*(esol(9, i) + esol(11+j, i));
			// Radius of Mohr circle.
			auto rr = .5*hypot(esol(9, i) - esol(11+j, i), 2.*esol(19+j, i));
			// S1MX: the largest of the four maximum principal stresses.
			esol(27+j, i) = xc + rr;
			// S2MN: the smallest of the four minimum principal stresses.
			esol(35+j, i) = xc - rr;
			// Von Mises = sqrt(.5*((s1-s2)**2+(s2-s3)**2+(s3-s1)**2)).
			// S3 .eq. zero.
			// Von Mises = sqrt(s1*s1 + s2*s2 - s1*s2).
			// Von Mises = sqrt(xc*xc + 3*rr*rr).
			// SEQVMX: the largest of the four equivalent stresses.
			esol(43+j, i) = hypot(xc, sqrt(3.)*rr);
		}
	}
	/* for(int i=0; i<11; i++){
		// esol(i, 0) = tmp(i, 0);
		// esol(i, 1) = tmp(i+6, 0);
		switch(i){
		case  0: fmt::print("MFORX I:{}\tJ:{}\n", esol(i, 0), esol(i, 1)); break;
		case  1: fmt::print("MFORY I:{}\tJ:{}\n", esol(i, 0), esol(i, 1)); break;
		case  2: fmt::print("MFORZ I:{}\tJ:{}\n", esol(i, 0), esol(i, 1)); break;
		case  3: fmt::print("MMOMX I:{}\tJ:{}\n", esol(i, 0), esol(i, 1)); break;
		case  4: fmt::print("MMOMY I:{}\tJ:{}\n", esol(i, 0), esol(i, 1)); break;
		case  5: fmt::print("MMOMZ I:{}\tJ:{}\n", esol(i, 0), esol(i, 1)); break;
		case  6: fmt::print("SDIR  I:{}\tJ:{}\n", esol(i, 0), esol(i, 1)); break;
		case  7: fmt::print("SBEND I:{}\tJ:{}\n", esol(i, 0), esol(i, 1)); break;
		case  8: fmt::print("ST    I:{}\tJ:{}\n", esol(i, 0), esol(i, 1)); break;
		case  9: fmt::print("SH    I:{}\tJ:{}\n", esol(i, 0), esol(i, 1)); break;
		case 10: fmt::print("SSF   I:{}\tJ:{}\n", esol(i, 0), esol(i, 1)); break;
		default: fmt::print("Default.\n");
		}
	} */
	
	return esol;
}


}