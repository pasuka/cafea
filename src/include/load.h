#ifndef LOAD_H
#define LOAD_H

#include <cassert>
#include <vector>
#include <iostream>

#include "fmt/format.h"

#include "base.h"

namespace cafea
{
/**
 *  Load definition.
 */
template <class T=float, class U=float>
class Load: public ObjectBase {
	public:
		using ObjectBase::ObjectBase;// Inherit Base's constructors.
		Load(){};
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
			const U range[], int n):lt_(lt), ld_(ld), df_(df),
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
		std::vector<U> range_;//!< Load range vector.
};
}
#endif