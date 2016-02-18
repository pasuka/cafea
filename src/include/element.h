#ifndef ELEMENT_H
#define ELEMENT_H

#include <cassert>
#include <array>
#include <vector>
#include <initializer_list>

#include <Eigen/Dense>

#include "base.h"

namespace cafea {

/**
 * Element object definition. 
 */
template <class T>
class Element: public ObjectBase {
	public:
		using ObjectBase::ObjectBase;// Inherit parent's constructors.
		//* Default constructor.
		Element(){};
		//* Destructor.
		~Element(){
			node_list.clear();
			m_mass.resize(0, 0);
			m_stif.resize(0, 0);
			m_tran.resize(0, 0);
			m_rhs.resize(0, 0);
			m_stress.resize(0, 0);
			m_reaction_force.resize(0, 0);
		};
		/**
		 *  \brief Initialize with element id type material type and node list.
		 *  \param [in] id an integer must bigger than zero.
		 *  \param [in] et number of element type.
		 *  \param [in] mp number of material type.
		 *  \param [in] nodes list of nodes id.
		 */
		Element(int id, int et, int mp, init_list_<int> nodes):id(id), etype(et), matl(mp){
			assert(id>0&&etype>0&&matl>0);
			for(const auto& it: nodes)node_list.push_back(it);
		}
		//! Generate stifness mass matrix of element.
		void form_matrix();
		//! Get stiffness matrix.
		Eigen::MatrixXd get_stif() const {return m_stif;};
		//! Get mass matrix.
		Eigen::MatrixXd get_mass() const {return m_mass;};
		//! Get transpose matrix.
		Eigen::MatrixXd get_tran() const {return m_tran;};
		//! Get right-hand side matrix.
		Eigen::MatrixXd get_rhs() const {return m_rhs;};
		//! Get raw pointer of stiffness matrix.
		double *get_stif_ptr() const {return m_stif.data();};
		//! Get raw pointer of mass matrix.
		double *get_mass_ptr() const {return m_mass.data();};
		//! Get raw pointer of transpose matrix.
		double *get_tran_ptr() const {return m_tran.data():};
		//! Get raw pointer of right-hand side matrix.
		double *get_rhs_ptr() const {return m_rhs.data();};
		
		//! Get stress matrix.
		matrix_<T> get_stress() const {return m_stress;};
		//! Get raw pointer of stress matrix.
		T *get_stress_ptr() const {return m_stress.data();};
		
		//! Post process.
		void post_stress();
		
		//! Get node list.
		std::vector<int> get_node_list() const {return node_list;};
		//! Set node list.
		void set_node_list(const int a[], int m){
			if(!node_list.empty())node_list.clear();
			for(int i=0; i<m; i++)node_list.push_back(a[i]);
		};
		//! Set node list with c++11 initializer_list.
		void set_node_list(init_list_<int> a){
			if(!node_list.empty())node_list.clear();
			for(const auto& it: a)node_list.push_back(it);
		};
		//! Set id of element.
		void set_element_id(int x){id = x;};
		//! Set type of element.
		void set_element_type(int x){etype = x;};
		//! Set material id.
		void set_material_id(int x){matl = x;};		
		//! Set lump of mass matrix.
		void set_lump(bool val){lump = val;};
		//! Set order of element.
		void set_element_order(int x){order = x;};
		//! Set dofs of each node.
		void set_dofs_per_node(int x){dofs_per_node = x;};
		
		//! Get material id.
		int get_material_id() const {return matl;};
		//! Get type of element.
		int get_element_type() const {return etype;};
		//! Get id of element.
		int get_element_id() const {return id;};
		//! Get order of element.
		int get_element_order() const {return order;};
		//! Get dofs of each node.
		int get_dofs_per_node() const {return dofs_per_node;};
		//! Inquire lump mass matrix or not.
		bool is_lump() const {return lump;};
		
 	private:
		int etype{-1};//!< Type of element.
		int matl{-1};//!< Material id.
		int order{-1};//!< Order of element.
		int dofs_per_node{-1};//!< Dofs for each node.
		std::array<int, 8> keyopt{-1, -1, -1, -1, -1, -1, -1, -1};//!< Parameters of element.
		std::vector<int> node_list;//!< Array of node list.
		
		bool lump{false};//!< Lump mass matrix of element.
		
		Eigen::MatrixXd m_stif;//!< Stiffness matrix of element.
		Eigen::MatrixXd m_mass;//!< Mass matrix of element.
		Eigen::MatrixXd m_tran;//!< Transpose matrix of element.
		Eigen::MatrixXd m_rhs;//!< Right-hand side of element.
		
		template <class U>
		using init_list_ = std::initializer_list<U>;
		template <class U>
		using matrix_ = Eigen::Matrix<U, Eigen::Dynamic, Eigen::Dynamic>;
		matrix_<T> m_stress;//!< Stress of element.
		matrix_<T> m_reaction_force;//!< Reaction force of element.
};
}
#endif