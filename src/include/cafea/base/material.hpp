/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#ifndef CAFEA_MATERIAL_HPP_
#define CAFEA_MATERIAL_HPP_

#include <vector>
#include <algorithm>

#include "cafea/base/base.hpp"
#include "cafea/base/enum_lib.hpp"
// #include "cafea/fortran/fortran_wrapper.hpp"

namespace cafea {
/**
 *  \class Material definition.
 */
template <class T = REAL4>
class Material: public ObjectBase {
	public:
		using ObjectBase::ObjectBase;
		//! Default constructor.
		Material() = delete;
		/**
		 *  \brief Initialize with material id type section.
		 *  \param [in] id material's id.
		 *  \param [in] mtype material type.
		 */
		Material(int id, MaterialType mtype): mtype_(mtype), ObjectBase {id, "Material"} {}
		/**
		 *  \brief Initialize with material id type section and parameters.
		 *  \param [in] id material's id.
		 *  \param [in] mtype material type.
		 *  \param [in] val material's 1st parameter array.
		 */
		Material(int id, MaterialType mtype, init_list_<T> val):
			mtype_(mtype), ObjectBase {id, "Material"} {
			assert(val.size() > 0 && val.size() <= 10);
			std::copy(val.begin(), val.end(), param_.begin());
		}
		/**
		 *  \brief Initialize with material id type section and parameters.
		 *  \param [in] id material's id.
		 *  \param [in] mtype material type.
		 *  \param [in] val material's 1st parameter array.
		 *  \param [in] val2 material's 2nd parameter array.
		 */
		Material(int id, MaterialType mtype, init_list_<T> va, init_list_<T> vb):
			mtype_(mtype), ObjectBase {id, "Material"} {
			assert(va.size() > 0 && va.size() <= 10);
			assert(vb.size() > 0 && vb.size() <= 10);
			std::copy(va.begin(), va.end(), param_.begin());
			std::copy(vb.begin(), vb.end(), param2_.begin());
		}
		// /**
		//  *  \brief Initialize with fortran struct.
		//  */
		// Material(const wrapper_::matl_f03 *p): ObjectBase {p->id_, "Material"} {
		// 	std::copy(std::begin(p->val_), std::begin(p->val_)+10, param_.begin());
		// }
		// /**
		//  *  \brief Initialize with fortran struc cdb_prop.
		//  */
		// Material(const wrapper_::cdb_prop *p): ObjectBase {p->id_, "Material"} {
		// 	std::copy(std::begin(p->arrb_), std::begin(p->arrb_)+10, param_.begin());
		// 	std::copy(std::begin(p->arrb2_), std::begin(p->arrb2_)+10, param2_.begin());
		// }
		//! Destructor.
		~Material() override {}
		//! Get type of material.
		MaterialType get_material_type() const { return mtype_;}
		//! Set type of material.
		void set_material_type(MaterialType mt) { mtype_ = mt;}
		//! Set property of material.
		void set_material_prop(MaterialProp mp, T val);
		//! Get property of material.
		T get_material_prop(MaterialProp mp) const;
		//! Get property vector of material.
		std::vector<T> get_material_prop_vec() const;
		//! Print material info.
		friend std::ostream& operator<<(std::ostream& cout, const Material &a) {
			return cout << fmt::format("{} type:{}\n", a.name_,
				a.mtype_ == MaterialType::LINEAR_ELASTIC ? "linear": "unknown");
		}

	private:
		MaterialType mtype_{MaterialType::UNKNOWN};//!< Material type.
		std::array<T, 10> param_;//!< 1st parameter array.
		std::array<T, 10> param2_;//!< 2nd parameter array.
};
// //! Specialization with float and double types.
template class Material<REAL4>;
template class Material<REAL8>;
}  // namespace cafea
#endif  // CAFEA_MATERIAL_HPP_
