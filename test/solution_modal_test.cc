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

struct cdb_ex_ {
	std::string cdb_name;
	std::vector<double> modal_freq;
	bool is_lumped{false};
};

cdb_ex_ nrc_ex[] = {
	{std::string("./ansys/pipe_demo_01.cdb"), {90.588, 100.503, 418.04, 480.193, 1241.493, 1335.526,
		2260.454, 2505.754, 2771.559, 3852.258, 3852.333, 5223.342, 5279.894, 6010.645, 6673.827},
		false,},
	{std::string("./ansys/pipe_demo_02.cdb"), {44.673, 80.632, 103.566, 193.109, 210.178, 329.211,
		336.523, 457.355, 466.377, 693.369, 745.125, 763.789, 801.945, 932.173, 947.206}, false,},
	{std::string("./ansys/pipe_demo_03.cdb"), {6.047, 6.269, 7.759, 8.922, 12.441, 12.83, 14.297,
		15.484, 16.369, 18.54, 19.496, 23.223, 24.08, 32.634, 33.749}, true,},
};


TEST_CASE("Modal Analysis", "[Solution Modal]")
{
	std::unique_ptr<SolutionBase<AnsysCdbReader<float>>> example(new SolutionSimple);
	example->init();
	example->load("./ansys/pipe_demo_01.cdb");
	example->analyze();
	example->assembly();
	example->solve();
	example->write2mat("demo01.mat");
	
	example->init();
	example->load("./ansys/pipe_demo_02.cdb");
	example->analyze();
	example->assembly();
	example->solve();
	
	example->init();
	example->load("./ansys/pipe_demo_03.cdb");
	example->set_mass_lumped();
	example->analyze();
	example->assembly();
	example->solve();
	
}

TEST_CASE("CDB examples", "[Solution Modal]")
{
	std::unique_ptr<SolutionBase<AnsysCdbReader<float>>> example(new SolutionSimple);
	for(auto const &p: nrc_ex){
		example->init();
		example->load(p.cdb_name.c_str());
		if(p.is_lumped)example->set_mass_lumped(true);
		example->analyze();
		example->assembly();
		example->solve();
	}
}
}