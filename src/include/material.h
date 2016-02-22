#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <array>
#include <string>
#include <initializer_list>

#include "base.h"

namespace cafea {

/**
 *  Material definition.
 */
class Material: public ObjectBase {
	public:
		using ObjectBase::ObjectBase;
		//* Default constructor.
		Material(){};
		// Destructor.
		~Material(){};
		/**
		 *  \brief Initialize with material id type section.
		 *  \param [in] id material's id.
		 *  \param [in] mtype material type.
		 *  \param [in] sect material's section type.
		 */
		Material(int id, 
			int mtype, 
			int sect):id(id), mtype(mtype), sect(sect), name("Material")
		{
			assert(id>0&&mtype>0&&sect>0);
		};
		/**
		 *  \brief Initialize with material id type section and parameters.
		 *  \param [in] id material's id.
		 *  \param [in] mtype material type.
		 *  \param [in] sect material's section type.
		 *  \param [in] val material's 1st parameter array.
		 */
		Material(int id, 
			int mtype, 
			int sect, 
			init_list_<double> val):id(id), mtype(mtype), sect(sect), name("Material")
		{
			assert(id>0&&mtype>0&&sect>0);
			assert(val.size()<=10);
			int i{0};
			for(const auto&it: val)param[i++] = it;
		};
		/**
		 *  \brief Initialize with material id type section and parameters.
		 *  \param [in] id material's id.
		 *  \param [in] mtype material type.
		 *  \param [in] sect material's section type.
		 *  \param [in] val material's 1st parameter array.
		 *  \param [in] val2 material's 2nd parameter array.
		 */
		Material(int id, 
			int mtype, 
			int sect, 
			init_list_<double> val, 
			init_list_<double> val2):id(id), mtype(mtype), sect(sect), name("Material")
		{
			assert(id>0&&mtype>0&&sect>0);
			assert(val.size()<=10&&val2.size()<=10);
			int i{0}, j{0};
			for(const auto&it: val)param[i++] = it;
			for(const auto&it: val2)param2[j++] = it;
		};
		
		//! Get id of material.
		int get_material_id() const {return id;};
		//! Get type of material.
		int get_material_type() const {return mtype;};
		//! Get type of section.
		int get_sect_type() const {return sect;};
		//! Get name of section.
		std::string get_sect_name() const {return sect_name;};
		
		//! Get elastic modulus.
		double get_elastic_modulus() const;
		//! Get density.
		double get_density() const;
		//! Get Poisson ratio.
		double get_poisson_ratio() const;
		//! Get section parameter.
		double get_sect_var(int k) const;
		double get_sect_var(std::string s) const;
		//! Get variable value by name.
		double get_var_by_name(std::string s) const;
		double get_var_by_name(const char* cs) const;
		
		//! Set id of material.
		void set_material_id(int new_id){id = new_id;};
		//! Set type of material.
		void set_material_type(int new_mtype){mtype = new_mtype;};
		//! Set type of section.
		void set_sect_type(int new_sect){sect = new_sect;};
		//! Set name of section.
		void set_sect_name(std::string new_name){sect_name = new_name;};
		//! Set elastic modulus.
		void set_elastic_modulus(double y);
		//! Set density.
		void set_density(double y);
		//! Set Poisson ratio.
		void set_poisson_ratio(double y);
		//! Set section parameter.
		void set_sect_var(int k, double y);
		void set_sect_var(std::string s, double y);
		void set_sect_var(const char* cs, double y);
		//! Set variable value by name.
		void set_var_by_name(std::string s, double y);
		void set_var_by_name(const char* cs, double y);
		
	private:
		int mtype;//!< Material type.
		int sect;//!< Section type.
		std::string sect_name;//!< Section name.
		std::array<double, 10> param;//!< 1st parameter array.
		std::array<double, 10> param2;//!< 2nd parameter array.
		
		template <class T>
		using init_list_ = std::initializer_list<T>;
};
}
#endif