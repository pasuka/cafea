#include "load.h"

namespace cafea
{
/**
 *  \brief Get load by type.
 *  \param[in] lt load type.
 *  \return vector of load by selected load type.
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
 * \brief Add load cell.
 * \param[in] id        load cell id.
 * \param[in] load_type load type.
 * \param[in] dof_label load apply on dofs.
 * \param[in] value     values of load.
 * \return 0 means okay.
 */
template <class T>
int LoadSet<T>::add_load(int id, LoadType load_type, DofLabel dof_label, T value)
{
	LoadCell<T> tmp{id, load_type, dof_label, ld_};
	if(ld_==LoadDomain::FREQ){
		tmp.val_cmplx_ = COMPLEX<T>(value, 0.0);
	}
	else{
		tmp.val_ = value;
	}
	this->list_.push_back(std::move(tmp));
	return 0;
};
/**
 * \brief Add load cell.
 * \param[in] id        load cell id.
 * \param[in] load_type load type.
 * \param[in] dof_label label of dofs that apply.
 * \param[in] value_re  real part or 1st value of load.
 * \param[in] value_im  imag part or 2nd value of load.
 * \return 0 means okay.
 */
template <class T>
int LoadSet<T>::add_load(int id, LoadType load_type, DofLabel dof_label, T value_re, T value_im)
{
	LoadCell<T> tmp{id, load_type, dof_label, LoadDomain::FREQ};
	tmp.val_cmplx_ = value_re + value_im*1i;
	this->list_.push_back(std::move(tmp));
	return 0;
};
/**
 * \brief Add load cell.
 * \param[in] id        load cell id.
 * \param[in] load_type load type.
 * \param[in] dof_label label of dofs that apply.
 * \param[in] value     complex load value.
 * \return 0 means okay.
 */
template <class T>
int LoadSet<T>::add_load(int id, LoadType load_type, DofLabel dof_label, COMPLEX<T> value)
{
	LoadCell<T> tmp{id, load_type, dof_label, LoadDomain::FREQ};
	tmp.val_cmplx_ = value;
	this->list_.push_back(std::move(tmp));
	return 0;
};
}
