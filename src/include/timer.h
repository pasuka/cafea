#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <ostream>

#include "fmt/format.h"

namespace cafea
{
/**
 *  Timer object.
 */
class Timer{
	public:
		//! A constructor.
		Timer(): beg_(clock_::now()) {};
		//! Rest timer.
		void reset() { beg_ = clock_::now();};
		/**
		 *  \brief Get time consuming.
		 *  \param [out] time elapsed.
		 */
		double elapsed() const
		{
			return std::chrono::duration_cast<second_>(clock_::now()-beg_).count();
		};
		//! Print time cost.
		friend std::ostream& operator<<(std::ostream& cout, const Timer &a)
		{
			return cout << fmt::format("Time cost: %7.2f seconds.\n", a.elapsed());
		};
	private:
		using clock_  = std::chrono::high_resolution_clock;
		using second_ = std::chrono::duration<double, std::ratio<1>>;
		std::chrono::time_point<clock_> beg_;//!< Begin clock.
};
}
#endif
