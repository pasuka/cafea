#ifndef ELEMENT_H
#define ELEMENT_H

#include <cassert>
#include <cstddef>
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
		Element(int id, 
			int et, 
			int mp, 
			init_list_<size_t> nodes):id(id), etype(et), matl(mp), name("Element")
		{
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
		std::vector<size_t> get_node_list() const {return node_list;};
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
		//! Set order of element.
		// void set_element_order(int x){order = x;};
		//! Set dofs of each node.
		// void set_dofs_per_node(int x){dofs_per_node = x;};
		//! set type of element.
		void set_element_type(ElementType et){etype_val = et;};
		
		//! Get material id.
		int get_material_id() const {return matl;};
		//! Get type of element.
		int get_element_type() const {return etype;};
		//! Get type of element.
		ElementType get_element_type() const {return etype_val;};
		//! Get id of element.
		int get_element_id() const {return id;};
		//! Get order of element.
		int get_element_order() const;
		//! Get dofs of each node.
		int get_dofs_per_node() const;
		//! Get total number of nodes.
		size_t get_total_num_of_nodes() const {return node_list.size();};
		//! Get active number of nodes.
		int get_active_num_of_nodes() const;
		
 	private:
		int etype{-1};//!< Type of element.
		int matl{-1};//!< Material id.
		// int order{-1};//!< Order of element.
		// int dofs_per_node{-1};//!< Dofs for each node.
		std::array<int, 8> keyopt{-1, -1, -1, -1, -1, -1, -1, -1};//!< Parameters of element.
		std::vector<size_t> node_list;//!< Array of node list.
		
		ElementType etype_val;//!< Element type enum.
		
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

namespace
{
template <class T, class U>
using varargout_base = std::tuple<T, T, T, U>;
using varargout = varargout_base<Eigen::MatrixXd, Eigen::VectorXd>;
}	

namespace pipe_elem_lib
{
//! 2-node straight pipe element.
varargout pipe16(const Node[2], const Material&);
//! 2-node straight pipe element.
varargout pipe16(const Node&, const Node&, const Material&);
//! 2-node curved pipe element.
varargout pipe18(const Node[3], const Material&);
//! 2-node curved pipe element.
varargout pipe18(const Node&, const Node&, const Node&, const Material&);
}

namespace additional_elem_lib
{
//! 1-node mass element.
varargout mass21(const Node&, const Material&, const int[]);
//! 2-node spring element.
varargout combin14(const Node&, const Node&, const Material&, const int[]);
//! 2-node spring element.
varargout combin14(const Node[2], const Material&, const int[]);
}

namespace solid_elem_lib
{
//! 4-node tetrahedron solid element.
varargout c3d4(const Node[4], const Material&);
//! 4-node tetrahedron solid element.
varargout c3d4(const Node&, const Node&, const Node&, const Node&, const Material&);
//! 8-node hexahedron solid element.
varargout c3d8(const Node[8], const Material&);
//! 8-node hexahedron solid element.
varargout c3d8(const Node[8], const Material&, const int[]);
//! 20-node second order hexahedron solid element.
varargout c3d20(const Node[20], const Material&);
//! 27-node full second order hexahedron solid element.
varargout c3d27(const Node[27], const Material&);
}

namespace shell_elem_lib
{
//! 3-node triangle shell element.
varargout s3r(const Node&, const Node&, const Node&, const Material&);
//! 3-node triangle shell element.
varargout s3r(const Node[3], const Material&);
//! 4-node quadrangle shell element.
varargout s4r(const Node&, const Node&, const Node&, const Node&, const Material&);
//! 4-node quadrangle shell element.
varargout s4r(const Node[4], const Material&);
//! 8-node second order quadrangle shell element.
varargout s8r(const Node[8], const Material&);
//! 9-node full second order quadrangle shell element.
varargout s9r(const Node[9], const Material&);
//! 16-node full thrid order quadrangle shell element.
varargout s16r(const Node[16], const Material&);
//! 25-node full fourth order quadrangle shell element.
varargout s25r(const Node[25], const Material&);
//! 36-node full fifth order quadrangle shell element.
varargout s36r(const Node[36], const Material&);
}

namespace beam_elem_lib
{
//! 2-node straight beam element.
varargout b31(const Node[2], const Material&, const float[]);
//! 2-node straight beam element.
varargout b31(const Node[3], const Material&);
//! 2-node straight beam element.
varargout b31(const Node&, const Node&, const Material&, const float[]);
//! 2-node straight beam element.
varargout b31(const Node&, const Node&, const Node&, const Material&);
//! 2-node straight beam element.
varargout beam188(const Node[2], const Material&, const float[]);
//! 2-node straight beam element.
varargout beam188(const Node[3], const Material&);
//! 2-node straight beam element.
varargout beam188(const Node&, const Node&, const Material&, const float[]);
//! 2-node straight beam element.
varargout beam188(const Node&, const Node&, const Node&, const Material&);
//! 3-node straight beam element.
varargout b32(const Node[3], const Material&, const float[]);
//! 3-node straight beam element.
varargout b32(const Node[4], const Material&);
//! 3-node straight beam element.
varargout b32(const Node&, const Node&, const Node&, const Material&, const float[]);
//! 3-node straight beam element.
varargout b32(const Node&, const Node&, const Node&, const Node&, const Material&);
//! 3-node straight beam element.	
varargout beam189(const Node[3], const Material&, const float[]);
//! 3-node straight beam element.
varargout beam189(const Node[4], const Material&);
//! 3-node straight beam element.
varargout beam189(const Node&, const Node&, const Node&, const Material&, const float[]);
//! 3-node straight beam element.
varargout beam189(const Node&, const Node&, const Node&, const Node&, const Material&);
}
}
#endif