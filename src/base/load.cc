#include <complex>

#include "load.h"

namespace cafea
{
using namespace std::complex_literals;
/**
 *
 */
template <class T>
LoadCell<T> LoadSet<T>::get_load_by_type(LoadType lt)
{
	auto cmp = [lt](const LoadCell<T> &a){return static_cast<int>(lt)==static_cast<int>(a.lt_);};
	auto it = std::find(this->list_.begin(), this->list_.end(), cmp);
	if(it!=this->list_.end()){
		auto index = std::distance(this->list_.begin(), it);
		return this->list_[index];
	}
	else{
		LoadCell<T> tmp{-999, };
		return tmp;
	}
};
/**
 *
 */
template <class T>
int LoadSet<T>::add_load(int id, LoadType load_type, DofLabel dof_label, const T value)
{
	LoadCell<T> tmp{id, load_type, dof_label, ld_};
	if(ld_==LoadDomain::FREQ){
		tmp.val_cmplx_ = std::complex<T>(value, 0.0);
	}
	else{
		tmp.val_ = value;
	}
	this->list_.push_back(std::move(tmp));
	return 0;
};
/**
 *
 */
template <class T>
int LoadSet<T>::add_load(int id, LoadType load_type, DofLabel dof_label, const T value_re, const T value_im)
{
	LoadCell<T> tmp{id, load_type, dof_label, LoadDomain::FREQ};
	tmp.val_cmplx_ = value_re + value_im*1i;
	this->list_.push_back(std::move(tmp));
	return 0;
};
}