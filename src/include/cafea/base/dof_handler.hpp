/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#ifndef CAFEA_DOF_HANDLER_HPP_
#define CAFEA_DOF_HANDLER_HPP_

#include <vector>

#include "cafea/base/base.hpp"
#include "cafea/base/enum_lib.hpp"

namespace cafea {
/**
 *  \class Dof handler.
 *  DOF handler
 */
class DofHandler {
	public:
		//! Get number of DOF.
		size_t get_num_dofs() const { return dofs_.size();}
		//! Set number of DOF.
		void set_num_dofs(size_t val);
		//! Clear.
		void clear() { if (!dofs_.empty()) dofs_.clear();}
		//! Get DOF vector.
		std::vector<int> get_dofs() const { return dofs_;}
		//! Apply constraint.
		void set_constraint(DofLabel, DofType);
		//! Accumulate.
		void accum(int *ij, DofType mt = DofType::NORMAL);
		//! Get constraint.
		DofType get_constraint(DofLabel) const;
		//! Print information.
		friend std::ostream& operator<<(std::ostream& cout, const DofHandler &a) {
			for (const auto &it: a.dofs_) cout << it << " ";
			return cout << "\n";
		}

	private:
		std::vector<int> dofs_;//!< Storage of dofs.
};
}  // namespace cafea
#endif  // CAFEA_DOF_HANDLER_HPP_
