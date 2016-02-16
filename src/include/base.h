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
	
template<class T>
constexpr T EPS() {return std::numeric_limits<T>::epsilon();};
	
template<class T>
constexpr T PI() {return T(std::atan(1.0)*4.0);};

class ObjectBase {
	public:
		ObjectBase(){};
		~ObjectBase(){};
		
		template <class T>
		void set_name(T val){name = std::to_string(val);};
		template <>
		void set_name<std::string>(std::string s){name = s};
		template<>
		void set_name<char>(const char *cs){name = cs;};
		
		void set_id(int x){id = x;};
		void set_group(std::initializer_list<int> abc){
			assert(abc.size()<=8);
			int i{0};
			for(auto const &it: abc)group[i++] = it;
		}
		void set_group(const int y[], int n){
			assert(n<=8);
			for(int i=0; i<n; i++)group[i] = y[i];
		};
		
		std::string get_name() const {return name;};
		char *get_name() const {return name.c_str();};
		int get_id() const {return id;};
		std::array<int, 8> get_group() const {return group;};
		
	private:
		int id{-1};
		std::array<int, 8> group{-1, -1, -1, -1, -1, -1, -1, -1};
		std::string name{"This is a basement object!"};
}

class NodeBase: public ObjectBase {
	public:
		using ObjectBase::ObjectBase;
		
		NodeBase(int x, int y, std::initializer_list<float> abc):id(x), csys(y){
			assert(abc.size()<=3);
			int i{0};
			for(auto const &it: abc)xyz[i++] = it;
		};
		NodeBase(int x, int y, std::initializer_list<int> abc):id(x), csys(y){
			assert(abc.size()<=8);
			int i{0};
			for(auto const &it: abc)group[i++] = it;
		};
		NodeBase(
			int x,
			int y,
			std::initializer_list<int> abc,
			std::initializer_list<float> rst,
		):id(x), csys(y){
			assert(abc.size()<=8);
			assert(rst.size()<=3);
			int i{0};
			for(auto const &it: abc)group[i++] = it;
			int j{0};
			for(auto const &it: rst)xyz[j++] = it;
		}
		NodeBase(int x, int y, float a, float b, float c):id(x), csys(y), xyz{a, b, c}{};
		NodeBase(int x, int y):id(x), csys(y){};
		NodeBase(int x):id(x){};
		NodeBase(){};
	
	private:
		int csys{-1};// Coordinate system.
		float xyz[3]{0.0f, 0.0f, 0.0f};
		float angle[3]{181.0f, 181.0f, 181.0f};// Euler angle in degree.
};

template <class T>
class Node: public NodeBase {
	public:
		using NodeBase::NodeBase;// Inherit Base's constructors.
		bool is_active(){return !dofs.empty();};
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
	private:
		std::vector<int> dofs;
		Eigen::MatrixXd mode_shape;
		Eigen::Matrix<T, Eigen::Dynamic, 1> pres;// Pressure.
		Eigen::Matrix<T, Eigen::Dynamic, 1> range;
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> disp;// Displacement.
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> vel;// Velocity.
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> accel;// Acceleration.
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> stress;
};

template <class T>
class Element: public ObjectBase {
	public:
		using ObjectBase::ObjectBase;
		element(
			int x,
			int y,
			int z, 
			std::initializer_list<int> abc,
			std::initializer_list<int> rst,
		):id(x), etype(y), matl(z), node_list(abc){
			assert(rst.size()<=8);
			int i{0};
			for(auto const &it: rst)group[i++] = it;
		};
		element(int x, int y, int z, std::initializer_list<int> abc):id(x), etype(y), matl(z), node_list(abc){};
		element(int x, int y, int z):id(x), etype(y), matl(z){};
		element(int x, int y):id(x), etype(y){};
		element(int x):id(x){};
		element(){};
		~element(){
			node_list.clear();
			m_rhs.resize(0);
			m_stif.resize(0, 0);
			m_mass.resize(0, 0);
			m_tran.resize(0, 0);
		};
	
	private:
		int etype{-1};// Type of element.
		int matl{-1};// Material number.
		int keyopt[8]{-1, -1, -1, -1, -1, -1, -1, -1};// Parameters of element.
		std::vector<int> node_list;
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> m_stif;
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> m_mass;
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> m_tran;
		Eigen::Matrix<T, Eigen::Dynamic, 1> m_rhs;
}
}
#endif