#ifndef LOAD_H
#define LOAD_H

#include <cassert>
#include <vector>
#include <algorithm>
#include <iostream>

// #include <boost/variant.hpp>
// #include <variant>

#include "fmt/format.h"

#include "base.h"

namespace cafea
{
/**
 *  Load definition.
 */
template <class T=float>
class Load: public ObjectBase {
	public:
		using ObjectBase::ObjectBase;// Inherit Base's constructors.
		Load()=delete;
		/**
		 *  \brief Constructor with id load type.
		 *  \param [in] id load's id.
		 *  \param [in] lt load's type.
		 *  \param [in] ld load's domain.
		 *  \param [in] df dof label.
		 *  \param [in] val load value array.
		 *  \param [in] range load range array.
		 *  \param [in] n length of load array.
		 */
		Load(int id, LoadType lt, LoadDomain ld, DofLabel df, const T val[],
			const T range[], int n):lt_(lt), ld_(ld), df_(df),
			ObjectBase{id, fmt::format("Load#{0}", id)}
		{
			assert(n>0);
			for(int i=0; i<n; i++){
				val_.push_back(val[i]);
				range_.push_back(range[i]);
			}
		};
		//! A destructor.
		~Load() override {val_.clear(); range_.clear();};
	private:
		LoadType lt_;//!< Load type.
		LoadDomain ld_;//!< Load domain.
		DofLabel df_;//!< Dof label.
		std::vector<T> val_;//!< Load value vector.
		std::vector<T> range_;//!< Load range vector.
};

template <class T=float>
struct LoadCell
{
	int id_{-1};
	LoadType lt_;
	DofLabel df_;
	LoadDomain ld_;
	union
	{
		T val_;
		std::complex<T> val_cmplx_;
	};
	int get_id() const {return id_;};
	DofLabel get_dof_label() const {return df_;};
	
	static bool sort_by_load_type(const LoadCell<T> &a, const LoadCell<T> &b)
	{
		return a.lt_ == b.lt_;
	};
	static bool sort_by_dof_label(const LoadCell<T> &a, const LoadCell<T> &b)
	{
		return a.df_ == b.df_;
	};
	static bool sort_by_load_domain(const LoadCell<T> &a, const LoadCell<T> &b)
	{
		return a.ld_ == b.ld_;
	};
};


template struct LoadCell<REAL4>;
template struct LoadCell<REAL8>;

template <class T=float>
class LoadSet: public ObjectBase {
	public:
		using ObjectBase::ObjectBase;
		LoadSet()=delete;
		LoadSet(int id, LoadDomain ld, const T val):ld_(ld), val_(val), ObjectBase{id, fmt::format("LoadSet#{}", id)}{};
		int add_load(int, LoadType, DofLabel, const T);
		int add_load(int, LoadType, DofLabel, const T, const T);
		
		void set_value(const T a) { val_ = a;};
		T get_value() const {return val_;};
		void clear() {list_.clear();};
		std::vector<LoadCell<T>> get_load_by_type(LoadType lt=LoadType::PRES);
		~LoadSet() override {list_.clear();};
	private:
		LoadDomain ld_;
		T val_;
		std::vector<LoadCell<T>> list_;
};

template class LoadSet<REAL4>;
template class LoadSet<REAL8>;
}
#endif