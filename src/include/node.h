#ifndef NODE_H
#define NODE_H

#include <ostream>

#include <Eigen/Dense>
#include "fmt/format.h"

#include "base.h"
#include "dof_handler.h"

namespace cafea
{
/**
 *  Basic node object definition.
 */
template <class Scalar>
class NodeBase: public ObjectBase {
	public:
		using ObjectBase::ObjectBase;//!< Inherit Base's constructors.
		//! Default constructor.
		NodeBase(){};
		/**
		 *  \brief Initialize with node's id.
		 *  \param [in] id an integer must bigger than zero.
		 */
		NodeBase(int id):ObjectBase{id, fmt::format("Node#{0}", id)}, 
			csys_(CoordinateSystem::CARTESIAN) {};
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
		NodeBase(int id, CoordinateSystem csys):csys_(csys),
			ObjectBase{id, fmt::format("Node#{0}", id)} {};
		/**
		 *  \brief Initialize with node's id and x y z coordinate values.
		 *  \param [in] id an integer must bigger than zero.
		 *  \param [in] x value of x-axis.
		 *  \param [in] y value of y-axis.
		 *  \param [in] z value of z-axis.
		 *  
		 *  \details Default coordinate system Cartesian.
		 */
		NodeBase(int id, Scalar x, Scalar y, Scalar z):
			ObjectBase{id, fmt::format("Node#{0}", id)},
			csys_(CoordinateSystem::CARTESIAN) { xyz_ << x, y, z;};
		/**
		 *  \brief Initialize with node's id coordinate system and values.
		 *  \param [in] id an integer must bigger than zero.
		 *  \param [in] csys coordinate system type.
		 *  \param [in] u1 value of axis-1.
		 *  \param [in] u2 value of axis-2.
		 *  \param [in] u3 value of axis-3.
		 */
		NodeBase(int id, CoordinateSystem csys,
			Scalar u1, Scalar u2, Scalar u3):csys_(csys),
			ObjectBase{id, fmt::format("Node#{0}", id)} { xyz_ << u1, u2, u3;};
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
		NodeBase(int id, Scalar x, Scalar y, Scalar z,
			Scalar rotx, Scalar roty, Scalar rotz):
			csys_(CoordinateSystem::CARTESIAN),
			ObjectBase{id, fmt::format("Node#{0}", id)}
		{
			xyz_ << x, y, z;
			angle_ << rotx, roty, rotz;
		};
		/**
		 *  \brief Initialize with node's id coordinate and Euler angles.
		 *  \param [in] id an integer must bigger than zero.
		 *  \param [in] csys coordinate system type.
		 *  \param [in] value of 1-axis.
		 *  \param [in] value of 2-axis.
		 *  \param [in] value of 3-axis.
		 *  \param [in] value of rotate 1-axis.
		 *  \param [in] value of rotate 2-axis.
		 *  \param [in] value of rotate 3-axis.
		 */
		NodeBase(int id, CoordinateSystem csys, Scalar u1, Scalar u2, Scalar u3,
			Scalar ur1, Scalar ur2, Scalar ur3):csys_(csys),
			ObjectBase{id, fmt::format("Node#{0}", id)}
		{
			xyz_ << u1, u2, u3;
			angle_ << ur1, ur2, ur3;
		};
		/**
		 *  \brief Initialize with node's id and coordinate system and values.
		 */
		NodeBase(int id, CoordinateSystem csys, init_list_<Scalar> val):
			ObjectBase{id, fmt::format("Node#{0}", id)}, csys_(csys)
		{
			assert(val.size()==3);
			int i{0};
			for(auto &it: val)xyz_(i++) = it;
		};
		/**
		 *  \brief Initialize with node's id and coordinate values.
		 */
		NodeBase(int id, init_list_<Scalar> val):
			csys_(CoordinateSystem::CARTESIAN),
			ObjectBase{id, fmt::format("Node#{0}", id)}
		{
			assert(val.size()==3);
			int i{0};
			for(auto &it: val)xyz_(i++) = it;
		};
		/**
		 *  \brief Initialize with node's id and coordinate and Euler angles.
		 */
		NodeBase(int id, CoordinateSystem csys, init_list_<Scalar> val,
			init_list_<Scalar> val2):csys_(csys),
			ObjectBase{id, fmt::format("Node#{0}", id)}
		{
			assert(val.size()==3&&val2.size()==3);
			int i{0}, j{0};
			for(auto &it: val)xyz_(i++) = it;
			for(auto &it: val2)angle_(j++) = it;
		};
		/**
		 *  \brief Initialize with node's id and coordinate and Euler angles.		
		 */
		NodeBase(int id, init_list_<Scalar> val, init_list_<Scalar> val2):
			csys_(CoordinateSystem::CARTESIAN),
			ObjectBase{id, fmt::format("Node#{0}", id)}
		{
			assert(val.size()==3&&val2.size()==3);
			int i{0}, j{0};
			for(auto &it: val)xyz_(i++) = it;
			for(auto &it: val2)angle_(j++) = it;
		};
		//! Destructor.
		~NodeBase() override {};
		//! Get coordinate system.
		CoordinateSystem get_csys() const {return csys_;};
		//! Set coordinate system.
		void set_csys(CoordinateSystem ct) {csys_ = ct;};
		
		//! Get xyz values.
		vec3_<Scalar> get_xyz() const {return xyz_;};
		//! Get x value.
		Scalar get_x() const {return xyz_(0);};
		//! Get y value.
		Scalar get_y() const {return xyz_(1);};
		//! Get z value.
		Scalar get_z() const {return xyz_(2);};
		
		//! Set xyz values.
		void set_xyz(Scalar x, Scalar y, Scalar z) {xyz_ << x, y, z;};
		//! Set xyz values with 1d array.
		void set_xyz(Scalar val[3]) {xyz_ << val[0], val[1], val[2];};
		//! Set xyz values with list.
		void set_xyz(init_list_<Scalar> val)
		{
			assert(val.size()==3);
			xyz_ << val[0], val[1], val[2];
		};
		//! Set x value.
		void set_x(Scalar x) {xyz_(0) = x;};
		//! Set y value.
		void set_y(Scalar y) {xyz_(1) = y;};
		//! Set z value.
		void set_z(Scalar z) {xyz_(2) = z;};
	
		//! Set Euler angle values.
		void set_angle(Scalar r1, Scalar r2, Scalar r3) {angle_ << r1, r2, r3;};
		//! Set Euler angle values with 1d array.
		void set_angle(Scalar val[3]) {angle_ << val[0], val[1], val[2];};
		//! Set Euler angle values with list.
		void set_angle(init_list_<Scalar> val)
		{
			assert(val.size()==3);
			angle_ << val[0], val[1], val[2];
		};
		//! Get Euler angle values.
		Scalar get_rot_x() const {return angle_(0)>Scalar(180) ? Scalar(0): angle_(0);};
		//! Get Euler angle values.
		Scalar get_rot_y() const {return angle_(1)>Scalar(180) ? Scalar(0): angle_(1);};
		//! Get Euler angle values.
		Scalar get_rot_z() const {return angle_(2)>Scalar(180) ? Scalar(0): angle_(2);};
		//! Get Euler angle values.
		template <int x>
		Scalar get_rot() const {return angle_(x)>Scalar(180) ? Scalar(0): angle_(x);};
		//! Get Euler angle values.
		Scalar get_rot(int x) const {return angle_(x)>Scalar(180) ? Scalar(0): angle_(x);};
		//! Get Euler angle in rad.
		Scalar get_rot_rad(int x) const {return PI<Scalar>()*get_rot(x)/Scalar(180);};
		//! Get Euler transform matrix.
		Eigen::Matrix<Scalar, 3, 3> get_euler_tran() const;
		//! Get Euler angle values.
		vec3_<Scalar> get_angle() const
		{
			if((angle_.array()>Scalar(180)).any())
			{
				return vec3_<Scalar>::Zero();
			}
			else{
				return angle_;
			};
		};
		//! Print node information.
		friend std::ostream& operator<<(std::ostream& cout, const NodeBase &a)
		{
			cout << a.name_ << "\t";
			switch(a.csys_){
			case CoordinateSystem::CARTESIAN:
				cout << "Cartesian\n";
				break;
			case CoordinateSystem::SPHERICAL:
				cout << "Spherical\n";
				break;
			case CoordinateSystem::CYLINDRICAL:
				cout << "Cylindrical\n";
				break;
			default:
				cout << "Unknown\n";
			}
			cout << fmt::format("X:{0}\tY:{1}\tZ:{2}\n", a.get_x(), a.get_y(), a.get_z());
			if(!(a.angle_.array()>Scalar(180)).any())
			{
				std::array<std::string, 3> tmp = {"Yaw", "Pitch", "Roll"};
				cout << "Euler angle ";
				for(int i=0; i<3; i++){
					cout << fmt::format("{0}:{1} ", tmp[i], a.get_rot(i));
				}
				cout << "\n";
			}
			return cout;
		};
	protected:
		CoordinateSystem csys_{CoordinateSystem::CARTESIAN};//!< Coordinate system.
		vec3_<Scalar> xyz_ = vec3_<Scalar>::Zero();//!< Values of coordinate.
		vec3_<Scalar> angle_ = vec3_<Scalar>::Constant(181);//!< Euler's angle in degree.		
};

/**
 *  Node object definition.
 */
template <class Scalar, class ResultScalar>
class Node: public NodeBase<Scalar> {
	public:
		using NodeBase<Scalar>::NodeBase;// Inherit Base's constructors.
		//! Default constructor.
		Node(){};
		//! Destructor.
		~Node() override
		{
			disp_.resize(0, 0);
			vel_.resize(0, 0);
			accel_.resize(0, 0);
			stress_.resize(0, 0);
			disp_cmplx_.resize(0, 0);
			vel_cmplx_.resize(0, 0);
			accel_cmplx_.resize(0, 0);
			stress_cmplx_.resize(0, 0);
			range_.resize(0);
			dof_mgr_.clear();
		};
		//! DOF manager init.
		void dof_init(ElementType et);
		//! DOF accumulate.
		void dof_accum(int *ij, DofType mt=DofType::NORMAL);
		//! DOF apply boundary and load.
		void dof_apply(Boundary<Scalar> bc);
		//! DOF vector.
		const std::vector<int> dof_list() const {return dof_mgr_.get_dofs();};
		//! Activate node.
		void activate(bool stat=true) {activate_ = stat;};
		//! Check status of node.
		bool is_activated() const {return activate_;};
		//! Print.
		friend std::ostream& operator<<(std::ostream& cout, const Node &a)
		{
			// cout << "This Node information\n";
			if(a.activate_){
				cout << fmt::format("Node:{} is used\t", a.get_id());
				cout << fmt::format("Dofs:{}\n", a.dof_mgr_.get_num_dofs());
			}
			else{
				cout << fmt::format("Node:{} is unused\n", a.get_id());
			}
			// return cout << static_cast<const NodeBase<Scalar>&>(a);
			cout << a.dof_mgr_;
			return cout;
		};
		//! Initialize result container.
		bool init_result(SolutionType, int);
	private:
		DofHandler dof_mgr_;//!< Dof manager.
		bool activate_{false};//!< Status of node.
		
		vecX_<ResultScalar> range_;//!< Range of result.
		matrix_<ResultScalar> disp_;//!< Storage of displacement.
		matrix_<ResultScalar> vel_;//!< Storage of velocity.
		matrix_<ResultScalar> accel_;//!< Storage of acceleration.
		matrix_<ResultScalar> stress_;//!< Storage of stress.
		matrix_<std::complex<ResultScalar>> disp_cmplx_;//!< Storage of displacement in complex.
		matrix_<std::complex<ResultScalar>> vel_cmplx_;//!< Storage of velocity in complex.
		matrix_<std::complex<ResultScalar>> accel_cmplx_;//!< Storage of acceleration in complex.
		matrix_<std::complex<ResultScalar>> stress_cmplx_;//!< Storage of stress in complex.
};

//! Specialization.
template class Node<REAL4, REAL4>;
template class Node<REAL4, REAL8>;
template class Node<REAL8, REAL8>;
template class Node<REAL8, REAL4>;

/**
 *  \brief Coordinate transform uitiliy.
 */
template <class T, class U>
struct NodeFunc{
	//! Coordinate transform for 2-node element.
	static varargout_2_<U> coord_tran(const NodeBase<T>*, const NodeBase<T>*);
	//! Coordinate transform for 2-node and up direction.
	static varargout_2_<U> coord_tran(const NodeBase<T>*, const NodeBase<T>*,
		const T[]);
	//! Coordinate transform for triangle.
	static varargout_3_<U> coord_tran(const NodeBase<T>*, const NodeBase<T>*,
		const NodeBase<T>*);
	//! Coordinate transform for quadrangle.
	static varargout_3_<U> coord_tran(const NodeBase<T>*, const NodeBase<T>*,
		const NodeBase<T>*, const NodeBase<T>*);
};

//! Specialization.
template struct NodeFunc<REAL4, REAL4>;
template struct NodeFunc<REAL4, REAL8>;
template struct NodeFunc<REAL8, REAL4>;
template struct NodeFunc<REAL8, REAL8>;

}
#endif