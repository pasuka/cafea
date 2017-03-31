#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "cafea/section.h"
#include "func_random.hpp"

using cafea::Section;

TEST_CASE("init 01", "[Section]") {
    Section<float> sfa(random_value(1, 100), gen_section_type(0));
    Section<float> sfb(random_value(101, 200), gen_section_type(1));
    REQUIRE(sfa.get_id() < sfb.get_id());
    REQUIRE(0 == static_cast<int>(sfa.get_sect_type()));
    REQUIRE(1 == static_cast<int>(sfb.get_sect_type()));
}
