#ifndef MATERIAL_H
#define MATERIAL_H

#include "base.h"
#include "enum_lib.h"

namespace cafea
{
/**
 *  \class Material definition.
 */
template <class T=REAL4>
class Material: public ObjectBase {
	public:
		using ObjectBase::ObjectBase;
		//! Default constructor.
		Material()=delete;
		/**
		 *  \brief Initialize with material id type section.
		 *  \param [in] id material's id.
		 *  \param [in] mtype material type.
		 */
		Material(int id, MaterialType mtype):mtype_(mtype),
			ObjectBase{ id, fmt::format("Material#{0}", id)} {};
		/**
		 *  \brief Initialize with material id type section and parameters.
		 *  \param [in] id material's id.
		 *  \param [in] mtype material type.
		 *  \param [in] val material's 1st parameter array.
		 */
		Material(int id, MaterialType mtype, init_list_<T> val):mtype_(mtype),
			ObjectBase{ id, fmt::format("Material#{0}", id)}
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
		Material(int id, MaterialType mtype, init_list_<T> val, init_list_<T> val2):mtype_(mtype),
			ObjectBase{ id, fmt::format("Material#{0}", id)}
		{
			assert(val.size()>0&&val.size()<=10);
			assert(val2.size()>0&&val2.size()<=10);
			std::copy(val.begin(), val.end(), param_.begin());
			std::copy(val2.begin(), val2.end(), param2_.begin());
		};
		//! Destructor.
		~Material() override {};
		//! Get type of material.
		MaterialType get_material_type() const { return mtype_;};
		//! Set type of material.
		void set_material_type(MaterialType mt) { mtype_ = mt;};

		//! Set property of material.
		void set_material_prop(MaterialProp mp, T val);
		//! Get property of material.
		T get_material_prop(MaterialProp mp) const;
		//! Get property vector of material.
		std::vector<T> get_material_prop_vec() const;
		//! Print material info.
		friend std::ostream& operator<<(std::ostream& cout, const Material &a)
		{
			return cout << fmt::format("{} type:{}\n", a.name_,
				a.mtype_==MaterialType::LINEAR_ELASTIC ? "linear": "unknown");
		};
	private:
		MaterialType mtype_{MaterialType::UNKNOWN};//!< Material type.
		std::array<T, 10> param_;//!< 1st parameter array.
		std::array<T, 10> param2_;//!< 2nd parameter array.
};
//! Specialization with float and double types.
template class Material<REAL4>;
template class Material<REAL8>;
}
#endif
