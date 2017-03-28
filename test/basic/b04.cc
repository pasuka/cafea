#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "mesh_reader.h"
#include "func_random.hpp"

using cafea::BCYReader;

TEST_CASE("init", "[BCYReader]")
{
    SECTION("init 01")
    {
        BCYReader file_parser;
        file_parser.load_model("./bcy_data/nrc_ex02.bcy");
        std::cout << file_parser;
    }
}
