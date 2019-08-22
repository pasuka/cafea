#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "cafea/element/element.hpp"
#include "func_random.hpp"

using cafea::Element;

TEST_CASE("init", "[Element]") {
    SECTION("init 01") {
        Element<double> pp(11, 1, 1);
        REQUIRE(11 == pp.get_id());
    }
}
