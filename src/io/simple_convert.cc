/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#include "cafea/cafea.h"

namespace cafea {
namespace wrapper_ {
/**
 *  \brief Convert Node from Fortran/C to C++
 *  \param[in] p_node node in Fortran/C.
 *  \return Node instance of c++.
 */
template <class T, class U>
Node<T, U> AdapterF2Cpp<T, U>::cdb2node(const node_f *p_node) {
	Node<T, U> pt = {p_node->id_, p_node->xyz_[0], p_node->xyz_[1],
		p_node->xyz_[2], p_node->rot_[0], p_node->rot_[1], p_node->rot_[2]};
	switch (p_node->csys_) {
		case 1:
			pt.set_csys(CoordinateSystem::CYLINDRICAL);
			break;
		case 2:
			pt.set_csys(CoordinateSystem::SPHERICAL);
			break;
		case 0:
		default: pt.set_csys(CoordinateSystem::CARTESIAN);
	}
	return pt;
}
/**
 *  \brief Convert Element from Fortran/C to C++
 *  \param[in] p_elem element in Fortran/C.
 *  \return Element instance of c++.
 */
template <class T, class U>
Element<U> AdapterF2Cpp<T, U>::cdb2elem(const elem_f *p_elem) {
	Element<U> elem = {p_elem->id_, p_elem->prop_[0], p_elem->prop_[1]};
	int nn{0};
	switch (p_elem->etype_) {
		case 14:
			elem.set_element_type(ElementType::COMBIN14);
			nn = 2;
			break;
		case 16:
			elem.set_element_type(ElementType::PIPE16);
			nn = 2;
			break;
		case 18:
			elem.set_element_type(ElementType::PIPE18);
			nn = 3;
			break;
		case 21:
			elem.set_element_type(ElementType::MASS21);
			nn = 1;
			break;
		case 181:
			elem.set_element_type(ElementType::SHELL181);
			nn = 4;
			break;
		case 185:
			elem.set_element_type(ElementType::SOLID185);
			nn = 8;
			break;
		case 186:
			elem.set_element_type(ElementType::SOLID186);
			nn = 20;
			break;
		case 281:
			elem.set_element_type(ElementType::SHELL281);
			nn = 8;
			break;
		default: elem.set_element_type(ElementType::UNKNOWN);
	}
	if (0 < nn) elem.set_node_list(&p_elem->node_list_[0], nn);
	elem.set_option(&p_elem->opt_[0], 10);
	return elem;
}
/**
 *  \brief Convert Material from Fortran/C to C++
 *  \param[in] p_matl material in Fortran/C.
 *  \return Material instance of c++.
 */
template <class T, class U>
Material<T> AdapterF2Cpp<T, U>::cdb2matl(const matl_f *p_matl) {
	Material<T> mp(p_matl->id_, MaterialType::LINEAR_ELASTIC);
	mp.set_material_prop(MaterialProp::DENS, p_matl->val_[0]);
	mp.set_material_prop(MaterialProp::EX, p_matl->val_[1]);
	mp.set_material_prop(MaterialProp::NU, p_matl->val_[2]);
	mp.set_material_prop(MaterialProp::GXY, p_matl->val_[3]);
	return mp;
}
/**
 *  \brief Convert Section from Fortran/C to C++
 *  \param[in] p_sect section in Fortran/C.
 *  \return Section instance of c++.
 */
template <class T, class U>
Section<T> AdapterF2Cpp<T, U>::cdb2sect(const matl_f *p_sect) {
	Section<T> st(p_sect->id_, SectionType::UNKNOWN);
	st.set_sect_prop(SectionProp::OD, p_sect->val_[0]);
	st.set_sect_prop(SectionProp::TKWALL, p_sect->val_[1]);
	st.set_sect_prop(SectionProp::RADCUR, p_sect->val_[2]);
	if (p_sect->val_[5] > p_sect->val_[6]) {
		st.set_sect_prop(SectionProp::DENSFL, p_sect->val_[5]);
	} else {
		st.set_sect_prop(SectionProp::DENSFL, p_sect->val_[6]);
	}
	return st;
}

/**
 *  \brief Convert Node from Fortran/C to C++.
 */
template <class T, class U>
Node<T, U> AdapterF2Cpp<T, U>::bcy2node(const node_bcy *p) {
	Node<T, U> pt{p->id_, p->xyz_[0], p->xyz_[1], p->xyz_[2], p->rot_[0], p->rot_[1], p->rot_[2]};
	switch (p->csys_) {
		case 1:
			pt.set_csys(CoordinateSystem::CYLINDRICAL);
			break;
		case 2:
			pt.set_csys(CoordinateSystem::SPHERICAL);
			break;
		case 0:
		default: pt.set_csys(CoordinateSystem::CARTESIAN);
	}
	return pt;
}
/**
 *  \brief Convert Element from Fortran/C to C++.
 */
template <class T, class U>
Element<U> AdapterF2Cpp<T, U>::bcy2elem(const elem_bcy *p) {
	Element<U> elem {p->id_, p->mtype_, p->stype_};
	int nn{0};
	switch (p->etype_) {
		case 14:
			elem.set_element_type(ElementType::COMBIN14);
			nn = 2;
			break;
		case 16:
			elem.set_element_type(ElementType::PIPE16);
			nn = 2;
			break;
		case 18:
			elem.set_element_type(ElementType::PIPE18);
			nn = 3;
			break;
		case 21:
			elem.set_element_type(ElementType::MASS21);
			nn = 1;
			break;
		case 181:
			elem.set_element_type(ElementType::SHELL181);
			nn = 4;
			break;
		case 185:
			elem.set_element_type(ElementType::SOLID185);
			nn = 8;
			break;
		case 186:
			elem.set_element_type(ElementType::SOLID186);
			nn = 20;
			break;
		case 281:
			elem.set_element_type(ElementType::SHELL281);
			nn = 8;
			break;
		default: elem.set_element_type(ElementType::UNKNOWN);
	}
	if (0 < nn) elem.set_node_list(&p->node_list_[0], nn);
	return elem;
}
/**
 *  \brief Convert Material from Fortran/C to C++.
 */
template <class T, class U>
Material<T> AdapterF2Cpp<T, U>::bcy2matl(const matl_bcy *p) {
	Material<T> mp(p->id_, MaterialType::LINEAR_ELASTIC);
	switch (p->tp_) {
		case 14:
			mp.set_material_type(MaterialType::SPRING_STIFFNESS);
			break;
		case 21:
			mp.set_material_type(MaterialType::MASS_VALUES);
			break;
		case 1:
		default: fmt::print("Default linear elastic material.\n");
	}
	mp.set_material_prop(MaterialProp::DENS, p->val_[0]);
	mp.set_material_prop(MaterialProp::EX, p->val_[1]);
	mp.set_material_prop(MaterialProp::GXY, p->val_[2]);
	mp.set_material_prop(MaterialProp::NU, p->val_[3]);
	mp.set_material_prop(MaterialProp::DMPR, p->val_[4]);
	mp.set_material_prop(MaterialProp::ALPD, p->val_[5]);
	mp.set_material_prop(MaterialProp::BETD, p->val_[6]);
	return mp;
}
/**
 *  \brief Convert Section from Fortran/C to C++.
 */
template <class T, class U>
Section<T> AdapterF2Cpp<T, U>::bcy2sect(const sect_bcy *p) {
	Section<T> st(p->id_, SectionType::UNKNOWN);
	switch (p->tp_) {
		case 14:
			st.set_sect_type(SectionType::SPRING);
			break;
		case 16:
		case 18:
			st.set_sect_type(SectionType::PIPE);
			break;
		case 21:
			st.set_sect_type(SectionType::MASS);
			break;
		default: fmt::print("Unsupported section type.\n");
	}
	st.set_sect_prop(SectionProp::OD, p->val_[0]);
	st.set_sect_prop(SectionProp::TKWALL, p->val_[1]);
	st.set_sect_prop(SectionProp::RADCUR, p->val_[2]);
	st.set_sect_prop(SectionProp::DENSFL, p->val_[3]);
	st.set_sect_prop(SectionProp::PRESIN, p->val_[4]);
	return st;
}
/**
 *  \brief Convert Boundary from Fortran/C to C++.
 */
template <class T, class U>
Boundary<T> AdapterF2Cpp<T, U>::bcy2bndy(const bndy_bcy *p) {
	Boundary<T> bc(p->id_, BoundaryType::FIXED, DofLabel::ALL);
	return bc;
}
/**
 *  \brief Convert Load from Fortran/C to C++.
 */
template <class T, class U>
Load<T> AdapterF2Cpp<T, U>::bcy2load(const load_bcy *p) {
	/*Load<T> ld(p->id_,);
	return ld;*/
}
}  // namespace wrapper_
}  // namespace cafea
