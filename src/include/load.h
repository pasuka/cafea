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
	//! Print info.
	friend std::ostream& operator<<(std::ostream& cout, const LoadCell &a)
	{
		return cout << fmt::format("LoadCell id:{} type:{} dof label:{} domain:{}\n",
			a.id_, static_cast<int>(a.lt_), static_cast<int>(a.df_), static_cast<int>(a.ld_));
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
		 	ObjectBase{ id, fmt::format("LoadSet#{}", id)} {};
		void add_load(LoadCell<T> tmp) { list_.push_back(tmp);};
		//! Add single valued load.
		int add_load(int, LoadType, DofLabel, T);
		//! Add doubly valued load.
		int add_load(int, LoadType, DofLabel, T, T);
		//! Add complex valued load.
		int add_load(int, LoadType, DofLabel, COMPLEX<T>);
		//! Clear load set.
		void clear() { list_.clear();};
		//! Get load subset by type.
		std::vector<LoadCell<T>> get_load_by_type(LoadType lt=LoadType::PRES);
		//! Get load subset by dof.
		std::vector<LoadCell<T>> get_load_by_dof(DofLabel df=DofLabel::UX);
		//! Get value of load set.
		T get_value() const { return val_;};
		//! Set value of load set.
		void set_value(T val_new) { val_ = val_new;};
		//! Get enum of load domain.
		LoadDomain get_load_domain() const { return ld_;};
		//! Set enum of load domain.
		void set_load_domain(LoadDomain ld_new) { ld_ = ld_new;};
		//! Get count of load cells.
		size_t get_count() const { return list_.size();};
		//! Destructor.
		~LoadSet() override { list_.clear();};
		//! Print info.
		friend std::ostream& operator<<(std::ostream& cout, const LoadSet &a)
		{
			return cout << fmt::format("LoadSet id:{} domain:{} value:{} count:{}\n",
				a.id_, a.ld_==LoadDomain::FREQ ? "Frequency":"Time", a.val_, a.get_count());
		};
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
