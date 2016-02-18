#ifndef NODE_H
#define NODE_H

#include <cassert>
#include <string>
#include <vector>
#include <initializer_list>

#include <Eigen/Dense>

#include "base.h"

namespace cafea {
	
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
		Node(int id, REAL8 x, REAL8 y, REAL8 z):id(id), csys(0){
			assert(id>0&&csys>=0);
			xyz << x, y, z;
		}
		/**
		 *  \brief Initialize with node's id coordinate system and values.
		 *  \param [in] id an integer must bigger than zero.
		 *  \param [in] csys coordinate system type.
		 *  \param [in] u1 value of axis-1.
		 *  \param [in] u2 value of axis-2.
		 *  \param [in] u3 value of axis-3.
		 */
		Node(int id, int csys, REAL8 u1, REAL8 u2, REAL8 u3):id(id), csys(csys){
			assert(id>0&&csys>=0);
			xyz << u1, u2, u3;
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
		Node(int id, REAL8 x, REAL8 y, REAL8 z, REAL8 rotx, REAL8 roty, REAL8 rotz):id(id), csys(0){
			assert(id>0&&csys>=0);
			xyz << x, y, z;
			angle << rotx, roty, rotz;
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
		//! Get dofs vector. 
		std::vector<int> get_dofs() const {return dofs;};
		//! Append dofs vector.
		void append_dofs(int i){dofs.push_back(i);};
		//! Clear dofs vector.
		void clear_dofs() const {dofs.clear();};
		
		//! Get coordinate system
		int get_csys() const {return csys;};
		//! Set coordinate system
		void set_csys(int i) {csys = i;};
		
		//! Get xyz values.
		Eigen::Vector3d get_xyz() const {return xyz;};
		//! Get x value.
		REAL8 get_x() const {return xyz[0];};
		//! Get y value.
		REAL8 get_y() const {return xyz[1];};
		//! Get z value.
		REAL8 get_z() const {return xyz[2];};
		
		//! Set xyz values.
		void set_xyz(REAL8 x, REAL8 y, REAL8 z){xyz << x, y, z;};
		//! Set xyz values with 1d array.
		void set_xyz(REAL8 val[3]){xyz << val[0], val[1], val[2];};
		//! Set xyz values with list.
		void set_xyz(std::initializer_list<REAL8> val){
			assert(val.size()==3);
			xyz << val[0], val[1], val[2];
		};
		//! Set x value.
		void set_x(REAL8 x){xyz[0] = x;};
		//! Set y value.
		void set_y(REAL8 y){xyz[1] = y;};
		//! Set z value.
		void set_z(REAL8 z){xyz[2] = z;};
	
		//! Set Euler angle values.
		void set_angle(REAL8 r1, REAL8 r2, REAL8 r3){angle << r1, r2, r3;};
		//! Set Euler angle values with 1d array.
		void set_angle(REAL8 val[3]){angle << val[0], val[1], val[2];};
		//! Set Euler angle values with list.
		void set_angle(std::initializer_list<REAL8> val){
			assert(val.size()==3);
			angle << val[0], val[1], val[2];
		};
		//! Get Euler angle values.
		Eigen::Vector3d get_angle() const {
			if((angle>180.0).any()){
				return Eigen::Vector3d::Zero();
			}
			else{
				return angle;
			};
		};
		
	private:
		int csys{-1};//!< Coordinate system.
		Eigen::Vector3d xyz(0.0, 0.0, 0.0);//!< Values of coordinate.
		Eigen::Vector3d angle(181.0, 181.0, 181.0);//!< Euler's angle in degree.
		std::vector<int> dofs;//!< Storage of Degree of freedoms.
		Eigen::MatrixXd mode_shape;//!< Storage of mode shape.
		Eigen::VectorXd range;//!< Storage of time- or frequency- domain range.
		
		using REAL8 = double;//!< ISO_C_Binding Doube == Real(kind=8)
		//! C++11's trick for template aliase.
		template <class U>
		using matrix_ = Eigen::Matrix<U, Eigen::Dynamic, Eigen::Dynamic>;
		matrix_<T> pres;//!< Storage of pressure.
		matrix_<T> disp;//!< Storage of displacement.
		matrix_<T> vel;//!< Storage of velocity.
		matrix_<T> accel;//!< Storage of acceleration.
		matrix_<T> stress;//!< Storage of stress.	
};
}
#endif