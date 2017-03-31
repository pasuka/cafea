#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "cafea/dof_handler.h"
#include "func_random.hpp"

using cafea::DofHandler;

TEST_CASE("init", "[dof handler]") {
    DofHandler pp;
    pp.set_num_dofs(3);
    REQUIRE(3 == pp.get_num_dofs());
}

TEST_CASE("set dof number", "[dof handler]") {
    DofHandler pp;
    pp.set_num_dofs(6);
    REQUIRE(6 == pp.get_num_dofs());
    for (auto x: {1, 2, 3, 4, 5, 6,}) {
        pp.set_num_dofs(x);
        REQUIRE(6 == pp.get_num_dofs());
    }
    for (auto x: {1, 2, 3, 4, 5, 6,}) {
        pp.clear();
        pp.set_num_dofs(x);
        REQUIRE(x == pp.get_num_dofs());
    }
}

TEST_CASE("set dof type", "[dof handler]") {
    DofHandler pp;
    pp.set_num_dofs(7);
    REQUIRE(7 == pp.get_num_dofs());
    for (int x: {10, 100, 1000, 10000, 100000, 1000000,}) {
        auto df = gen_dof_label(random_value(0, 6));
        auto dt = gen_dof_type();
        pp.set_constraint(df, dt);
        REQUIRE(dt == pp.get_constraint(df));
    }
}

TEST_CASE("accum dof number", "[dof handler]") {
    DofHandler pp;
    pp.set_num_dofs(7);
    int num{0};
    pp.accum(&num);
    auto v = pp.get_dofs();
    REQUIRE(7 == v.size());
    for (int i = 0; i < 7; i++) REQUIRE(i == v[i]);
    REQUIRE(7 == num);
    DofHandler p2;
    p2.set_num_dofs(7);
    auto df = gen_dof_label(random_value(0, 6));
    auto dt = gen_dof_type(random_value(0, 2));
    p2.set_constraint(df, dt);
    p2.accum(&num, dt);
    auto v2 = p2.get_dofs();
    REQUIRE(7 == v2.size());
    REQUIRE(num > v2[static_cast<int>(df)]);
    for (int i = 0; i < 7; i++) {
        if (i != static_cast<int>(df)) {
            REQUIRE(static_cast<int>(gen_dof_type(3)) == v2[i]);
        }
    }
    p2.accum(&num);
    auto v3 = p2.get_dofs();
    int j{6};
    for (int i = 0; i < 7; i++) {
        if (i != static_cast<int>(df)) REQUIRE((num-j--) == v3[i]);
    }
}
