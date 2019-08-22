#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "cafea/base/boundary.hpp"
#include "func_random.hpp"

using cafea::Boundary;

TEST_CASE("init", "[Boundary]") {
    Boundary<float> bf(random_value(1, 100), gen_boundary_type(0), gen_dof_label(2));
    REQUIRE(1 <= bf.get_id());
    REQUIRE(bf.get_id() <= 100);
    REQUIRE(0 == static_cast<int>(bf.get_boundary_type()));
    REQUIRE(2 == static_cast<int>(bf.get_dof_label()));
    REQUIRE(bf.get_boundary_val() == Approx(0.0f).epsilon(0.00001f));
    bf.set_boundary_type(gen_boundary_type(1));
    bf.set_dof_label(gen_dof_label(3));
    bf.set_boundary_val(random_value<float>(11.f, 31.f));

    Boundary<double> bd(random_value(101, 200), gen_boundary_type(1), gen_dof_label(3), random_value<double>(.1f, 9.f));
    REQUIRE(101 <= bd.get_id());
    REQUIRE(bd.get_id() <= 200);
    REQUIRE(1 == static_cast<int>(bd.get_boundary_type()));
    REQUIRE(3 == static_cast<int>(bd.get_dof_label()));

    REQUIRE(bf.get_id() != bd.get_id());
    REQUIRE(bf.get_boundary_val() > bd.get_boundary_val());
    REQUIRE(bf.get_boundary_type() == bd.get_boundary_type());
    REQUIRE(bf.get_dof_label() == bd.get_dof_label());
}

TEST_CASE("vector", "[Boundary]") {
    std::vector<Boundary<float>> pa;
    for (int num: {1, 100, 1000, 10000, 100000, 1000000,}) {
        for (int i = 0; i < num; i++) {
            pa.push_back(Boundary<float>(random_value(num, 10*num), gen_boundary_type(), gen_dof_label()));
        }
        for (const auto &x: pa) {
            REQUIRE(num <= x.get_id());
            REQUIRE(10*num >= x.get_id());
        }
        pa.clear();
    }
}
