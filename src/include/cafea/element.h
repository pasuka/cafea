/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#ifndef CAFEA_ELEMENT_H_
#define CAFEA_ELEMENT_H_

#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include "cafea/node.h"
#include "cafea/load.h"
#include "cafea/section.h"
#include "cafea/material.h"
#include "cafea/element_lib.h"
#include "cafea/fortran_wrapper.h"

namespace cafea {
/**
 *  Element object definition.
 */
template <class T = REAL8>
class Element: public ObjectBase {
	public:
		using ObjectBase::ObjectBase;//!< Inherit Base's constructors.
		//* Default constructor.
		Element() = delete;
		//* Destructor.
		~Element() override {
			nodes_.clear();
			global_dofs_.clear();
			attr_.clear();
			mass_.resize(0, 0);
			stif_.resize(0, 0);
			tran_.resize(0, 0);
			rhs_.resize(0);

			load_cmplx_.resize(0, 0);
			rhs_cmplx_.resize(0, 0);
			result_.resize(0, 0);
			result_cmplx_.resize(0, 0);
		}
		/**
		 *  \brief Initialize with element id type material type and node list.
		 *  \param [in] id an integer must bigger than zero.
		 *  \param [in] et number of element type.
		 *  \param [in] mp number of material type.
		 *  \param [in] st number of section type.
		 *  \param [in] nodes list of nodes id.
		 */
		Element(int id, ElementType et, int mp, int st, init_list_<int> nodes):
			etype_(et), ObjectBase {id, "Elem"}, matl_(mp), sect_(st) {
			assert(sect_ > 0 && matl_ > 0);
			assert(nodes.size() > 0);
			// std::copy(nodes.begin(), nodes.end(), [nodes_](int x){if(x)nodes_.push_back(x);});
			for (const auto &it: nodes) {
				assert(it > 0);
				nodes_.push_back(it);
			}
		}
		/**
		 *  \brief Initialize with element id material.
		 *  \param [in] id an positive integer.
		 *  \param [in] mp number of material type.
		 *  \param [in] st number of section type.
		 */
		Element(int id, int mp, int st): ObjectBase {id, "Elem"}, matl_(mp),
			sect_(st) { assert(sect_ > 0 && matl_ > 0);}
		/**
		  *  \brief Initialize with fortran struct.
		  */
		Element(const wrapper_::elem_f03 *p_elem): matl_(p_elem->prop_[0]),
			sect_(p_elem->prop_[1]), ObjectBase {p_elem->id_, "Elem"} {
			assert(0 < sect_);
			assert(0 < matl_);
			set_element_type(p_elem->etype_);
			set_option(p_elem->opt_, 10);
			for (const auto &it: p_elem->node_list_) {
				if (0 > it) break;
				nodes_.push_back(it);
			}
		}
		//! Generate stifness mass matrix of element.
		template <class U = REAL4>
		void form_matrix(const Node<U, T>[], const Material<U>*, const Section<U>*);

		template <class U = REAL4>
		void form_matrix(const std::vector<Node<U, T>>, const Material<U>*, const Section<U>*);

		template <class U = REAL4>
		void form_matrix(const Node<U, T>[], const Material<U>*, const Section<U>*, const std::vector<LoadCell<U>>);

		template <class U = REAL4>
		void form_matrix(const std::vector<Node<U, T>>, const Material<U>*, const Section<U>*, const std::vector<LoadCell<U>>);

		//! Get stiffness matrix.
		matrix_<T> get_stif() const { return stif_;}
		//! Get mass matrix.
		matrix_<T> get_mass() const { return mass_;}
		//! Get transpose matrix.
		matrix_<T> get_tran() const { return tran_;}
		//! Get right-hand side matrix.
		vecX_<T> get_rhs() const { return rhs_;}
		//!
		template <class ResType = T>
		matrix_<ResType> get_rhs() const;
		//! Get result matrix.
		matrix_<T> get_result() const { return result_;}
		//!
		template <class ResType = T>
		matrix_<ResType> get_result() const;
		//!
		cmatrix_<T> get_rhs_cmplx() const { return rhs_cmplx_;}

		//! Get raw pointer of stiffness matrix.
		const T* get_stif_ptr() const { return stif_.data();}
		//! Get raw pointer of mass matrix.
		const T* get_mass_ptr() const { return mass_.data();}
		//! Get raw pointer of transpose matrix.
		const T* get_tran_ptr() const { return tran_.data();}
		//! Get raw pointer of right-hand side matrix.
		const T* get_rhs_ptr() const { return rhs_.data();}
		//! Get raw pointer of result matrix.
		const T* get_result_ptr() const { return result_.data();}

		//! Post process.
		void post_stress(const vecX_<T>);

		template <class ResType = T>
		void post_stress(const matrix_<ResType>);
		// void post_stress(const matrix_<T>);
		// void post_stress(const matrix_<std::complex<T>>);

		//! Set node list.
		void set_node_list(const int a[], int m) {
			if (!nodes_.empty()) nodes_.clear();
			for ( int i = 0; i < m; i++) nodes_.push_back(a[i]);
		}
		//! Set node list with c++11 initializer_list.
		void set_node_list(init_list_<int> a) {
			if (!nodes_.empty()) nodes_.clear();
			std::copy(a.begin(), a.end(), std::back_inserter(nodes_));
			// for(const auto& it: a)nodes_.push_back(it);
		}
		//! Set element option.
		void set_option(init_list_<int> a) {
			assert(0 < a.size() && a.size() <= 10);
			std::copy(a.begin(), a.end(), keyopt_.begin());
		}
		//! Set element option.
		void set_option(const int a[], int m) {
			assert(0 < m && m <= 10);
			for (int i = 0; i < m; i++) keyopt_[i] = a[i];
		}
		//! Set id of element.
		void set_element_id(int x) { id_ = x;}
		//! Set type of element.
		void set_element_type(int x);
		//! Set material id.
		void set_material_id(int x) { matl_ = x;}
		//! Set section id.
		void set_section_id(int x) { sect_ = x;}
		//! set type of element.
		void set_element_type(ElementType et) { etype_ = et;}
		//! Set dof index of element.
		void set_element_dofs(int x) { global_dofs_.push_back(x);}
		//! Clear dof index of element.
		void clear_element_dofs() {
			if (!global_dofs_.empty()) global_dofs_.clear();
		}
		//! Set mass matrix format.
		void set_lumped_mass(bool val = false) { if (val) { keyopt_[0] = 1;}}

		//! Get material id.
		int get_material_id() const { return matl_;}
		//! Get section id.
		int get_section_id() const { return sect_;}
		//! Get node list.
		std::vector<int> get_node_list() const { return nodes_;}
		//! Get option.
		std::array<int, 10> get_option() const { return keyopt_;}
		//! Get type of element.
		ElementType get_element_type() const { return etype_;}
		//! Get id element type.
		size_t get_element_type_id() const;
		//! Get id of element.
		int get_element_id() const { return id_;}
		//! Get order of element.
		size_t get_element_order() const;
		//! Get dofs of each node.
		size_t get_dofs_per_node() const;
		//! Get total number of nodes.
		size_t get_total_num_of_node() const { return nodes_.size();}
		//! Get active number of nodes.
		size_t get_active_num_of_node() const;
		//! Get shape of element matrix.
		std::array<size_t, 2> get_matrix_shape() const;
		//! Get shape of rhs matrix.
		std::array<size_t, 2> get_rhs_shape() const;
		//! Get shape of result matrix.
		std::array<size_t, 2> get_result_shape() const;
		//! Get global dofs array.
		std::vector<int> get_element_dofs() const { return global_dofs_;}
		//! Get mass format.
		bool is_lumped_mass() const { return 0 < keyopt_[0];}
		//! Print information.
		friend std::ostream& operator<<(std::ostream& cout, const Element &a) {
			cout << fmt::format("Element id:{}\t", a.id_);
			cout << fmt::format("Num node:{}\t", a.get_total_num_of_node());
			cout << fmt::format("Active node:{}\t", a.get_active_num_of_node());
			cout << fmt::format("Order:{}\t", a.get_element_order());
			cout << fmt::format("Dof per node:{}\t", a.get_dofs_per_node());
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
		matrix_<T> result_;//!< Result of element.

		cmatrix_<T> rhs_cmplx_;//!< Right-hand matrix of element in complex.
		cmatrix_<T> load_cmplx_;//!< Load matrix of element in complex.
		cmatrix_<T> result_cmplx_;//!< Result of element in complex.
};

#include "element_ext.hpp"
#include "element_cmplx_ext.hpp"

//! Specialization.
template class Element<REAL8>;
template class Element<REAL4>;
}  // namespace cafea
#endif  // CAFEA_ELEMENT_H_
