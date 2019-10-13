/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#include "cafea/base/material.hpp"

namespace cafea {
/**
 *  \brief Set property value of material.
 *  \param[in] mp material property enum.
 *  \param[in] val value of material property.
 */
template <class Scalar, typename U>
void Material<Scalar, U>::set_material_prop(MaterialProp mp, Scalar val) {
	switch (mp) {
		case(MaterialProp::DENS): this->param_[0] = val; break;
		case(MaterialProp::YOUNG):
		case(MaterialProp::EX):
		case(MaterialProp::EY):
		case(MaterialProp::EZ):   this->param_[1] = val; break;
		case(MaterialProp::GXY):
		case(MaterialProp::GYZ):
		case(MaterialProp::GXZ):  this->param_[2] = val; break;
		case(MaterialProp::NU):
		case(MaterialProp::NUXY):
		case(MaterialProp::NUYZ):
		case(MaterialProp::NUXZ):
		case(MaterialProp::PRXY):
		case(MaterialProp::PRYZ):
		case(MaterialProp::PRXZ): this->param_[3] = val; break;
		case(MaterialProp::DMPR): this->param_[4] = val; break;
		case(MaterialProp::ALPD): this->param_[5] = val; break;
		case(MaterialProp::BETD): this->param_[6] = val; break;
		default: fmt::print("Do not find matched property name.\n");
	}
}

/**
 *  \brief Get property value of material.
 *  \param[in] mp material property enum.
 *  \return value of material property.
 */
template <class Scalar, typename U>
Scalar Material<Scalar, U>::get_material_prop(MaterialProp mp) const {
	Scalar val{0};
	switch (mp) {
		case(MaterialProp::DENS): val = this->param_[0]; break;
		case(MaterialProp::YOUNG):
		case(MaterialProp::EX):
		case(MaterialProp::EY):
		case(MaterialProp::EZ):   val = this->param_[1]; break;
		case(MaterialProp::GXY):
		case(MaterialProp::GYZ):
		case(MaterialProp::GXZ):  val = this->param_[2]; break;
		case(MaterialProp::NU):
		case(MaterialProp::NUXY):
		case(MaterialProp::NUYZ):
		case(MaterialProp::NUXZ):
		case(MaterialProp::PRXY):
		case(MaterialProp::PRYZ):
		case(MaterialProp::PRXZ): val = this->param_[3]; break;
		case(MaterialProp::DMPR): val = this->param_[4]; break;
		case(MaterialProp::ALPD): val = this->param_[5]; break;
		case(MaterialProp::BETD): val = this->param_[6]; break;
		default: fmt::print("Do not find matched property name.\n");
	}
	return val;
}
/**
 *  \brief
 */
template <class Scalar, typename U>
std::vector<Scalar> Material<Scalar, U>::get_material_prop_vec() const {
	std::vector<Scalar> tmp;
	switch (this->mtype_) {
		case MaterialType::SPRING_STIFFNESS:
			for (int i = 0; i < 2; i++) tmp.push_back(this->param_[i]);
			break;
		case MaterialType::MASS_VALUES:
			for (int i = 0; i < 6; i++) tmp.push_back(this->param_[i]);
			break;
		default: fmt::print("Do not find matched property name.\n");
	}
	return std::move(tmp);
}
}  // namespace cafea
