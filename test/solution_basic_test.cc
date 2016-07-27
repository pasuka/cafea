#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "cafea.h"

TEST_CASE("Load cdb file", "[Solution Basic]")
{
	using cafea::SolutionBase;
	using cafea::SolutionModal;
	using cafea::AnsysCdbReader;
	
	SolutionModal<AnsysCdbReader<float>, float, double> example01;
	
	example01.load("./ansys/pipe_demo_02.cdb");
	
	auto param = example01.get_info();
	REQUIRE(param[0]==43);
	REQUIRE(param[1]==30);
	REQUIRE(param[2]==2);
	REQUIRE(param[3]==1);
	REQUIRE(param[4]==0);
	std::unique_ptr<SolutionBase<AnsysCdbReader<float>>> example02(new SolutionModal<AnsysCdbReader<float>>);
	
	example02->load(fmt::format("./ansys/pipe_demo_{:02d}.cdb", 1).c_str());
}