#ifndef ELEMENT_H
#define ELEMENT_H

#include <cassert>
#include <array>
#include <tuple>
#include <vector>
#include <initializer_list>

#include <Eigen/Dense>

#include "base.h"

namespace cafea
{
/**
 *  Enum of element types.
 */
enum struct ElementType {
	PIPE16, PIPE18,
	BEAM188, BEAM189, B31, B32,
	SOLID185, SOLID186, C3D4, C3D8, C3D20,
	SHELL181, SHELL281, S3R, S4R, S8R, S9R,
	MASS21, COMBIN14,
	UNKNOWN,
};

/**
 * Element object definition. 
 */
template <class T=double, class U=double>
class Element: public ObjectBase {
	public:
		using ObjectBase::ObjectBase;//!< Inherit Base's constructors.
		//* Default constructor.
		Element()=delete;
		//* Destructor.
		~Element(){
			nodes_.clear();
			mass_.resize(0, 0);
			stif_.resize(0, 0);
			tran_.resize(0, 0);
			rhs_.resize(0, 0);
		};
		/**
		 *  \brief Initialize with element id type material type and node list.
		 *  \param [in] id an integer must bigger than zero.
		 *  \param [in] et number of element type.
		 *  \param [in] mp number of material type.
		 *  \param [in] st number of section type.
		 *  \param [in] nodes list of nodes id.
		 */
		Element(int id, ElementType et, int mp, int st, init_list_<int> nodes):
			ObjectBase{id, fmt::format("Elem#{0}", id)}, etype_(et), matl_(mp), sect_(st)
		{
			assert(sect_>0&&matl_>0);
			for(const auto& it: nodes){
				assert(it>0);
				nodes_.push_back(it);
			}
		}
		//! Generate stifness mass matrix of element.
		void form_matrix();
		//! Get stiffness matrix.
		matrix_<T> get_stif() const {return stif_;};
		//! Get mass matrix.
		matrix_<T> get_mass() const {return mass_;};
		//! Get transpose matrix.
		matrix_<T> get_tran() const {return tran_;};
		//! Get right-hand side matrix.
		matrix_<T> get_rhs() const {return rhs_;};
		//! Get raw pointer of stiffness matrix.
		T *get_stif_ptr() const {return stif_.data();};
		//! Get raw pointer of mass matrix.
		T *get_mass_ptr() const {return mass_.data();};
		//! Get raw pointer of transpose matrix.
		T *get_tran_ptr() const {return tran_.data();};
		//! Get raw pointer of right-hand side matrix.
		T *get_rhs_ptr() const {return rhs_.data();};
		
		/* //! Get stress matrix.
		matrix_<U> get_stress() const {return stress_;};
		//! Get raw pointer of stress matrix.
		U *get_stress_ptr() const {return stress_.data();}; */
		
		//! Post process.
		void post_stress();
		
		//! Get node list.
		std::vector<int> get_node_list() const {return nodes_;};
		//! Set node list.
		void set_node_list(const int a[], int m)
		{
			if(!nodes_.empty())nodes_.clear();
			for(int i=0; i<m; i++)nodes_.push_back(a[i]);
		};
		//! Set node list with c++11 initializer_list.
		void set_node_list(init_list_<int> a)
		{
			if(!nodes_.empty())nodes_.clear();
			for(const auto& it: a)nodes_.push_back(it);
		};
		//! Set id of element.
		void set_element_id(int x){id_ = x;};
		//! Set type of element.
		void set_element_type(int x){etype_ = x;};
		//! Set material id.
		void set_material_id(int x){matl_ = x;};
		//! Set section id.
		void set_section_id(int x){sect_ = x;};
		//! set type of element.
		void set_element_type(ElementType et){etype_ = et;};
		
		//! Get material id.
		int get_material_id() const {return matl_;};
		//! Get section id.
		int get_section_id() const {return sect_;};
		//! Get type of element.
		ElementType get_element_type() const {return etype_;};
		//! Get id of element.
		int get_element_id() const {return id_;};
		//! Get order of element.
		int get_element_order() const;
		//! Get dofs of each node.
		int get_dofs_per_node() const;
		//! Get total number of nodes.
		int get_total_num_of_nodes() const {return nodes_.size();};
		//! Get active number of nodes.
		int get_active_num_of_nodes() const;
		
 	private:
		ElementType etype_{ElementType::UNKNOWN};//!< Type of element.
		int matl_{-1};//!< Material id.
		int sect_{-1};//!< Section id.
		std::array<int, 8> keyopt_{-1, -1, -1, -1, -1, -1, -1, -1};//!< Parameters of element.
		std::vector<int> nodes_;//!< Array of node list.
		
		matrix_<T> stif_;//!< Stiffness matrix of element.
		matrix_<T> mass_;//!< Mass matrix of element.
		matrix_<T> tran_;//!< Transpose matrix of element.
		matrix_<T> rhs_;//!< Right-hand side of element.
		
};
}
#endif