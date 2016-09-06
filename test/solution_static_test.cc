#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "cafea.h"

namespace
{
using cafea::SolutionBase;
using cafea::SolutionStatic;
using cafea::AnsysCdbReader;
//! Simplify.
using SolutionSimple = SolutionStatic<AnsysCdbReader<float>, float, double>;

TEST_CASE("Internal Pressure", "[Solution Static]")
{
	std::unique_ptr<SolutionBase<AnsysCdbReader<float>>> example(new SolutionSimple);
	
	example->clear();
	example->load("./ansys/pipe_demo_02.cdb");
	example->analyze();
	example->assembly();
	example->solve();
	
	example->clear();
	example->load("./ansys/pipe_demo_03.cdb");
	example->analyze();
	example->assembly();
	example->solve();
}
}