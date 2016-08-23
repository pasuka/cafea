#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "cafea.h"

namespace
{
using cafea::SolutionBase;
using cafea::SolutionModal;
using cafea::AnsysCdbReader;
//! Simplify.
using SolutionSimple = SolutionModal<AnsysCdbReader<float>, float, double>;
using Solution = SolutionBase<AnsysCdbReader<float>>;

TEST_CASE("Modal Analysis", "[Solution Modal]")
{
	std::unique_ptr<Solution> example(new SolutionSimple);
	example->load("./ansys/pipe_demo_01.cdb");
	example->analyze();
	example->assembly();
	example->solve();
	example->write2mat("demo01.mat");
}
}