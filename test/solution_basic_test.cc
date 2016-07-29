#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "cafea.h"

TEST_CASE("Load cdb file", "[Solution Basic]")
{
	using cafea::SolutionBase;
	using cafea::SolutionModal;
	using cafea::AnsysCdbReader;
	
	using SolutionSimple = SolutionModal<AnsysCdbReader<float>, float, double>;
	
	std::unordered_map<std::string, std::array<size_t, 5>> cdb_list{
		{"./ansys/pipe_demo_01.cdb", {27, 12, 1, 1, 0}},
		{"./ansys/pipe_demo_02.cdb", {43, 30, 2, 1, 0}},
		{"./ansys/pipe_demo_03.cdb", { 1,  1, 1, 1, 1}},
		{"./ansys/nrc1677_1_1.cdb", {,,,,}},
		{"./ansys/nrc1677_1_6.cdb", {,,,,}},
		{"./ansys/nrc1677_2_4.cdb", {,,,,}},
		{"./ansys/nrc1677_1_2.cdb", {,,,,}},
		{"./ansys/nrc1677_1_7.cdb", {,,,,}},
		{"./ansys/nrc6645_1_1.cdb", {,,,,}},
		{"./ansys/nrc1677_1_3.cdb", {,,,,}},
		{"./ansys/nrc1677_2_1.cdb", {,,,,}},
		{"./ansys/nrc1677_1_4.cdb", {,,,,}},
		{"./ansys/nrc1677_2_2.cdb", {,,,,}},
		{"./ansys/nrc1677_1_5.cdb", {,,,,}},
		{"./ansys/nrc1677_2_3.cdb", {,,,,}},};
	SolutionSimple example01;
	
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