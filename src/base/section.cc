/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#include "cafea/base/section.hpp"

namespace cafea {
/**
 *  \brief Set property value of section.
 *  \param[in] sp property enum.
 *  \param val property value.
 */
template <class Scalar, typename U>
void Section<Scalar, U>::set_sect_prop(SectionProp sp, Scalar val) {
	switch (sp) {
		case(SectionProp::OD):
		case(SectionProp::ADDONMASS):
		case(SectionProp::ADDONSPRING): this->param_[0] = val; break;
		case(SectionProp::TKWALL): 		this->param_[1] = val; break;
		case(SectionProp::RADCUR): 		this->param_[2] = val; break;
		case(SectionProp::DENSFL): 		this->param_[3] = val; break;
		case(SectionProp::PRESIN): 		this->param_[4] = val; break;
		default: fmt::print("Do not find matched property.\n");
	}
}

/**
 *  \brief Get property value of section.
 *  \param[in] sp property enum.
 *  \return property value.
 */
template <class Scalar, typename U>
Scalar Section<Scalar, U>::get_sect_prop(SectionProp sp) const {
	Scalar val{0};
	switch (sp) {
		case(SectionProp::OD):
		case(SectionProp::ADDONMASS):
		case(SectionProp::ADDONSPRING): val = this->param_[0]; break;
		case(SectionProp::TKWALL): 		val = this->param_[1]; break;
		case(SectionProp::RADCUR): 		val = this->param_[2]; break;
		case(SectionProp::DENSFL): 		val = this->param_[3]; break;
		case(SectionProp::PRESIN): 		val = this->param_[4]; break;
		default: fmt::print("Do not find matched property.\n");
	}
	return val;
}
}  // namespace cafea
