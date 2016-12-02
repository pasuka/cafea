#ifndef ELEMENT_H
#define ELEMENT_H

#include <cassert>
#include <cstddef>

#include <map>
#include <array>
#include <tuple>
#include <vector>
#include <ostream>
#include <algorithm>
#include <initializer_list>

#include <Eigen/Dense>

#include "base.h"
#include "node.h"
#include "material.h"
#include "section.h"
#include "element_lib.h"
#include "load.h"

namespace cafea
{
/**
 * Element object definition. 
 */
template <class T=double>
class Element: public ObjectBase {
	public:
		using ObjectBase::ObjectBase;//!< Inherit Base's constructors.
		//* Default constructor.
		Element(){};
		//* Destructor.
		~Element(){
			nodes_.clear();
			global_dofs_.clear();
			attr_.clear();
			mass_.resize(0, 0);
			stif_.resize(0, 0);
			tran_.resize(0, 0);
			rhs_.resize(0);
			result_.resize(0, 0);
			result_cmplx_.resize(0, 0);
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
		/**
		 *  \brief Initialize with element id material.
		 *  \param [in] id an positive integer.
		 *  \param [in] mp number of material type.
		 *  \param [in] st number of section type.
		 */
		Element(int id, int mp, int st):ObjectBase{id, fmt::format("Elem#{0}",
			id)}, matl_(mp), sect_(st) {assert(sect_>0&&matl_>0);};
			
		//! Generate stifness mass matrix of element.
		template <class U=REAL4>
		void form_matrix(const Node<U, T>[], const Material<U>*, const Section<U>*);
		
		template <class U=REAL4>
		void form_matrix(const vector<Node<U, T>>, const Material<U>*, const Section<U>*);
		
		template <class U=REAL4>
		void form_matrix(const Node<U, T>[], const Material<U>*, const Section<U>*, const std::vector<LoadCell<U>>);
		
		template <class U=REAL4>
		void form_matrix(const vector<Node<U, T>>, const Material<U>*, const Section<U>*, const std::vector<LoadCell<U>>);
		
		//! Get stiffness matrix.
		matrix_<T> get_stif() const {return stif_;};
		//! Get mass matrix.
		matrix_<T> get_mass() const {return mass_;};
		//! Get transpose matrix.
		matrix_<T> get_tran() const {return tran_;};
		//! Get right-hand side matrix.
		vecX_<T> get_rhs() const {return rhs_;};
		//! Get result matrix.
		matrix_<T> get_result() const {return result_;};
		
		//! Get raw pointer of stiffness matrix.
		const T *get_stif_ptr() const {return stif_.data();};
		//! Get raw pointer of mass matrix.
		const T *get_mass_ptr() const {return mass_.data();};
		//! Get raw pointer of transpose matrix.
		const T *get_tran_ptr() const {return tran_.data();};
		//! Get raw pointer of right-hand side matrix.
		const T *get_rhs_ptr() const {return rhs_.data();};
		//! Get raw pointer of result matrix.
		const T *get_result_ptr() const {return result_.data();};
		
		//! Post process.
		void post_stress(const vecX_<T>);
		void post_stress(const vecX_<std::complex<T>>);
		// void post_stress(const matrix_<T>);
		// void post_stress(const matrix_<std::complex<T>>);
		
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
		//! Set element option. 
		void set_option(init_list_<int> a)
		{
			assert(0<a.size()&&a.size()<=10);
			std::copy(a.begin(), a.end(), keyopt_.begin());
		};
		//! Set element option.
		void set_option(const int a[], int m)
		{
			assert(0<m&&m<=10);
			for(int i=0; i<m; i++)keyopt_[i] = a[i];
		};
		//! Set id of element.
		void set_element_id(int x){id_ = x;};
		//! Set type of element.
		void set_element_type(int x);
		//! Set material id.
		void set_material_id(int x){matl_ = x;};
		//! Set section id.
		void set_section_id(int x){sect_ = x;};
		//! set type of element.
		void set_element_type(ElementType et){etype_ = et;};
		//! Set dof index of element.
		void set_element_dofs(int x) {global_dofs_.push_back(x);};
		//! Set mass matrix format.
		void set_lumped_mass(bool val=false){if(val)keyopt_[0]=1;};
		
		//! Get material id.
		int get_material_id() const {return matl_;};
		//! Get section id.
		int get_section_id() const {return sect_;};
		//! Get node list.
		std::vector<int> get_node_list() const {return nodes_;};
		//! Get option.
		std::array<int, 10> get_option() const {return keyopt_;};
		//! Get type of element.
		ElementType get_element_type() const {return etype_;};
		//! Get id element type.
		size_t get_element_type_id() const;
		//! Get id of element.
		int get_element_id() const {return id_;};
		//! Get order of element.
		size_t get_element_order() const;
		//! Get dofs of each node.
		size_t get_dofs_per_node() const;
		//! Get total number of nodes.
		size_t get_total_num_of_node() const {return nodes_.size();};
		//! Get active number of nodes.
		size_t get_active_num_of_node() const;
		//! Get shape of element matrix.
		std::array<size_t, 2> get_matrix_shape() const;
		//! Get shape of rhs matrix.
		std::array<size_t, 2> get_rhs_shape() const;
		//! Get shape of result matrix.
		std::array<size_t, 2> get_result_shape() const;
		//! Get global dofs array.
		std::vector<int> get_element_dofs() const {return global_dofs_;};
		//! Get mass format.
		bool get_lumped_mass()const{return 0<keyopt_[0];};
		//! Print information.
		friend std::ostream& operator<<(std::ostream& cout, const Element &a)
		{
			cout << fmt::format("Element id:{}\t", a.id_);
			cout << fmt::format("Num node:{}\t", a.get_total_num_of_node());
			cout << fmt::format("Active node:{}\t", a.get_active_num_of_node());
			cout << fmt::format("Order:{}\t", a.get_element_order());
			cout << fmt::format("Dof per node:{}\t", a.get_dofs_per_node());
			switch(a.etype_){
			case ElementType::PIPE16: cout << "2-node straight pipe"; break;
			case ElementType::PIPE18: cout << "2-node curved pipe"; break;
			case ElementType::BEAM188:
			case ElementType::B31: cout << "2-node straight beam"; break;
			case ElementType::BEAM189:
			case ElementType::B32: cout << "3-node straight beam"; break;
			case ElementType::C3D4: cout << "4-node tet"; break;
			case ElementType::C3D8:
			case ElementType::SOLID185: cout << "8-node hex"; break;
			case ElementType::C3D20:
			case ElementType::SOLID186: cout << "20-node hex"; break;
			case ElementType::S3R: cout << "3-node tri"; break;
			case ElementType::S4R:
			case ElementType::SHELL181: cout << "4-node quad"; break;
			case ElementType::S8R:
			case ElementType::SHELL281: cout << "8-node quad"; break;
			case ElementType::S9R: cout << "9-node quad"; break;
			case ElementType::MASS21: cout << "1-node mass"; break;
			case ElementType::COMBIN14: cout << "2-node spring"; break;
			case ElementType::UNKNOWN:
			default: cout << fmt::format("Unknown element type");
			}
			return cout << "\n";
		}
 	private:
		ElementType etype_{ElementType::UNKNOWN};//!< Type of element.
		int matl_{-1};//!< Material id.
		int sect_{-1};//!< Section id.
		std::array<int, 10> keyopt_{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};//!< Parameters of element.
		std::vector<int> nodes_;//!< Array of node list.
		std::vector<int> global_dofs_;//!< Array of global dofs.
		std::map<std::string, T> attr_;//!< Attibute of element parameters.
		
		matrix_<T> stif_;//!< Stiffness matrix of element.
		matrix_<T> mass_;//!< Mass matrix of element.
		matrix_<T> tran_;//!< Transpose matrix of element.
		vecX_<T> rhs_;//!< Right-hand side of element.
		matrix_<std::complex<T>> rhs_cmplx_;//!< Right-hand matrix of element in complex.
		
		matrix_<T> result_;//!< Result of element.
		matrix_<std::complex<T>> result_cmplx_;//!< Result of element in complex.
		
};

#include "element_ext.hpp"
#include "element_cmplx_ext.hpp"

//! Specialization.
template class Element<REAL8>;
template class Element<REAL4>;

/**
 *  \brief Attribution of element.
 */
struct ElementAttr
{
	//! Get dofs per node.
	static size_t get_dofs_per_node(ElementType);
	//! Get activated number of nodes.
	static size_t get_active_num_of_node(ElementType);
	//! Get shape function order.
	static size_t get_element_order(ElementType);
	//! Get element type in Ansys rules.
	static size_t get_element_type_id(ElementType);
};
}
#endif