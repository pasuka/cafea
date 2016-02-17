#ifndef BASE_H
#define BASE_H

#include <cmath>
#include <cassert>
#include <array>
#include <string>
#include <vector>
#include <limits>
#include <initializer_list>

#include <Eigen/Dense>

namespace cafea {

//! Precision of machine via different types.	
template<class T>
constexpr T EPS() {return std::numeric_limits<T>::epsilon();};
//! M_PI
template<class T>
constexpr T PI() {return T(std::atan(1.0)*4.0);};

/**
 *  Basic parent object.
 */
class ObjectBase {
	public:
		//! A constructor.
		ObjectBase(){};
		//! A destructor.
		~ObjectBase(){};
		//! Set numeric number for object's name.
		template <class T>
		void set_name(T val){name = std::to_string(val);};
		//! Set object's name.
		void set_name(std::string s){name = s};
		void set_name(const char *cs){name = cs;};
		//! Set object's id.
		void set_id(int x){id = x;};
		//! Set object's group via C++11. 
		/**
		 *  \code{.cpp}
		 *  ObjectBase a;
		 *  a.set_group({1,3,4,5,8,2});
		 *  \endcode
		 */
		void set_group(std::initializer_list<int> abc){
			assert(abc.size()<=8);
			int i{0};
			for(auto const &it: abc)group[i++] = it;
		}
		//! Set object's group via C-style.
		void set_group(const int y[], int n){
			assert(n<=8);
			for(int i=0; i<n; i++)group[i] = y[i];
		};
		//! Get object's name.
		std::string get_name() const {return name;};
		//! Get object's id.
		int get_id() const {return id;};
		//! Get object's group.
		std::array<int, 8> get_group() const {return group;};
		
	private:
		int id{-1};//!< Object's id
		std::array<int, 8> group{-1, -1, -1, -1, -1, -1, -1, -1};//!< Object's group array.
		std::string name{"This is a base object!"};//!< Object's name.
};

/**
 *  Node object definition.
 */
template <class T>
class Node: public ObjectBase {
	public:
		using ObjectBase::ObjectBase;// Inherit Base's constructors.
		//! Default constructor.
		Node(){};
		/**
		 *  \brief Initialize with node's id.
		 *  \param [in] id an integer must bigger than zero.
		 */
		Node(int id):id(id), csys(0){assert(id>0);};
		/**
		 *  \brief Initialize with node's id and coordinate system.
		 *  \param [in] id an integer must bigger than zero.
		 *  \param [in] csys coordinate system types.
		 *  
		 *  coordinate system types list.
		 *  | Types     | Values |
		 *  |:=========:|:======:|
		 *  |Cartesian  |0       |
		 *  |Cylindrical|1       |
		 *  |Spherical  |2       |
		 */
		Node(int id, int csys):id(id), csys(csys){assert(id>0&&csys>=0);};
		/**
		 *  \brief Initialize with node's id and x y z coordinate values.
		 *  \param [in] id an integer must bigger than zero.
		 *  \param [in] x value of x-axis.
		 *  \param [in] y value of y-axis.
		 *  \param [in] z value of z-axis.
		 *  
		 *  \details Default coordinate system Cartesian.
		 */
		Node(int id, float x, float y, float z):id(id), csys(0){
			assert(id>0&&csys>=0);
			xyz[0] = x; xyz[1] = y; xyz[2] = z;
		};
		/**
		 *  \brief Initialize with node's id coordinate system and values.
		 *  \param [in] id an integer must bigger than zero.
		 *  \param [in] csys coordinate system type.
		 *  \param [in] u1 value of axis-1.
		 *  \param [in] u2 value of axis-2.
		 *  \param [in] u3 value of axis-3.
		 */
		Node(int id, int csys, float u1, float u2, float u3):id(id), csys(csys){
			assert(id>0&&csys>=0);
			xyz[0] = u1; xyz[1] = u2; xyz[2] = u3;
		};
		/**
		 *  \brief Initialize with node's id coordinate values and Euler angles.
		 *  \param [in] id an integer must bigger than zero.
		 *  \param [in] x value of x-axis.
		 *  \param [in] y value of y-axis.
		 *  \param [in] z value of z-axis.
		 *  \param [in] rotx value of rotate x-axis.
		 *  \param [in] roty value of rotate y-axis.
		 *  \param [in] rotz value of rotate z-axis.
		 *  
		 *  \details Euler angle must in degrees.
		 */
		Node(int id, float x, float y, float z, float rotx, float roty, float rotz):id(id), csys(0){
			assert(id>0&&csys>=0);
			xyz[0] = x; xyz[1] = y; xyz[2] = z;
			angle[0] = rotx; angle[1] = roty; angle[2] = rotz;
		};
		//! Destructor.
		~Node(){
			dofs.clear();
			range.resize(0);
			mode_shape.resize(0, 0);
			vel.resize(0, 0);
			disp.resize(0, 0);
			pres.resize(0, 0);
			accel.resize(0, 0);
			stress.resize(0, 0);
		};
		//! Inquire if this node is used in FE analysis.
		bool is_active(){return !dofs.empty();}; 
	private:
		int csys{-1};//!< Coordinate system.
		float xyz[3]{0.0f, 0.0f, 0.0f};//!< Values of coordinate.
		float angle[3]{181.0f, 181.0f, 181.0f};//!< Euler's angle in degree.
		std::vector<int> dofs;//!< Storage of Degree of freedoms.
		Eigen::MatrixXd mode_shape;//!< Storage of mode shape.
		Eigen::VectorXd range;//!< Storage of time- or frequency- domain range.
		
		//! C++11's trick for template aliase.
		template <class U>
		using matrix = Eigen::Matrix<U, Eigen::Dynamic, Eigen::Dynamic>;
		matrix<T> pres;//!< Storage of pressure.
		matrix<T> disp;//!< Storage of displacement.
		matrix<T> vel;//!< Storage of velocity.
		matrix<T> accel;//!< Storage of acceleration.
		matrix<T> stress;//!< Storage of stress.
		
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
		Element(int id, int et, int mp, std::initializer_list<int> nodes):id(id), etype(et), matl(mp){
			assert(id>0&&etype>0&&matl>0);
			for(const auto& it: nodes)node_list.push_back(it);
		}
		//! Generate stifness mass matrix of element.
		void form_matrix();
	private:
		int etype{-1};//!< Type of element.
		int matl{-1};//!< Material number.
		int keyopt[8]{-1, -1, -1, -1, -1, -1, -1, -1};//!< Parameters of element.
		std::vector<int> node_list;//!< Array of node list.
		
		bool lump{false};//!< Lump mass matrix of element.
		
		Eigen::MatrixXd m_stif;//!< Stiffness matrix of element.
		Eigen::MatrixXd m_mass;//!< Mass matrix of element.
		Eigen::MatrixXd m_tran;//!< Transpose matrix of element.
		Eigen::MatrixXd m_rhs;//!< Right-hand side of element.
		
		template <class U>
		using matrix = Eigen::Matrix<U, Eigen::Dynamic, Eigen::Dynamic>;
		matrix<T> m_stress;//!< Stress of element.
		matrix<T> m_reaction_force;//!< Reaction force of element.
}
}
#endif