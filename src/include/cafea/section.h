/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#ifndef CAFEA_SECTION_H_
#define CAFEA_SECTION_H_

#include <algorithm>

#include "cafea/base.h"
#include "cafea/enum_lib.h"
#include "cafea/fortran_wrapper.h"

namespace cafea {
/**
 *  \class Section definition.
 */
template <class Scalar = REAL4>
class Section: public ObjectBase {
	public:
		using ObjectBase::ObjectBase;
		//! Default constructor.
		Section() = delete;
		/**
		 *  \brief Initialize with section id type.
		 *  \param [in] id section's id.
		 *  \param [in] sect section's type.
		 */
		Section(int id, SectionType sect): sect_(sect),
			ObjectBase {id, fmt::format("Section#{0}", id)} {}
		/**
		 *  \brief Initialize with section id type and parameters.
		 *  \param [in] id section's id.
		 *  \param [in] sect section's type.
		 *  \param [in] val section's 1st parameter array.
		 */
		Section(int id, SectionType sect, init_list_<Scalar> val): sect_(sect),
			ObjectBase {id, fmt::format("Section#{0}", id)} {
			assert(val.size() > 0 && val.size() <= 10);
			std::copy(val.begin(), val.end(), param_.begin());
		}
		/**
		 *  \brief Initialize with section id type and parameters.
		 *  \param [in] id section's id.
		 *  \param [in] sect section's type.
		 *  \param [in] va section's 1st parameter array.
		 *  \param [in] vb section's 2nd parameter array.
		 */
		Section(int id, SectionType sect, init_list_<Scalar> va, init_list_<Scalar> vb): sect_(sect),
			ObjectBase {id, fmt::format("Section#{}", id)} {
			assert(va.size() > 0 && va.size() <= 10);
			assert(vb.size() > 0 && vb.size() <= 10);
			std::copy(va.begin(), va.end(), param_.begin());
			std::copy(vb.begin(), vb.end(), param2_.begin());
		}
		/**
		 *  \brief Initialize with section id and values.
		 *  \param [in] id section's id.
		 *  \param [in] val array of value.
		 *  \param [in] n length of array.
		 */
		Section(int id, const Scalar val[], int n): ObjectBase {id, fmt::format("Section#{}", id)} {
			assert(0 < n && n <= 10);
			param_.fill(Scalar(0));
			for (int i = 0; i < n; i++) param_[i] = val[i];
		}
		/**
		 *  \brief Initialize with fortran struct.
		 */
		Section(const wrapper_::sect_f03 *p): ObjectBase {p->id_, fmt::format("Section#{}", p->id_)} {
			std::copy(std::begin(p->val_), std::begin(p->val_)+10, param_.begin());
		}
		Section(const wrapper_::cdb_prop *p): ObjectBase {p->id, fmt::format("Section#{}", p->id)} {
			std::copy(std::begin(p->arrb), std::begin(p->arrb)+10, param_.begin());
			std::copy(std::begin(p->arrb2), std::begin(p->arrb2)+10, param2_.begin());
		}
		//! Destructor.
		~Section() override {}
		//! Get type of section.
		SectionType get_sect_type() const { return sect_;}
		//! Set type of section.
		void set_sect_type(SectionType st) { sect_ = st;}
		//! Get property of section.
		Scalar get_sect_prop(SectionProp sp) const;
		//! Set property of section.
		void set_sect_prop(SectionProp sp, Scalar val);
		//! Print section info.
		friend std::ostream& operator<<(std::ostream& cout, const Section &a) {
			return cout << fmt::format("{} type:{}\n", a.name_,
				static_cast<int>(a.sect_));
		}

	private:
		SectionType sect_{SectionType::UNKNOWN};//!< Section type.
		std::array<Scalar, 10> param_;//!< 1st parameter array.
		std::array<Scalar, 10> param2_;//!< 2nd parameter array.
};
//! Specialization with float and double types.
template class Section<REAL4>;
template class Section<REAL8>;
}  // namespace cafea
#endif  // CAFEA_SECTION_H_
