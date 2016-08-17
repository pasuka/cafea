#include "cafea.h"

namespace cafea
{
namespace wrapper_
{
/**
 *  \brief Convert Node from Fortran/C to C++
 *  \param[in] p_node node in Fortran/C.
 *  \return Node instance of c++.
 */
template <class Scalar, class ResultScalar>
Node<Scalar, ResultScalar> convert2node(const node_f *p_node)
{
	Node<Scalar, ResultScalar> pt = {p_node->id_, p_node->xyz_[0], p_node->xyz_[1],
		p_node->xyz_[2], p_node->rot_[0], p_node->rot_[1], p_node->rot_[2]};
	switch(p_node->csys_){
	case 1: pt.set_csys(CoordinateSystem::CYLINDRICAL); break;
	case 2: pt.set_csys(CoordinateSystem::SPHERICAL); break;
	case 0:
	default: pt.set_csys(CoordinateSystem::CARTESIAN); break;
	}
	return pt;
};
/**
 *  \brief Convert Element from Fortran/C to C++
 *  \param[in] p_elem element in Fortran/C.
 *  \return Element instance of c++.
 */
template <class Scalar>
Element<Scalar> convert2elem(const elem_f *p_elem)
{
	Element<Scalar> elem = {p_elem->id_, p_elem->prop_[0],
		p_elem->prop_[1]};
	int nn{0};
	switch(p_elem->etype_){
	case 14: elem.set_element_type(ElementType::COMBIN14); nn = 2; break;
	case 16: elem.set_element_type(ElementType::PIPE16); nn = 2; break;
	case 18: elem.set_element_type(ElementType::PIPE18); nn = 3; break;
	case 21: elem.set_element_type(ElementType::MASS21); nn = 1; break;
	case 181: elem.set_element_type(ElementType::SHELL181); nn = 4; break;
	case 185: elem.set_element_type(ElementType::SOLID185); nn = 8; break;
	case 186: elem.set_element_type(ElementType::SOLID186); nn = 20; break;
	case 281: elem.set_element_type(ElementType::SHELL281); nn = 8; break;
	default: elem.set_element_type(ElementType::UNKNOWN);
	}
	if(0<nn)elem.set_node_list(&p_elem->node_list_[0], nn);
	elem.set_option(&p_elem->opt_[0], 10);
	return elem;
};
/**
 *  \brief Convert Material from Fortran/C to C++
 *  \param[in] p_matl material in Fortran/C.
 *  \return Material instance of c++.
 */
template <class Scalar>
Material<Scalar> convert2matl(const matl_f *p_matl)
{
	Material<Scalar> mp(p_matl->id_, MaterialType::LINEAR_ELASTIC);
	mp.set_material_prop(MaterialProp::DENS, p_matl->val_[0]);
	mp.set_material_prop(MaterialProp::EX, p_matl->val_[1]);
	mp.set_material_prop(MaterialProp::NU, p_matl->val_[2]);
	mp.set_material_prop(MaterialProp::GXY, p_matl->val_[3]);
	return mp;
};
/**
 *  \brief Convert Section from Fortran/C to C++
 *  \param[in] p_sect section in Fortran/C.
 *  \return Section instance of c++.
 */
template <class Scalar>
Section<Scalar> convert2sect(const matl_f *p_sect)
{
	Section<Scalar> st(p_sect->id_, SectionType::UNKNOWN);
	st.set_sect_prop(SectionProp::OD, p_sect->val_[0]);
	st.set_sect_prop(SectionProp::TKWALL, p_sect->val_[1]);
	st.set_sect_prop(SectionProp::RADCUR, p_sect->val_[2]);
	return st;
};

//! Specialization
template Node<REAL4, REAL4> convert2node(const node_f*);
template Node<REAL8, REAL8> convert2node(const node_f*);
template Node<REAL4, REAL8> convert2node(const node_f*);
template Node<REAL8, REAL4> convert2node(const node_f*);
//! Specialization
template Element<REAL8> convert2elem(const elem_f*);
template Element<REAL4> convert2elem(const elem_f*);
//! Specialization
template Material<REAL4> convert2matl(const matl_f*);
template Material<REAL8> convert2matl(const matl_f*);
//! Specialization
template Section<REAL4> convert2sect(const matl_f*);
template Section<REAL8> convert2sect(const matl_f*);
}
}