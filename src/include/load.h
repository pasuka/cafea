#ifndef LOAD_H
#define LOAD_H

#include "fmt/format.h"

#include "base.h"

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
	LoadType lt_;//!< type of load.
	DofLabel df_;//!< dof label of load.
	LoadDomain ld_;//!< domain of load.
	//! Value of load.
	union
	{
		T val_;
		std::complex<T> val_cmplx_;
	};
	//! Get id of load.
	int get_id() const { return id_;};
	//! Get dof loabel.
	DofLabel get_dof_label() const { return df_;};
	//! Sort by load type.
	static bool sort_by_load_type(const LoadCell<T> &a, const LoadCell<T> &b)
	{
		return a.lt_ == b.lt_;
	};
	//! Sort by dof label.
	static bool sort_by_dof_label(const LoadCell<T> &a, const LoadCell<T> &b)
	{
		return a.df_ == b.df_;
	};
	//! Sort by load domain.
	static bool sort_by_load_domain(const LoadCell<T> &a, const LoadCell<T> &b)
	{
		return a.ld_ == b.ld_;
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
		LoadSet(int id, LoadDomain ld, const T val):ld_(ld), val_(val),
		 	ObjectBase{id, fmt::format("LoadSet#{}", id)} {};
		//! Add single valued load.
		int add_load(int, LoadType, DofLabel, const T);
		//! Add doubly valued load.
		int add_load(int, LoadType, DofLabel, const T, const T);
		//! Add complex valued load.
		int add_load(int, LoadType, DofLabel, const COMPLEX<T>);
		//! Clear load set.
		void clear() {list_.clear();};
		//! Get load subset by type.
		std::vector<LoadCell<T>> get_load_by_type(LoadType lt=LoadType::PRES);
		//! Destructor.
		~LoadSet() override { list_.clear();};
	private:
		LoadDomain ld_;//!< Load domain.
		std::vector<LoadCell<T>> list_;//!< Load set container.
};
//! Specialization of load set.
template class LoadSet<REAL4>;
template class LoadSet<REAL8>;
}
#endif
