#ifndef LOAD_H
#define LOAD_H

#include "base.h"
#include "enum_lib.h"

namespace cafea
{
/**
 *  \struct Load cell.
 *  \brief Cell of Load.
 */
template <class T=REAL4>
struct LoadCell
{
	int id_{-1};//!< id of load.
	LoadType lt_{LoadType::UNKNOWN};//!< type of load.
	DofLabel df_{DofLabel::UNKNOWN};//!< dof label of load.
	LoadDomain ld_{LoadDomain::TIME};//!< domain of load.
	//! Value of load.
	union
	{
		T val_;
		COMPLEX<T> val_cmplx_;
	};
	//! Sort by load type.
	static bool sort_by_load_type(const LoadCell<T> &a, const LoadCell<T> &b)
	{
		return a.id_ < b.id_ && static_cast<int>(a.lt_) < static_cast<int>(b.lt_);
	};
	//! Sort by dof label.
	static bool sort_by_dof_label(const LoadCell<T> &a, const LoadCell<T> &b)
	{
		return a.id_ < b.id_ && static_cast<int>(a.df_) < static_cast<int>(b.df_);
	};
	//! Sort by load domain.
	static bool sort_by_load_domain(const LoadCell<T> &a, const LoadCell<T> &b)
	{
		return a.id_ < b.id_ && static_cast<int>(a.ld_) < static_cast<int>(b.ld_);
	};
};
//! Specialization of load cell.
template struct LoadCell<REAL4>;
template struct LoadCell<REAL8>;

/**
 *  \brief Set of loads.
 */
template <class T=REAL4>
class LoadSet: public ObjectBase {
	public:
		using ObjectBase::ObjectBase;
		LoadSet()=delete;//!< Forbidden to build empty load set.
		//! Constructor of load set.
		LoadSet(int id, LoadDomain ld, T val):ld_(ld), val_(val),
		 	ObjectBase{id, fmt::format("LoadSet#{}", id)} {};
		//! Add single valued load.
		int add_load(int, LoadType, DofLabel, T);
		//! Add doubly valued load.
		int add_load(int, LoadType, DofLabel, T, T);
		//! Add complex valued load.
		int add_load(int, LoadType, DofLabel, COMPLEX<T>);
		//! Clear load set.
		void clear() {list_.clear();};
		//! Get load subset by type.
		std::vector<LoadCell<T>> get_load_by_type(LoadType lt=LoadType::PRES);
		//! Get value of load set.
		T get_value() const { return val_;};
		//! Set value of load set.
		void set_value(T val_new) { val_ = val_new;};
		//! Destructor.
		~LoadSet() override { list_.clear();};
	private:
		LoadDomain ld_;//!< Load domain.
		T val_;//!< Load set index value.
		std::vector<LoadCell<T>> list_;//!< Load set container.
};
//! Specialization of load set.
template class LoadSet<REAL4>;
template class LoadSet<REAL8>;
}
#endif
