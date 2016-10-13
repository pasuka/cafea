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
	double freq_range[2]={-1., -1.};
};

cdb_ex_ nrc_ex[] = {
	{std::string("./ansys/pipe_demo_01.cdb"), {90.588, 100.503, 418.04, 480.193, 1241.493, 1335.526,
		2260.454, 2505.754, 2771.559, 3852.258, 3852.333, 5223.342, 5279.894, 6010.645, 6673.827},},
	{std::string("./ansys/pipe_demo_02.cdb"), {44.673, 80.632, 103.566, 193.109, 210.178, 329.211,
		336.523, 457.355, 466.377, 693.369, 745.125, 763.789, 801.945, 932.173, 947.206}, false,
		{0.0, 948.0},},
	{std::string("./ansys/pipe_demo_03.cdb"), {6.047, 6.269, 7.759, 8.922, 12.441, 12.83, 14.297,
		15.484, 16.369, 18.54, 19.496, 23.223, 24.08, 32.634, 33.749}, true,},
	{std::string("./ansys/nrc1677_1_1.cdb"), {28.534, 55.771, 81.499, 141.739, 162.816},},
	{std::string("./ansys/nrc1677_1_2.cdb"), {8.711, 8.806, 17.507, 40.366, 41.625},},
	{std::string("./ansys/nrc1677_1_3.cdb"), {9.359, 12.705, 15.376, 17.796, 21.602, 32.033, 38.067,
		40.291, 48.895},},
	{std::string("./ansys/nrc1677_1_4.cdb"), {
		6.120	,6.169	,6.526	,6.539	,6.604	,6.609	,6.712	,7.978	,10.198	,11.715	,3.371	,13.871	,14.231	,4.466	,14.685	,15.557	,17.066	,8.884	,28.155	,28.177	,29.479	,9.723	,30.266	,30.423	,0.429	,31.599	,31.612	,9.181	,40.155	,40.436	,
	},},
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
		if(p.freq_range[1]>cafea::EPS<>()){
		example->set_parameter(cafea::SolutionOption::MODAL_FREQ_RANGE, {p.freq_range[0], p.freq_range[1]});
		}
		else{
			example->set_parameter(cafea::SolutionOption::MODAL_NUMBER, {double(p.modal_freq.size()),});
		}
		example->analyze();
		example->assembly();
		example->solve();
		cafea::matrix_<double> rst = example->get_result();
		fmt::print("\nFile Name:{}\n", p.cdb_name.c_str());
		fmt::print("No.\tPresent Sovler\tReference\tError\n");
		for(int i=0; i<rst.rows(); i++){
			fmt::print("{:02d}\t{:7.3f}Hz\t{:7.3f}Hz\t", i+1, rst(i, 0), p.modal_freq[i]);
			auto err = fabs(rst(i, 0)-p.modal_freq[i])*1e2/p.modal_freq[i];
			fmt::print("{:6.2f}%\n", err);
			REQUIRE(err<=5.0);
		}
	}
}
}