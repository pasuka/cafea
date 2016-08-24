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

TEST_CASE("Modal Analysis", "[Solution Modal]")
{
	std::unique_ptr<SolutionBase<AnsysCdbReader<float>>> example(new SolutionSimple);
	example->load("./ansys/pipe_demo_01.cdb");
	example->analyze();
	example->assembly();
	example->solve();
	// example->write2mat("demo01.mat");
	
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