/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#include "cafea/load.h"

namespace cafea {
/**
 *  \brief Get load by type.
 *  \param[in] lt load type.
 *  \return vector of load by selected load type.
 */
template <class T>
std::vector<LoadCell<T>> LoadSet<T>::get_load_by_type(LoadType lt) {
	std::vector<LoadCell<T>> tmp;
	if (this->list_.size() < 512) {
		for (auto &x: this->list_) { if (x.lt_ == lt) {tmp.push_back(x);}}
	} else {
		std::sort(this->list_.begin(), this->list_.end(),
			[] (auto a, auto b) { return a.lt_ < b.lt_;});
		// find searches for an element equal to value.
		// find_if searches for an element for which predicate p returns true.
		// for compare function or lambda expression please choose find_if.
		auto p1 = std::find_if(this->list_.begin(), this->list_.end(),
			[lt] (const auto &a) { return lt == a.lt_;});
		if (p1 != this->list_.end()) {
			auto p2 = std::find_if(this->list_.rbegin(), this->list_.rend(),
				[lt] (const auto &a) { return a.lt_ == lt;});
			int n_start = std::distance(this->list_.begin(), p1);
			int n_stop = -std::distance(this->list_.rend(), p2);
			// fmt::print("Index from {} to {}. ", n_start, n_stop);
			std::copy(this->list_.begin()+n_start, this->list_.begin()+n_stop, std::back_inserter(tmp));
			// alternatively, resize vector with new value.
			// tmp.resize(1+n_stop-n_start, this->list_[0]);
			// std::copy(this->list_.begin()+n_start, this->list_.begin()+n_stop, tmp.begin());
		} else {
			// fmt::print("Find nothing\n");
		}
	}
	// fmt::print("Total find :{} cells.\n", tmp.size());
	return std::move(tmp);
}
/**
 *  \brief Get load by dof.
 *  \param[in] df dof label.
 *  \return vector of load by selected dof label.
 */
template <class T>
std::vector<LoadCell<T>> LoadSet<T>::get_load_by_dof(DofLabel df) {
	std::vector<LoadCell<T>> tmp;
	if (this->list_.size() < 512) {
		for (auto &x: this->list_) { if (x.df_ == df) { tmp.push_back(x);}}
	} else {
		std::sort(this->list_.begin(), this->list_.end(),
			[] (auto a, auto b) { return a.df_ < b.df_;});
		auto p1 = std::find_if(this->list_.begin(), this->list_.end(),
			[df] (const auto &a) {return df == a.df_;});
		if (p1 != this->list_.end()) {
			auto p2 = std::find_if(this->list_.rbegin(), this->list_.rend(),
				[df] (const auto &a) { return a.df_ == df;});
			int n_start = std::distance(this->list_.begin(), p1);
			int n_stop = -std::distance(this->list_.rend(), p2);
			std::copy(this->list_.begin()+n_start, this->list_.begin()+n_stop, std::back_inserter(tmp));
		}
	}
	return std::move(tmp);
}
/**
 * \brief Add load cell.
 * \param[in] id        load cell id.
 * \param[in] load_type load type.
 * \param[in] dof_label load apply on dofs.
 * \param[in] value     values of load.
 * \return 0 means okay.
 */
template <class T>
int LoadSet<T>::add_load(int id, LoadType load_type, DofLabel dof_label, T value) {
	LoadCell<T> tmp {id, load_type, dof_label, ld_};
	if (ld_ == LoadDomain::FREQ) {
		tmp.val_cmplx_ = COMPLEX<T>(value, 0.0);
	} else {
		tmp.val_ = value;
	}
	this->list_.push_back(std::move(tmp));
	return 0;
}
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
int LoadSet<T>::add_load(int id, LoadType load_type, DofLabel dof_label, T value_re, T value_im) {
	LoadCell<T> tmp {id, load_type, dof_label, LoadDomain::FREQ};
	tmp.val_cmplx_ = value_re + value_im*1i;
	this->list_.push_back(std::move(tmp));
	return 0;
}
/**
 * \brief Add load cell.
 * \param[in] id        load cell id.
 * \param[in] load_type load type.
 * \param[in] dof_label label of dofs that apply.
 * \param[in] value     complex load value.
 * \return 0 means okay.
 */
template <class T>
int LoadSet<T>::add_load(int id, LoadType load_type, DofLabel dof_label, COMPLEX<T> value) {
	LoadCell<T> tmp {id, load_type, dof_label, LoadDomain::FREQ};
	tmp.val_cmplx_ = value;
	this->list_.push_back(std::move(tmp));
	return 0;
}
}  // namespace cafea
