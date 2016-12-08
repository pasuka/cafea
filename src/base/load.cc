#include <complex>
#include <algorithm>

#include "load.h"

namespace cafea
{
using namespace std::complex_literals;
/**
 *
 */
template <class T>
std::vector<LoadCell<T>> LoadSet<T>::get_load_by_type(LoadType lt)
{
	// auto func = std::bind(LoadCell<T>::sort_by_load_type, std::placeholders::_1, std::placeholders::_2);
	// std::sort(this->list_.begin(), this->list_.end(), func);
	std::vector<LoadCell<T>> tmp;
	for(auto &x: this->list_){
		if(x.lt_==lt)tmp.push_back(x);
	}
	return std::move(tmp);
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