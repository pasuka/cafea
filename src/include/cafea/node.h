/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#ifndef CAFEA_NODE_H_
#define CAFEA_NODE_H_

#include <vector>
#include <string>
#include <typeinfo>
#include <typeindex>
#include <functional>

#include <Eigen/Dense>
#include <Eigen/Geometry>

#include "cafea/base.h"
#include "cafea/boundary.h"
#include "cafea/utils_ext.h"
#include "cafea/dof_handler.h"
#include "cafea/element_attr.h"

namespace cafea {
/**
 *  \class Base definition of node.
 *  Basic node object definition.
 */
template <class T = REAL4>
class NodeBase: public ObjectBase {
	public:
		using ObjectBase::ObjectBase;//!< Inherit Base's constructors.
		//! Default constructor.
		NodeBase() = delete;
		/**
		 *  \brief Initialize with node's id.
		 *  \param [in] id an integer must bigger than zero.
		 */
		explicit NodeBase(int id): ObjectBase {id, fmt::format("Node#{0}", id)},
			csys_(CoordinateSystem::CARTESIAN) {}
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
		NodeBase(int id, CoordinateSystem csys): csys_(csys),
			ObjectBase {id, fmt::format("Node#{0}", id)} {}
		/**
		 *  \brief Initialize with node's id and x y z coordinate values.
		 *  \param [in] id an integer must bigger than zero.
		 *  \param [in] x value of x-axis.
		 *  \param [in] y value of y-axis.
		 *  \param [in] z value of z-axis.
		 *
		 *  \details Default coordinate system Cartesian.
		 */
		NodeBase(int id, T x, T y, T z): csys_(CoordinateSystem::CARTESIAN),
			ObjectBase {id, fmt::format("Node#{0}", id)} { xyz_ << x, y, z;}
		/**
		 *  \brief Initialize with node's id coordinate system and values.
		 *  \param [in] id an integer must bigger than zero.
		 *  \param [in] csys coordinate system type.
		 *  \param [in] u1 value of axis-1.
		 *  \param [in] u2 value of axis-2.
		 *  \param [in] u3 value of axis-3.
		 */
		NodeBase(int id, CoordinateSystem csys, T u1, T u2, T u3): csys_(csys),
			ObjectBase {id, fmt::format("Node#{0}", id)} { xyz_ << u1, u2, u3;}
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
		NodeBase(int id, T x, T y, T z, T rx, T ry, T rz): csys_(CoordinateSystem::CARTESIAN),
			ObjectBase {id, fmt::format("Node#{0}", id)} {
			xyz_ << x, y, z;
			angle_ << rx, ry, rz;
		}
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
		NodeBase(int id, CoordinateSystem csys, T u1, T u2, T u3, T ur1, T ur2, T ur3):
			csys_(csys), ObjectBase {id, fmt::format("Node#{0}", id)} {
			xyz_ << u1, u2, u3;
			angle_ << ur1, ur2, ur3;
		}
		/**
		 *  \brief Initialize with node's id and coordinate system and values.
		 */
		NodeBase(int id, CoordinateSystem csys, init_list_<T> val):
			ObjectBase {id, fmt::format("Node#{0}", id)}, csys_(csys) {
			assert(val.size() == 3);
			int i{0};
			for (auto &it: val) xyz_(i++) = it;
		}
		/**
		 *  \brief Initialize with node's id and coordinate values.
		 */
		NodeBase(int id, init_list_<T> val): csys_(CoordinateSystem::CARTESIAN),
			ObjectBase {id, fmt::format("Node#{0}", id)} {
			assert(val.size() == 3);
			int i{0};
			for (auto it: val) xyz_[i++] = it;
		}
		/**
		 *  \brief Initialize with node's id and coordinate and Euler angles.
		 */
		NodeBase(int id, CoordinateSystem csys, init_list_<T> va, init_list_<T> vb):
			csys_(csys), ObjectBase {id, fmt::format("Node#{0}", id)} {
			assert(va.size() == 3 && vb.size() == 3);
			int i{0}, j{0};
			for (auto it: va) xyz_(i++) = it;
			for( auto it: vb) angle_(j++) = it;
		}
		/**
		 *  \brief Initialize with node's id and coordinate and Euler angles.
		 */
		NodeBase(int id, init_list_<T> va, init_list_<T> vb): csys_(CoordinateSystem::CARTESIAN),
			ObjectBase {id, fmt::format("Node#{0}", id)} {
			assert(va.size() == 3 && vb.size() == 3);
			int i{0}, j{0};
			for (auto &it: va) xyz_(i++) = it;
			for (auto &it: vb) angle_(j++) = it;
		}
		//! Destructor.
		~NodeBase() override {}
		//! Get coordinate system.
		CoordinateSystem get_csys() const { return csys_;}
		//! Set coordinate system.
		void set_csys(CoordinateSystem ct) { csys_ = ct;}

		//! Get xyz values.
		vec3_<T> get_xyz() const { return xyz_;}
		//! Get x value.
		T get_x() const { return xyz_(0);}
		//! Get y value.
		T get_y() const { return xyz_(1);}
		//! Get z value.
		T get_z() const { return xyz_(2);}

		//! Set xyz values.
		void set_xyz(T x, T y, T z) { xyz_ << x, y, z;}
		//! Set xyz values with 1d array.
		void set_xyz(T val[3]) { xyz_ << val[0], val[1], val[2];}
		//! Set xyz values with list.
		void set_xyz(init_list_<T> val) {
			assert(val.size() == 3);
			xyz_ << val[0], val[1], val[2];
		}
		//! Set x value.
		void set_x(T x) { xyz_(0) = x;}
		//! Set y value.
		void set_y(T y) { xyz_(1) = y;}
		//! Set z value.
		void set_z(T z) { xyz_(2) = z;}

		//! Set Euler angle values.
		void set_angle(T r1, T r2, T r3) { angle_ << r1, r2, r3;}
		//! Set Euler angle values with 1d array.
		void set_angle(T val[3]) { angle_ << val[0], val[1], val[2];}
		//! Set Euler angle values with list.
		void set_angle(init_list_<T> val) {
			assert(val.size() == 3);
			angle_ << val[0], val[1], val[2];
		}
		//! Get Euler angle values.
		T get_rot_x() const { return get_rot(0);}
		//! Get Euler angle values.
		T get_rot_y() const { return get_rot(1);}
		//! Get Euler angle values.
		T get_rot_z() const { return get_rot(2);}
		//! Get Euler angle values.
		T get_rot(int x) const { return angle_(x) > T(180) ? T(0): angle_(x);}
		//! Get Euler angle in rad.
		T get_rot_rad(int x) const { return PI<T>()*get_rot(x)/T(180);}
		//! Get Euler transform matrix.
		Eigen::Matrix<T, 3, 3> get_euler_tran() const {
			using AA = Eigen::AngleAxis<T>;
			using mat33 = Eigen::Matrix<T, 3, 3>;
			mat33 tran = mat33::Identity();
			vec3_<T> IY = vec3_<T>::Zero(3), IX = IY, IZ = IY;

			if (angle_(0) < 1.8e2) {
				IX(0) = IY(1) = IZ(2) = T(1);
				T a0 = get_rot_rad(0);
				T a1 = get_rot_rad(1);
				T a2 = get_rot_rad(2);
				tran = AA(a2, IY)*AA(a1, IX)*AA(a0, IZ);
			}
			return tran;
		}
		//! Get Euler angle values.
		vec3_<T> get_angle() const {
			return (angle_.array() > T(180)).any() ? vec3_<T>::Zero(): angle_;
		}
		//! Print node information.
		friend std::ostream& operator<<(std::ostream& cout, const NodeBase &a) {
			cout << a.name_ << "\t";
			switch (a.csys_) {
				case CoordinateSystem::CARTESIAN:
					cout << "Cartesian\n";
					break;
				case CoordinateSystem::SPHERICAL:
					cout << "Spherical\n";
					break;
				case CoordinateSystem::CYLINDRICAL:
					cout << "Cylindrical\n";
					break;
				default: cout << "Unknown\n";
			}
			cout << fmt::format("X:{0}\tY:{1}\tZ:{2}\n", a.get_x(), a.get_y(), a.get_z());
			if (!(a.angle_.array() > T(180)).any()) {
				std::array<std::string, 3> tmp = {"Yaw", "Pitch", "Roll"};
				cout << "Euler angle ";
				for (int i = 0; i < 3; i++) {
					cout << fmt::format("{0}:{1} ", tmp[i], a.get_rot(i));
				}
				cout << "\n";
			}
			return cout;
		}

	protected:
		CoordinateSystem csys_{CoordinateSystem::CARTESIAN};//!< Coordinate system.
		vec3_<T> xyz_ = vec3_<T>::Zero();//!< Values of coordinate.
		vec3_<T> angle_ = vec3_<T>::Constant(181);//!< Euler's angle in degree.
};

/**
 *  \class Node definition.
 *  Node object definition.
 */
template <class T = REAL4, class U = REAL8>
class Node: public NodeBase<T> {
	public:
		using NodeBase<T>::NodeBase;//!< Inherit Base's constructors.
		//! Default constructor.
		Node() = delete;
		//! Destructor.
		~Node() override {
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
		}
		//! DOF manager init.
		void dof_init(ElementType et);
		//! DOF accumulate with default dof type.
		void dof_accum(int *ij, DofType mt = DofType::NORMAL) { dof_accum_2(ij, mt);}
		//! DOF apply boundary and load.
		void dof_apply(Boundary<T> bc);
		//! DOF vector.
		std::function<std::vector<int>()> dof_list = std::bind(&DofHandler::get_dofs, &dof_mgr_);
		//! Activate node.
		void activate(bool stat = true) { activate_ = stat;}
		//! Check status of node.
		bool is_activated() const { return activate_;}
		//! Print.
		friend std::ostream& operator<<(std::ostream& cout, const Node &a) {
			// cout << "This Node information\n";
			if (a.activate_) {
				cout << fmt::format("Node:{} is used\t", a.get_id());
				cout << fmt::format("Dofs:{}\n", a.dof_mgr_.get_num_dofs());
			} else {
				cout << fmt::format("Node:{} is unused\n", a.get_id());
			}
			// return cout << static_cast<const NodeBase<T>&>(a);
			cout << a.dof_mgr_;
			return cout;
		}
		//! Initialize result container.
		void init_result(SolutionType, int);
		//! Set result vector or matrix.
		void set_result(SolutionType, LoadType, int, matrix_<U>);
		//! Set result in complex domain.
		template <class T2 = COMPLEX<U>>
		void set_result(SolutionType, LoadType, int, matrix_<T2>);
		//! Get result vector or matrix.
		matrix_<U> get_result(SolutionType, LoadType, int) const;
		//! Get result in complex domain.
		template <class T2 = COMPLEX<U>>
		matrix_<T2> get_result(SolutionType, LoadType, int) const;

	private:
		DofHandler dof_mgr_;//!< Dof manager.
		bool activate_{false};//!< Status of node.
		//! DOF accumulate bind to dof manager.
		std::function<void(int*, DofType)> dof_accum_2 = std::bind(&DofHandler::accum,
			&dof_mgr_, std::placeholders::_1, std::placeholders::_2);
		vecX_<U> range_;//!< Range of result.
		matrix_<U> disp_;//!< Storage of displacement.
		matrix_<U> vel_;//!< Storage of velocity.
		matrix_<U> accel_;//!< Storage of acceleration.
		matrix_<U> stress_;//!< Storage of stress.

		cmatrix_<U> disp_cmplx_;//!< Storage of displacement in complex.
		cmatrix_<U> vel_cmplx_;//!< Storage of velocity in complex.
		cmatrix_<U> accel_cmplx_;//!< Storage of acceleration in complex.
		cmatrix_<U> stress_cmplx_;//!< Storage of stress in complex.
};

#include "node_ext.hpp"

//! Specialization.
template class Node<REAL4, REAL4>;
template class Node<REAL4, REAL8>;
template class Node<REAL8, REAL8>;
template class Node<REAL8, REAL4>;

/**
 *  \brief Coordinate transform uitiliy.
 */
template <class T = REAL4, class U = REAL8>
struct NodeFunc {
	//! Coordinate transform for 2-node element.
	static varargout_2_<U> coord_tran(const NodeBase<T>*, const NodeBase<T>*);
	//! Coordinate transform for 2-node and up direction.
	static varargout_2_<U> coord_tran(const NodeBase<T>*, const NodeBase<T>*, const T[]);
	//! Coordinate transform for 2-node and up direction.
	static varargout_2_<U> coord_tran(const NodeBase<T>*, const NodeBase<T>*, init_list_<T>);
	//! Coordinate transform for 2-node and up direction.
	static varargout_2_<U> coord_tran(
		const NodeBase<T> *p1,
		const NodeBase<T> *p2,
		T up_x,
		T up_y,
		T up_z) { return coord_tran(p1, p2, {up_x, up_y, up_z});}
	//! Coordinate transform for triangle.
	static varargout_3_<U> coord_tran(const NodeBase<T>*, const NodeBase<T>*, const NodeBase<T>*);
	//! Coordinate transform for quadrangle.
	static varargout_3_<U> coord_tran(const NodeBase<T>*, const NodeBase<T>*, const NodeBase<T>*, const NodeBase<T>*);
};

//! Specialization.
template struct NodeFunc<REAL4, REAL4>;
template struct NodeFunc<REAL4, REAL8>;
template struct NodeFunc<REAL8, REAL4>;
template struct NodeFunc<REAL8, REAL8>;
//
// /**
//  * \brief Quadrilateral Area Coordinate Method.
//  */
// template <class T=REAL8>
// struct QACM{
// 	//!
// 	static std::tuple<T, vecX_<T>, vecX_<T>, vecX_<T>> shape_2d(T, T, const matrix_<T>, int);
// };
//
// //! Specialization.
// template struct QACM<REAL4>;
// template struct QACM<REAL8>;
}  // namespace cafea
#endif  // CAFEA_NODE_H_
