#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <map>
#include <array>
#include <vector>
#include <string>
#include <iostream>

#include "cafea/io/mesh_reader.hpp"
#include "cafea/utils/timer.hpp"

using cafea::CDBReader;

std::map<std::string, std::map<std::string, size_t>> examples {
    {"./ansys/pipe_demo_01.cdb", {{"node", 27}, {"element", 12}, {"material", 1}, {"real_constant", 1}, {"BC", 1}}},
    {"./ansys/pipe_demo_02.cdb", {{"node", 43}, {"element", 30}, {"material", 1}, {"real_constant", 2}, {"BC", 2}}},
    {"./ansys/pipe_demo_03.cdb", {{"node", 53}, {"element", 52}, {"material", 2}, {"real_constant", 7}, {"BC", 19}}},
    {"./ansys/nrc1677_1_1.cdb", {{"node", 11}, {"element", 19}, {"material", 1}, {"real_constant", 11}, {"BC", 2}}},
    {"./ansys/nrc1677_1_2.cdb", {{"node", 18}, {"element", 32}, {"material", 1}, {"real_constant", 15}, {"BC", 4}}},
    {"./ansys/nrc1677_1_3.cdb", {{"node", 35}, {"element", 35}, {"material", 2}, {"real_constant", 6}, {"BC", 16}}},
    {"./ansys/nrc1677_1_4.cdb", {{"node", 115}, {"element", 123}, {"material", 1}, {"real_constant", 21}, {"BC", 26}}},
    {"./ansys/nrc1677_1_5.cdb", {{"node", 49}, {"element", 52}, {"material", 3}, {"real_constant", 23}, {"BC", 8}}},
    {"./ansys/nrc1677_1_6.cdb", {{"node", 68}, {"element", 88}, {"material", 1}, {"real_constant", 29}, {"BC", 19}}},
    {"./ansys/nrc1677_1_7.cdb", {{"node", 98}, {"element", 107}, {"material", 2}, {"real_constant", 24}, {"BC", 31}}},
    {"./ansys/nrc1677_2_1.cdb", {{"node", 53}, {"element", 52}, {"material", 2}, {"real_constant", 7}, {"BC", 19}}},
    {"./ansys/nrc1677_2_2.cdb", {{"node", 35}, {"element", 35}, {"material", 2}, {"real_constant", 6}, {"BC", 16}}},
    {"./ansys/nrc1677_2_3.cdb", {{"node", 81}, {"element", 85}, {"material", 3}, {"real_constant", 10}, {"BC", 22}}},
    {"./ansys/nrc1677_2_4.cdb", {{"node", 204}, {"element", 250}, {"material", 22}, {"real_constant", 83}, {"BC", 39}}},
    {"./ansys/nrc6645_1_1.cdb", {{"node", 69}, {"element", 67}, {"material", 6}, {"real_constant", 12}, {"BC", 21}}}};

TEST_CASE("demo", "[CDBReader]") {
    SECTION("multi-cases") {
        CDBReader fp;
        cafea::Timer tt;
        for (const auto &x: examples) {
            tt.reset();
            fp.load_model(x.first);
            std::cout << tt;
            auto info = fp.get_info();
            auto lib_ref = x.second;
            for (const auto &p: info) {
                std::cout << p.first << " " << p.second << "\n";
                REQUIRE(p.second == lib_ref[p.first]);
            }
            fp.clean_model();
        }
    }
}
