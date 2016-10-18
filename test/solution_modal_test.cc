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

//! Case struct.
struct cdb_ex_ {
	std::string cdb_name;//!< File path.
	std::vector<double> modal_freq;//!< Result of modal frequency.
	bool is_lumped{false};//!< Lumped mass.
	double freq_range[2]={-1., -1.};//!< Range of frequency.
	bool is_pres{false};//!< Internal pressure.
	double pres{350.0};//!< Value of internal pressure.
};

//! Passed cases.
cdb_ex_ nrc_ex[] = {
	{std::string("./ansys/pipe_demo_01.cdb"), {90.588, 100.503, 418.04, 480.193, 1241.493, 1335.526,
		2260.454, 2505.754, 2771.559, 3852.258, 3852.333, 5223.342, 5279.894, 6010.645, 6673.827},},
	{std::string("./ansys/pipe_demo_02.cdb"), {44.673, 80.632, 103.566, 193.109, 210.178, 329.211,
		336.523, 457.355, 466.377, 693.369, 745.125, 763.789, 801.945, 932.173, 947.206}, false,
		{0.0, 948.0},},
	{std::string("./ansys/pipe_demo_03.cdb"), {6.047, 6.269, 7.759, 8.922, 12.441, 12.83, 14.297,
		15.484, 16.369, 18.54, 19.496, 23.223, 24.08, 32.634, 33.749}, true, {0.0, 33.8}},
	{std::string("./ansys/nrc1677_1_1.cdb"), {28.534, 55.771, 81.499, 141.739, 162.816},},
	{std::string("./ansys/nrc1677_1_2.cdb"), {8.711, 8.806, 17.507, 40.366, 41.625},},
	{std::string("./ansys/nrc1677_2_1.cdb"), {6.0476, 6.2692, 7.7593, 8.9227, 12.4419, 12.8300,
		14.2974, 15.4842, 16.3691, 18.5402, 19.4966, 23.2237, 24.0804, 32.6346, 33.7491}},
	{std::string("./ansys/nrc6645_1_1.cdb"), {2.906, 4.383, 5.515, 5.701, 6.978, 7.342, 7.877,
		10.396, 11.062, 11.232, 11.532, 12.455, 13.964, 16.092},},
};

//! Not passed yet...
cdb_ex_ nrc_ex_2[] = {
	{std::string("./ansys/nrc1677_1_3.cdb"), {9.359, 12.705, 15.376, 17.796, 21.602, 25.097, 32.033,
		38.067, 40.291, 48.895}, true, {-1., -1.}, true, 350.0},
	{std::string("./ansys/nrc1677_1_4.cdb"), {6.12, 6.169, 6.526, 6.539, 6.604, 6.609, 6.712, 7.978,
		10.198, 11.715, 13.371, 13.871, 14.231, 14.466, 14.685, 15.557, 17.066, 18.884, 28.155,
		28.177, 29.479, 29.723, 30.266, 30.423, 30.429, 31.599, 31.612, 39.181, 40.155, 40.436, },
		false, {-1., -1.}, false, 2.4e3},
	{std::string("./ansys/nrc1677_1_5.cdb"), {4.035, 4.257, 9.115, 11.187, 17.106, 18.171, 22.375,
		27.193, 28.011, 37.976, 40.968},},
	{std::string("./ansys/nrc1677_1_6.cdb"), {6.391, 9.993, 13.274, 14.484, 15.327, 17.499, 19.090,
		19.623, 21.436, 28.707, 29.867, 31.484, 32.009, 36.365, 40.980, 41.367, 47.391, 49.765,
		50.123, 52.928, 56.898, 58.506, 67.464, 70.457, 75.405, 79.179, 80.738, 86.099}, },
	{std::string("./ansys/nrc1677_1_7.cdb"), {5.033, 7.812, 8.192, 8.977, 9.312, 9.895, 13.221,
		14.956, 15.066, 17.754, 18.208, 22.899, 25.022, 25.854, 26.941, 28.131, 30.297, 35.218,
		37.095, 42.612, 44.415, 48.086}},
	
	{std::string("./ansys/nrc1677_2_2.cdb"), {9.1655, 12.6533, 15.1847, 17.4952, 21.2461, 24.7136,
		31.771, 37.7442, 39.9244, 48.2221, 57.0146, 61.0477, 62.0268, 68.4341, 76.179, 77.7516,
		99.622, 101.6221, 106.1587, 112.7788, 132.6403, 153.8602, 158.8719, 200.4281, 206.9838}, true},
	{std::string("./ansys/nrc1677_2_3.cdb"), {7.2431, 10.1497, 14.6066, 16.0215, 17.177, 17.9922,
		22.274, 23.6365, 27.8631, 29.207, 29.4711, 31.5635, 34.0245, 34.7638, 35.1169},true},
	{std::string("./ansys/nrc1677_2_4.cdb"), {2.61, 2.914, 4.336, 4.66, 5.723, 5.832, 7.358, 7.768,
		9.956, 10.327, 10.676, 10.945, 12.021, 12.298, 13.250, 13.404, 14.426, 14.717, 15.252,
		15.549, 16.166, 16.803, 17.230, 17.273, 17.453, 18.702, 18.896, 19.982, 21.455, 21.522,
		22.733, 23.298, 24.064, 24.595, 25.105, 26.513, 26.943, 27.503, 28.659, 29.537, 30.603,
		31.261, 32.274, 35.465, 36.042, 36.343, 36.736, 37.992, 38.328, 40.173}, true},
	
};

TEST_CASE("CDB examples", "[Solution Modal]")
{
	std::unique_ptr<SolutionBase<AnsysCdbReader<float>>> example(new SolutionSimple);
	for(auto const &p: nrc_ex){
		fmt::print("***************************************************************************\n");
		fmt::print("Begin Load test.\n");
		example->init();
		example->load(p.cdb_name.c_str());
		if(p.is_lumped)example->set_mass_lumped(true);
		if(p.freq_range[1]>cafea::EPS<>()){
		example->set_parameter(cafea::SolutionOption::MODAL_FREQ_RANGE, {p.freq_range[0], p.freq_range[1]});
		}
		else{
			example->set_parameter(cafea::SolutionOption::MODAL_NUMBER, {double(p.modal_freq.size()),});
		}
		if(p.is_pres)example->set_parameter(cafea::SolutionOption::PRESSURE_INTERNAL, {p.pres,});
		example->analyze();
		example->assembly();
		example->solve();
		cafea::matrix_<double> rst = example->get_result();
		fmt::print("\nFile Name:{}\n\n", p.cdb_name.c_str());
		fmt::print("***************************************************************************\n");
		fmt::print("\tNo.\t  Present  \t  Reference\t  Error\n");
		for(int i=0; i<rst.rows(); i++){
			fmt::print("\t{:02d}\t{:7.3f}Hz\t{:7.3f}Hz\t", i+1, rst(i, 0), p.modal_freq[i]);
			auto err = fabs(rst(i, 0)-p.modal_freq[i])*1e2/p.modal_freq[i];
			fmt::print("{:6.2f}%\n", err);
			REQUIRE(err<=1.0);
		}
		fmt::print("***************************************************************************\n");
	}
}
}