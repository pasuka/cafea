#ifndef SOLUTION_CONFIG_H
#define SOLUTION_CONFIG_H

#include "enum_lib.h"

namespace cafea
{
template <SolutionType T=SolutionType::STATIC>
class SolutionConfig {
	public:
		SolutionConfig(){};
		~SolutionConfig(){};
		
	private:
		bool eliminate_contraints
		bool lump_mass_{false};
};
}
#endif