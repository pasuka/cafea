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
template <class T>
varargout_2_<double> coord_tran(const NodeBase<T> *p1, const NodeBase<T> *p2)
{
	double length{0};
	matrix_<double> tran = matrix_<double>::Zero(3, 3);
	vec3_<T> vxx, vxy, vyy, vzz;
	
	vxx = p2->get_xyz() - p1->get_xyz();
	length = vxx.norm();
	assert(length>(EPS<T>()));
	vxx /= length;
	
	// auto A{sqrt(vxx(0)*vxx(0)+vxx(1)*vxx(1))};
	auto A = hypot(vxx(0), vxx(1));
	if(A<T(1.e-6)){
		tran(0, 2) = vxx(2)/fabs(vxx(2));
		tran(1, 1) = T(1);
		tran(2, 0) = -vxx(2)/fabs(vxx(2));
	}
	else{
		tran.row(0) << vxx(0), vxx(1), vxx(2);
		tran.row(1) << -vxx(1)/A, vxx(0)/A, T(0);
		tran.row(2) << -vxx(0)*vxx(2)/A, -vxx(1)*vxx(2)/A, A;
	}
	
	return make_tuple(length, tran);
}

/**
 *  \brief Coordinate transform for 2-node beam with up-axis.
 */
template <class T>
varargout_2_<double> coord_tran(const NodeBase<T> *p1, const NodeBase<T> *p2, const T up[])
{
	double length{0};
	matrix_<double> tran = matrix_<double>::Zero(3, 3);
	vec3_<T> vxx, vyy, vzz, vxy;
	
	vxx = p2->get_xyz() - p1->get_xyz();
	vxy << up[0], up[1], up[2];
	
	length = vxx.norm();
	
	assert(length>(EPS<T>()));
	
	vxx /= length;
	vxy /= vxy.norm();
	
	auto the{acos(vxx.dot(vxy))*T(180)/PI<T>()};
	if(the<T(1))vxy << T(1), T(0), T(1);
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
template <class T>
varargout_3_<double> coord_tran(const NodeBase<T> *p1, const NodeBase<T> *p2, const NodeBase<T> *p3)
{
	double area{0};
	matrix_<double> tran = matrix_<double>::Zero(3, 3);
	matrix_<double> xy = matrix_<double>::Zero(3, 2);
	vec3_<T> vxx, vyy, vzz, vxy;
	
	vxx = p2->get_xyz() - p1->get_xyz();
	vxy = p3->get_xyz() - p1->get_xyz();
	
	auto L12 = vxx.norm();
	auto L13 = vxy.norm();
	
	assert(L12>(EPS<T>()));
	assert(L13>(EPS<T>()));
	
	vxx /= L12;
	vxy /= L13;
	
	vzz = vxx.cross(vxy);
	
	vyy = vzz.cross(vxx);
	
	vyy /= vyy.norm();
	vzz /= vzz.norm();
	tran.row(0) << vxx(0), vxx(1), vxx(2);
	tran.row(1) << vyy(0), vyy(1), vyy(2);
	tran.row(2) << vzz(0), vzz(1), vzz(2);
	vxy = p3->get_xyz() - p2->get_xyz();
	auto L23 = vxy.norm();
	
	assert(L23>(EPS<T>()));
	auto cos_t = (L12*L12+L13*L13-L23*L23)/(T(2)*L12*L13);
	auto alpha = acos(cos_t);
	
	xy.row(1) << L12, 0.0;
	xy.row(2) << L13*cos_t, L13*sin(alpha);
	area = L12*L13*sin(alpha)*0.5;
	
	return make_tuple(area, xy, tran);
}

/**
 *  \brief Coordinate transform for 4-node quadrangle element.
 */
template <class T>
varargout_3_<double> coord_tran(const NodeBase<T> *p1, const NodeBase<T> *p2,
	const NodeBase<T> *p3, const NodeBase<T> *p4) 
{
	double area{0};
	matrix_<double> tran = matrix_<double>::Zero(3, 3); 
	matrix_<double> xy = matrix_<double>::Zero(4, 2);
	vec3_<T> vxx, vyy, vzz, vxy;

	vxx = p2->get_xyz() - p1->get_xyz();
	vxy = p3->get_xyz() - p1->get_xyz();
	
	auto L12 = vxx.norm();
	auto L13 = vxy.norm();
	
	assert(L12>(EPS<T>()));
	assert(L13>(EPS<T>()));
	
	vxx /= L12;
	vxy /= L13;
	
	vzz = vxx.cross(vxy);
	vyy = vzz.cross(vxx);
	vyy /= vyy.norm();
	vzz /= vzz.norm();
	tran.row(0) << vxx(0), vxx(1), vxx(2);
	tran.row(1) << vyy(0), vyy(1), vyy(2);
	tran.row(2) << vzz(0), vzz(1), vzz(2);
	
	vxy = p3->get_xyz() - p2->get_xyz();
	auto L23 = vxy.norm();
	
	assert(L23>(EPS<T>()));
	
	auto alpha213 = acos((L12*L12+L13*L13-L23*L23)/(2.0*L12*L13));
	
	vxy = p4->get_xyz() - p1->get_xyz();
	auto L14 = vxy.norm();
	
	assert(L14>(EPS<T>()));
	
	vxy = p4->get_xyz() - p3->get_xyz();
	auto L34 = vxy.norm();
	
	assert(L34>(EPS<T>()));
	
	auto alpha314 = acos((L13*L13+L14*L14-L34*L34)/(2.0*L13*L14));
	auto alpha41y = PI<T>()*0.5 - alpha213 - alpha314;
	
	xy.row(0) << 0.0, 0.0;
	xy.row(1) << L12, 0.0;
	xy.row(2) << L13*cos(alpha213), L13*sin(alpha213);
	xy.row(3) << L14*sin(alpha41y), L14*cos(alpha41y);
	
	area = (L12*L13*sin(alpha213)+L13*L14*sin(alpha314))*0.5;

	return make_tuple(area, xy, tran);
}
//! Specialization with REAL4/float type.
template varargout_2_<REAL8> coord_tran<REAL4>(
	const NodeBase<REAL4>*, const NodeBase<REAL4>*);
//! Specialization with REAL4/float type.	
template varargout_2_<REAL8> coord_tran<REAL4>(
	const NodeBase<REAL4>*, const NodeBase<REAL4>*, const REAL4[]);
//! Specialization with REAL4/float type.
template varargout_3_<REAL8> coord_tran<REAL4>(
	const NodeBase<REAL4>*, const NodeBase<REAL4>*, const NodeBase<REAL4>*);
//! Specialization with REAL4/float type.
template varargout_3_<REAL8> coord_tran<REAL4>(
	const NodeBase<REAL4>*, const NodeBase<REAL4>*,
	const NodeBase<REAL4>*, const NodeBase<REAL4>*);
//! Specialization with REAL8/double type.
template varargout_2_<REAL8> coord_tran<REAL8>(
	const NodeBase<REAL8>*, const NodeBase<REAL8>*);
//! Specialization with REAL8/double type.
template varargout_2_<REAL8> coord_tran<REAL8>(
	const NodeBase<REAL8>*, const NodeBase<REAL8>*, const REAL8[]);
//! Specialization with REAL8/double type.
template varargout_3_<REAL8> coord_tran<REAL8>(
	const NodeBase<REAL8>*, const NodeBase<REAL8>*, const NodeBase<REAL8>*);
//! Specialization with REAL8/double type.
template varargout_3_<REAL8> coord_tran<REAL8>(
	const NodeBase<REAL8>*, const NodeBase<REAL8>*,
	const NodeBase<REAL8>*, const NodeBase<REAL8>*);
}