/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#include <tuple>
#include <iostream>
#include <cmath>
#include <Eigen/Dense>
#include <Eigen/Geometry>

#include "cafea/cafea.h"

using std::tie;
using std::map;
using std::tuple;
using std::string;
using std::make_tuple;

using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::Vector3d;

namespace cafea {
namespace {
template <class T>
using varargout = tuple<matrix_<T>, matrix_<T>, matrix_<T>, vecX_<T>, map<string, T>>;
}

/**
 *
 */
template <class T, class U>
varargout<U> StructuralElement<T, U>::shell8r(const vector<Node<T, U>> pt, const Material<T> *mp, const Section<T> *sect) {
	assert(8 == pt.size());
	return StructuralElement<T, U>::shell8r(&pt[0], &pt[1], &pt[2], &pt[3], &pt[4], &pt[5], &pt[6], &pt[7], mp, sect);
}

/*
 *
 */
template <class T, class U>
varargout<U> StructuralElement<T, U>::shell9r(const vector<Node<T, U>> pt, const Material<T> *mp, const Section<T> *sect) {
	assert(9 == pt.size());
	return StructuralElement<T, U>::shell9R(&pt[0], &pt[1], &pt[2], &pt[3], &pt[4], &pt[5], &pt[6], &pt[7], &pt[8], mp, sect);
}

/**
 *  \brief
 */
template <class T, class U>
varargout<U> StructuralElement<T, U>::shell8r(const NodeBase<T> *p1, const NodeBase<T> *p2, const NodeBase<T> *p3,
											  const NodeBase<T> *p4, const NodeBase<T> *p5, const NodeBase<T> *p6,
											  const NodeBase<T> *p7, const NodeBase<T> *p8, const Material<T> *prop,
											  const Section<T> *sect) {
	const int nn{48};
	matrix_<U> stif = matrix_<U>::Zero(nn, nn);
	matrix_<U> mass = matrix_<U>::Zero(nn, nn);
	matrix_<U> loc2gbl = matrix_<U>::Zero(nn, nn);
	vecX_<U> rhs = vecX_<U>::Zero(nn);

	U area;
	matrix_<U> xy = matrix_<U>::Zero(4, 2);
	matrix_<U> tt = matrix_<U>::Zero(3, 3);
	tie(area, xy, tt) = NodeFunc<T, U>::coord_tran(p1, p2, p3, p4);
	for (int i = 0; i < 16; i++) loc2gbl.block(i*3, i*3, 3, 3) = tt;

	map<string, U> attr{{"Length", 0.0}, {"Area", area}, {"Volume", area*t}};

	return make_tuple(stif, mass, loc2gbl, rhs, attr);
}

/**
 *  \brief
 */
template <class T, class U>
varargout<U> StructuralElement<T, U>::shell9r(const NodeBase<T> *p1, const NodeBase<T> *p2, const NodeBase<T> *p3,
											  const NodeBase<T> *p4, const NodeBase<T> *p5, const NodeBase<T> *p6,
											  const NodeBase<T> *p7, const NodeBase<T> *p8, const NodeBase<T> *p9,
											  const Material<T> *prop, const Section<T> *sect) {
	const int nn{54};
	matrix_<U> stif = matrix_<U>::Zero(nn, nn);
	matrix_<U> mass = matrix_<U>::Zero(nn, nn);
	matrix_<U> loc2gbl = matrix_<U>::Zero(nn, nn);
	vecX_<U> rhs = vecX_<U>::Zero(nn);

	U area;
	matrix_<U> xy = matrix_<U>::Zero(4, 2);
	matrix_<U> tt = matrix_<U>::Zero(3, 3);
	tie(area, xy, tt) = NodeFunc<T, U>::coord_tran(p1, p2, p3, p4);
	for (int i = 0; i < 16; i++) loc2gbl.block(i*3, i*3, 3, 3) = tt;

	map<string, U> attr{{"Length", 0.0}, {"Area", area}, {"Volume", area*t}};

	return make_tuple(stif, mass, loc2gbl, rhs, attr);
}
}  // namespace cafea
