/*
 *  cafea --- A FEA library for dynamic analysis.
 *  Copyright (c) 2007-2017 T.Q.
 *  All rights reserved.
 *  Distributed under GPL v3 license.
 */
#ifndef CAFEA_TIMER_HPP_
#define CAFEA_TIMER_HPP_

#include <chrono>
#include <ostream>

#include "fmt/format.h"

namespace cafea {
/**
 * @brief 
 * 
 */
class Timer {
	public:
		//! A constructor.
		Timer(): beg_(clock_::now()) {}
		//! Rest timer.
		void reset() { beg_ = clock_::now();}
		/**
		 *  \brief Get time consuming.
		 *  \param [out] time elapsed.
		 */
		double elapsed() const {
			return std::chrono::duration_cast<second_>(clock_::now() - beg_).count();
		}
		//! Print time cost.
		friend std::ostream& operator<<(std::ostream& cout, const Timer &a) {
			return cout << fmt::format("Time cost: {:7.3f} seconds.\n", a.elapsed());
		}

	private:
		using clock_  = std::chrono::high_resolution_clock;
		using second_ = std::chrono::duration<double, std::ratio<1>>;
		std::chrono::time_point<clock_> beg_;//!< Begin clock.
};
}  // namespace cafea
#endif  // CAFEA_TIMER_HPP_
