#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "cafea.h"

namespace
{
using cafea::SolutionBase;
using cafea::SolutionStatic;
using cafea::AnsysCdbReader;
using cafea::BcyReader;
//! Simplify.
// using SolutionSimple = SolutionStatic<AnsysCdbReader<float>, float, double>;
using SolutionSimple = SolutionStatic<BcyReader<float>, float, double>;

TEST_CASE("Internal Pressure", "[Solution Static]")
{
	std::unique_ptr<SolutionBase<BcyReader<float>>> example(new SolutionSimple);
	
	example->clear();
	example->load("./bcy_data/nrc_ex02.bcy");
}
}