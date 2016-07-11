#ifndef MATERIAL_H
#define MATERIAL_H

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
 *  Enum of material type.
 */
enum struct MaterialType {
	LINEAR_ELASTIC,
	UNKNOWN,
};

/**
 *  Enum of material property.
 */
enum struct MaterialProp {
	DENS,//!< Mass density.
	YOUNG, EX, EY, EZ,//!< Elastic modulus.
	GXY, GYZ, GXZ,//!< Shear elastic modulus.
	NU, NUXY, NUYZ, NUXZ,//!< Minor Poisson's ratios. 
	PRXY, PRYZ, PRXZ,//!< Major Poisson's ratios.
	DMPR,//!< Constant structural damping coefficient in full harmonic analysis or damping ratio in mode-superposition analysis.
	ALPD,//!< Mass matrix multiplier for damping.
	BETD,//!< Stiffness matrix multiplier for damping.
};

/**
 *  Material definition.
 */
template <class Scalar=float>
class Material: public ObjectBase {
	public:
		using ObjectBase::ObjectBase;
		/**
		 *  \brief Initialize with material id type section.
		 *  \param [in] id material's id.
		 *  \param [in] mtype material type.
		 */
		Material(int id, MaterialType mtype):mtype_(mtype),
			ObjectBase{id, fmt::format("Material#{0}", id)}{};
		/**
		 *  \brief Initialize with material id type section and parameters.
		 *  \param [in] id material's id.
		 *  \param [in] mtype material type.
		 *  \param [in] val material's 1st parameter array.
		 */
		Material(int id, MaterialType mtype, init_list_<Scalar> val):mtype_(mtype),
			ObjectBase{id, fmt::format("Material#{0}", id)}
		{
			assert(val.size()>0&&val.size()<=10);
			std::copy(val.begin(), val.end(), param_.begin());
		};
		/**
		 *  \brief Initialize with material id type section and parameters.
		 *  \param [in] id material's id.
		 *  \param [in] mtype material type.
		 *  \param [in] val material's 1st parameter array.
		 *  \param [in] val2 material's 2nd parameter array.
		 */
		Material(int id, MaterialType mtype, init_list_<Scalar> val,
			init_list_<Scalar> val2):mtype_(mtype),
			ObjectBase{id, fmt::format("Material#{0}", id)}
		{
			assert(val.size()>0&&val.size()<=10);
			assert(val2.size()>0&&val2.size()<=10);
			std::copy(val.begin(), val.end(), param_.begin());
			std::copy(val2.begin(), val2.end(), param2_.begin());
		};
		//! Destructor.
		~Material(){};
		//! Get type of material.
		MaterialType get_material_type() const {return mtype_;};
		//! Set type of material.
		void set_material_type(MaterialType mt) {mtype_ = mt;};
		
		//! Set property of material.
		void set_material_prop(MaterialProp mp, Scalar val);
		//! Get property of material.
		Scalar get_material_prop(MaterialProp mp) const;
		
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
			return cout << "\n";
		};
		
	private:
		MaterialType mtype_{MaterialType::UNKNOWN};//!< Material type.
		std::array<Scalar, 10> param_;//!< 1st parameter array.
		std::array<Scalar, 10> param2_;//!< 2nd parameter array.	
};
}
#endif