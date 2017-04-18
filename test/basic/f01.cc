#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <vector>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif
void cdb_reader_init();
void cdb_reader_info();
void cdb_reader_load(const char*, int);
#ifdef __cplusplus
}
#endif

TEST_CASE("demo", "[CDBReader]") {
    SECTION("case 01") {
        std::string ex01("./ansys/nrc1677_1_1.cdb");
        cdb_reader_init();
        cdb_reader_load(ex01.c_str(), ex01.size());
        cdb_reader_info();
    }
    SECTION("case 02") {
        std::string ex("./ansys/nrc1677_1_2.cdb");
        cdb_reader_init();
        cdb_reader_load(ex.c_str(), ex.size());
        cdb_reader_info();
    }
    SECTION("multi-cases") {
        std::vector<std::string> examples{"./ansys/pipe_demo_01.cdb",
            "./ansys/pipe_demo_02.cdb", "./ansys/pipe_demo_03.cdb",
            "./ansys/nrc1677_1_1.cdb", "./ansys/nrc1677_1_2.cdb",
            "./ansys/nrc1677_1_3.cdb", "./ansys/nrc1677_1_4.cdb",
            "./ansys/nrc1677_1_5.cdb", "./ansys/nrc1677_1_6.cdb",
            "./ansys/nrc1677_1_7.cdb", "./ansys/nrc1677_2_1.cdb",
            "./ansys/nrc1677_2_2.cdb", "./ansys/nrc1677_2_3.cdb",
            "./ansys/nrc1677_2_4.cdb", "./ansys/nrc6645_1_1.cdb", };
        for (const auto fn: examples) {
            cdb_reader_init();
            cdb_reader_load(fn.c_str(), fn.size());
            cdb_reader_info();
        }
    }
}
