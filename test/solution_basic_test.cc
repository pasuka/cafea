#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "cafea.h"

namespace cdb_case_
{
/**
 *  \brief Test case of CDB files.
 *  key: file name.
 *  value: array of number of node/element/material/section/boundary.
 */
std::unordered_map<std::string, std::array<size_t, 5>> cdb_list{
	{"./ansys/pipe_demo_01.cdb", {27,  12,  1,  1,   6}},
	{"./ansys/pipe_demo_02.cdb", {43,  30,  1,  2,  12}},
	{"./ansys/pipe_demo_03.cdb", {53,  52,  2,  7, 108}},
	{"./ansys/nrc1677_1_1.cdb", { 11,  19,  1, 11,  12}},
	{"./ansys/nrc1677_1_2.cdb", { 18,  32,  1, 15,  24}},
	{"./ansys/nrc1677_1_3.cdb", { 35,  35,  2,  6,  90}},
	{"./ansys/nrc1677_1_4.cdb", {115, 123,  1, 21, 156}},
	{"./ansys/nrc1677_1_5.cdb", { 49,  52,  3, 23,  48}},
	{"./ansys/nrc1677_1_6.cdb", { 68,  88,  1, 29, 114}},
	{"./ansys/nrc1677_1_7.cdb", { 98, 107,  2, 24, 186}},
	{"./ansys/nrc1677_2_1.cdb", { 53,  52,  2,  7, 108}},
	{"./ansys/nrc1677_2_2.cdb", { 35,  35,  2,  6,  90}},
	{"./ansys/nrc1677_2_3.cdb", { 81,  85,  3, 10, 132}},
	{"./ansys/nrc1677_2_4.cdb", {204, 250, 22, 82, 225}},
	{"./ansys/nrc6645_1_1.cdb", { 69,  67,  6, 10, 126}},};
}

namespace
{
using cafea::SolutionBase;
using cafea::SolutionModal;
using cafea::AnsysCdbReader;
//! Simplify.
using SolutionSimple = SolutionModal<AnsysCdbReader<float>, float, double>;
using Solution = SolutionBase<AnsysCdbReader<float>>;
	
using cdb_case_::cdb_list;

TEST_CASE("Load cdb file", "[Solution Basic]")
{
	for(const auto &it: cdb_list){
		std::unique_ptr<Solution> example(new SolutionSimple);
		example->load(it.first.c_str());
		auto param = example->get_info();
		fmt::print("*******************************************************\n");
		fmt::print("File Name: {}\n", it.first.c_str());
		fmt::print("Num node: {} vs {}\n", param[0], it.second[0]);
		fmt::print("Num element: {} vs {}\n", param[1], it.second[1]);
		fmt::print("Num material: {} vs {}\n", param[2], it.second[2]);
		fmt::print("Num section: {} vs {}\n", param[3], it.second[3]);
		fmt::print("Num boundary: {} vs {}\n", param[4], it.second[4]);
		
		REQUIRE(param[0] == it.second[0]);//!< Compare node number.
		REQUIRE(param[1] == it.second[1]);//!< Compare element number.
		//! Note: cdb file may define extra material and real constants.
		//! current fortran90 wrapper do not eliminate unused material and real constants.
		REQUIRE(param[2] >= it.second[2]);//!< Compare material number.
		REQUIRE(param[3] >= it.second[3]);//!< Compare section number.
		REQUIRE(param[4] == it.second[4]);//!< Compare boundary number.
	}
}

TEST_CASE("Input model check", "[Solution Basic]")
{	
	for(const auto &it: cdb_list){
		std::unique_ptr<Solution> example(new SolutionSimple);
		example->load(it.first.c_str());
		fmt::print("*******************************************************\n");
		fmt::print("File: {}\t", it.first.c_str());
		example->check();
	}
}

TEST_CASE("Assembly global matrix", "[Solution Basic]")
{
	for(const auto &it: cdb_list){
		std::unique_ptr<Solution> example(new SolutionSimple);
		example->load(it.first.c_str());
		example->analyze();
	}
}
}