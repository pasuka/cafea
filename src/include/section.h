#ifndef SECTION_H
#define SECTION_H

#include <cassert>

#include <array>
#include <string>
#include <iostream>
#include <algorithm>

#include "fmt/format.h"

#include "base.h"

namespace cafea
{
/**
 *  Enum of section type.
 */
enum struct SectionType {
	PIPE, 
	SHELL,
	SOLID,
	SPRING,
	MASS,
	I_BEAM,//!< I-shaped beam section.
	L_BEAM,//!< L-shaped beam section.
	T_BEAM,//!< T-shaped beam section. 
	Z_BEAM,//!< Z-shaped beam section.
	RECT_BEAM,//!< Rectangle beam section.
	CSOLID_BEAM,//!< Circular solid beam section.
	CHAN_BEAM,//!< Channel beam section.
	HATS_BEAM,//!< Hat-shaped beam section.
	HREC_BEAM,//!< Hollow rectangle or box beam section.
	CTUBE_BEAM,//!< Circular tube section.
	UNKNOWN,
};

/**
 *  Enum of section property.
 */
enum struct SectionProp {
	OD,//!< Pipe outer diameter.
	TKWALL,//!< Wall thickness.
	RADCUR,//!< Radius of curvature.
	DENSFL,//!< Internal fluid density.
};

/**
 *  Section definition.
 */
template <class Scalar=float>
class Section: public ObjectBase {
	public:
		using ObjectBase::ObjectBase;
		//! Default constructor.
		Section()=delete;
		/**
		 *  \brief Initialize with section id type.
		 *  \param [in] id section's id.
		 *  \param [in] sect section's type.
		 */
		Section(int id, SectionType sect):sect_(sect),
			ObjectBase{id, fmt::format("Section#{0}", id)} {};
		/**
		 *  \brief Initialize with section id type and parameters.
		 *  \param [in] id section's id.
		 *  \param [in] sect section's type.
		 *  \param [in] val section's 1st parameter array.
		 */
		Section(int id, SectionType sect, init_list_<Scalar> val):sect_(sect),
			ObjectBase{id, fmt::format("Section#{0}", id)}
		{
			assert(val.size()>0&&val.size()<=10);
			std::copy(val.begin(), val.end(), param_.begin());
		};
		/**
		 *  \brief Initialize with section id type and parameters.
		 *  \param [in] id section's id.
		 *  \param [in] sect section's type.
		 *  \param [in] val section's 1st parameter array.
		 *  \param [in] val2 section's 2nd parameter array.
		 */
		Section(int id, SectionType sect, init_list_<Scalar> val,
			init_list_<Scalar> val2):sect_(sect),
			ObjectBase{id, fmt::format("Material#{0}", id)}
		{
			assert(val.size()>0&&val.size()<=10);
			assert(val2.size()>0&&val2.size()<=10);
			std::copy(val.begin(), val.end(), param_.begin());
			std::copy(val2.begin(), val2.end(), param2_.begin());
		};
		//! Destructor.
		~Section(){};
		//! Get type of section.
		SectionType get_sect_type() const {return sect_;};
		//! Set type of section.
		void set_sect_type(SectionType st) {sect_ = st;};
		//! Get property of section.
		Scalar get_sect_prop(SectionProp sp) const;
		//! Set property of section.
		void set_sect_prop(SectionProp sp, Scalar val);
		//! Print section info.
		friend std::ostream& operator<<(std::ostream& cout, const Section &a)
		{
			cout << a.name_ << "\t";
			switch(a.sect_){
			case SectionType::SHELL:
				cout << "Shell section";
				break;
			case SectionType::SOLID:
				cout << "Solid section";
				break;
			case SectionType::PIPE:
			case SectionType::CTUBE_BEAM:
				cout << "Tube section";
				break;
			default:
				cout << "Other section";
			}
			return cout << "\n";
		};
		
	private:
		SectionType sect_{SectionType::UNKNOWN};//!< Section type.
		std::array<Scalar, 10> param_;//!< 1st parameter array.
		std::array<Scalar, 10> param2_;//!< 2nd parameter array.	
};
}
#endif