#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <array>
#include <string>

#include "base.h"

namespace cafea
{
/**
 *  Enum of material type.
 */
enum struct MaterialType {
	LINEAR_ELASTIC,
	UNKNOWN,
};

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
 *  Material definition.
 */
template <class Scalar=float>
class Material: public ObjectBase {
	public:
		using ObjectBase::ObjectBase;
		//* Default constructor.
		Material(){};
		//! Destructor.
		~Material(){};
		/**
		 *  \brief Initialize with material id type section.
		 *  \param [in] id material's id.
		 *  \param [in] mtype material type.
		 *  \param [in] sect material's section type.
		 */
		Material(int id, MaterialType mtype, SectionType sect):mtype_(mtype),
			sect_(sect), ObjectBase{id, fmt::format("Material#{0}", id)}
		{
			assert(id_>0);
		};
		/**
		 *  \brief Initialize with material id type section and parameters.
		 *  \param [in] id material's id.
		 *  \param [in] mtype material type.
		 *  \param [in] sect material's section type.
		 *  \param [in] val material's 1st parameter array.
		 */
		Material(int id, MaterialType mtype, SectionType sect,
			init_list_<Scalar> val):mtype_(mtype), sect_(sect),
			ObjectBase{id, fmt::format("Material#{0}", id)}
		{
			assert(id>0);
			assert(val.size()>0&&val.size()<=10);
			int i{0};
			for(const auto &it: val)param_[i++] = it;
		};
		/**
		 *  \brief Initialize with material id type section and parameters.
		 *  \param [in] id material's id.
		 *  \param [in] mtype material type.
		 *  \param [in] sect material's section type.
		 *  \param [in] val material's 1st parameter array.
		 *  \param [in] val2 material's 2nd parameter array.
		 */
		Material(int id, MaterialType mtype, SectionType sect, 
			init_list_<Scalar> val, init_list_<Scalar> val2):mtype_(mtype),
			sect_(sect), ObjectBase{id, fmt::format("Material#{0}", id)}
		{
			assert(id>0);
			assert(val.size()>0&&val.size()<=10);
			assert(val2.size()>0&&val2.size()<=10);
			int i{0}, j{0};
			for(const auto& it: val)param_[i++] = it;
			for(const auto& it: val2)param2_[j++] = it;
		};
		
		//! Get type of material.
		MaterialType get_material_type() const {return mtype_;};
		//! Set type of material.
		void set_material_type(MaterialType mt) {mtype_ = mt;};
		//! Get type of section.
		SectionType get_sect_type() const {return sect_;};
		//! Set type of section.
		void set_sect_type(SectionType st) {sect_ = st;};
		//! Print material info.
		friend std::ostream& operator<<(std::ostream& cout, const Material &a)
		{
			cout << a.name_ << "\t";
			switch(a.mtype_){
			case MaterialType::LINEAR_ELASTIC:
				cout << "Linear elastic material";
				break;
			default:
				cout << "Unkown material";
			}
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
		MaterialType mtype_{MaterialType::UNKNOWN};//!< Material type.
		SectionType sect_{SectionType::UNKNOWN};//!< Section type.
		std::array<Scalar, 10> param_;//!< 1st parameter array.
		std::array<Scalar, 10> param2_;//!< 2nd parameter array.	
};
}
#endif