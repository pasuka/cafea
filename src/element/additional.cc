/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#include <Eigen/Dense>
#include <Eigen/Geometry>

#include "cafea/element_lib.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::Vector3d;
using Eigen::Vector3f;
using Eigen::AngleAxisf;
using Eigen::Matrix3f;

namespace cafea {
/**
 *  \brief Mass Element.
 */
template <class T, class U>
elem_out_5<U> StructuralElement<T, U>::mass21(const std::vector<Node<T, U>> pt, const Material<T> *mp,
											  const Section<T> *sect, const int opt[]) {
	assert(1 == pt.size());
	return StructuralElement<T, U>::mass21(&pt[0], mp, sect, opt);
}
/**
 *  \brief Spring Element.
 */
template <class T, class U>
elem_out_5<U> StructuralElement<T, U>::combin14(const std::vector<Node<T, U>> pt, const Material<T> *mp,
												const Section<T> *sect, const int opt[]) {
	assert(2 == pt.size());
	return StructuralElement<T, U>::combin14(&pt[0], &pt[1], mp, sect, opt);
}
/**
 *  \brief Mass.
 */
template <class T, class U>
elem_out_5<U> StructuralElement<T, U>::mass21(const NodeBase<T> *p, const Material<T> *prop,
											  const Section<T> *sect, const int opt[]) {
	matrix_<U> stif = matrix_<U>::Zero(6, 6);
	matrix_<U> mass = matrix_<U>::Zero(6, 6);
	matrix_<U> tran = matrix_<U>::Identity(6, 6);
	vecX_<U>   rhs  = vecX_<U>::Zero(6);
	std::map<std::string, U> attr{{"Length", U(0)}, {"Area", U(0)}, {"Volume", U(0)},
		{"Mass", sect->get_sect_prop(SectionProp::ADDONMASS)}};

	//! Mass on X Y Z direction.
	//! Notice: mass element do not need transform in most cases.
	auto val = sect->get_sect_prop(SectionProp::ADDONMASS);
	if (EPS<U>() < val) {
		for (auto i: {0, 1, 2}) mass(i, i) = val;
	} else {
		if (prop->get_material_type() == MaterialType::MASS_VALUES) {
			auto val_list = prop->get_material_prop_vec();
			for (int i = 0; i < 6; i++) {
				if (EPS<U>() < val_list[i]) mass(i, i) = val_list[i];
			}
		}
	}
	// fmt::print("Mass add-on:{}\n", mass(0, 0));
	return std::make_tuple(stif, mass, tran, rhs, attr);
}

/**
 *  \brief Spring.
 */
template <class T, class U>
elem_out_5<U> StructuralElement<T, U>::combin14(const NodeBase<T> *p1, const NodeBase<T> *p2,
											    const Material<T> *prop, const Section<T> *sect,
												const int opt[]) {
	matrix_<U> stif    = matrix_<U>::Zero(12, 12);
	matrix_<U> mass    = matrix_<U>::Zero(12, 12);
	matrix_<U> loc2gbl = matrix_<U>::Identity(12, 12);
	vecX_<U>   rhs     = vecX_<U>::Zero(12);
	std::map<std::string, U> attr{{"Length", U(0)}, {"Area", U(0)}, {"Volume", U(0)}};

	Eigen::Matrix<T, 3, 3> euler_tran;
	U l;
	matrix_<U> tt = matrix_<U>::Zero(3, 3);
	std::tie(l, tt) = NodeFunc<T, U>::coord_tran(p1, p2);

	if (opt[1] == 0) {
		for (int i: {0, 1, 2, 3}) loc2gbl.block(i*3, i*3, 3, 3) = tt;
		if (prop->get_material_type() == MaterialType::SPRING_STIFFNESS) {
			auto stif_list = prop->get_material_prop_vec();
			if (stif_list[0] > EPS<>()) {
				stif(0, 0) = stif(6, 6) = stif_list[0];
				stif(0, 6) = stif(6, 0) = -stif(0, 0);
			}
			if (stif_list[1] > EPS<>()) {
				stif(3, 3) = stif(9, 9) = stif_list[1];
				stif(3, 9) = stif(9, 3) = -stif(3, 3);
			}
		}
		/*
		for(int i: {0, 1, 2, 6, 7, 8})stif(i, i) = sect->get_sect_prop(SectionProp::ADDONSPRING);
		for(int i: {0, 1, 2})stif(i+6, i) = stif(i, i+6) = -stif(i, i);
		*/
	} else {
		euler_tran = p1->get_euler_tran();
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) loc2gbl(i, j) = loc2gbl(i+3, j+3) = euler_tran(i, j);
		}
		euler_tran = p2->get_euler_tran();
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) loc2gbl(i+6, j+6) = loc2gbl(i+9, j+9) = euler_tran(i, j);
		}
		int m{opt[1]-1};
		assert(0 <= m && m <= 5);
		stif(m, m) = stif(m+6, m+6) = sect->get_sect_prop(SectionProp::ADDONSPRING);
		stif(m+6, m) = stif(m, m+6) = -stif(m, m);
		// fmt::print("Dof:{}\tKe:{}\n", m+1, stif(m, m));
	}

	return std::make_tuple(stif, mass, loc2gbl, rhs, attr);
}
}  // namespace cafea
