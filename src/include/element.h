#ifndef ELEMENT_H
#define ELEMENT_H

#include <cassert>
#include <cstddef>
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

namespace cafea
{
/**
 * Element object definition. 
 */
template <class T=double, class U=double>
class Element: public ObjectBase {
	public:
		using ObjectBase::ObjectBase;//!< Inherit Base's constructors.
		//* Default constructor.
		Element(){};
		//* Destructor.
		~Element(){
			nodes_.clear();
			mass_.resize(0, 0);
			stif_.resize(0, 0);
			tran_.resize(0, 0);
			rhs_.resize(0, 0);
			stress_.resize(0, 0);
			stress_cmplx_.resize(0, 0);
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
		void form_matrix(const Node<REAL4, U> [], const Material<REAL4>*, const Section<REAL4>*);
		void form_matrix(const Node<REAL8, U> [], const Material<REAL8>*, const Section<REAL8>*);
		//! Get stiffness matrix.
		matrix_<T> get_stif() const {return stif_;};
		//! Get mass matrix.
		matrix_<T> get_mass() const {return mass_;};
		//! Get transpose matrix.
		matrix_<T> get_tran() const {return tran_;};
		//! Get right-hand side matrix.
		matrix_<T> get_rhs() const {return rhs_;};
		//! Get raw pointer of stiffness matrix.
		const T *get_stif_ptr() const {return stif_.data();};
		//! Get raw pointer of mass matrix.
		const T *get_mass_ptr() const {return mass_.data();};
		//! Get raw pointer of transpose matrix.
		const T *get_tran_ptr() const {return tran_.data();};
		//! Get raw pointer of right-hand side matrix.
		const T *get_rhs_ptr() const {return rhs_.data();};
		
		/* //! Get stress matrix.
		matrix_<U> get_stress() const {return stress_;};
		//! Get raw pointer of stress matrix.
		U *get_stress_ptr() const {return stress_.data();}; */
		
		//! Post process.
		void post_stress();
		
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
		//! Get id of element.
		int get_element_id() const {return id_;};
		//! Get order of element.
		int get_element_order() const;
		//! Get dofs of each node.
		int get_dofs_per_node() const;
		//! Get total number of nodes.
		int get_total_num_of_node() const {return nodes_.size();};
		//! Get active number of nodes.
		int get_active_num_of_node() const;
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
		
		matrix_<T> stif_;//!< Stiffness matrix of element.
		matrix_<T> mass_;//!< Mass matrix of element.
		matrix_<T> tran_;//!< Transpose matrix of element.
		matrix_<T> rhs_;//!< Right-hand side of element.
		
		matrix_<T> stress_;//!< Stress of element.
		matrix_<std::complex<T>> stress_cmplx_;//!< Stress of element in complex.
		
};

//! Specialization.
// template class Element<REAL4, REAL4>;
// template class Element<REAL4, REAL8>;
template class Element<REAL8, REAL8>;
// template class Element<REAL8, REAL4>;
namespace element_attr_
{
size_t get_dofs_per_node(ElementType);
size_t get_active_num_of_node(ElementType);
size_t get_element_order(ElementType);
}
}
#endif