#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "material.h"
#include "func_random.hpp"

using cafea::Material;

TEST_CASE("init 01", "[Material]")
{
    Material<float> mf(random_value(1, 10), gen_material_type(0));
    REQUIRE(1<=mf.get_id());
    REQUIRE(10>=mf.get_id());
    REQUIRE(0==static_cast<int>(mf.get_material_type()));
    mf.set_material_type(gen_material_type(1));
    REQUIRE(1==static_cast<int>(mf.get_material_type()));

    Material<double> md(random_value(11, 20), gen_material_type(1));
    REQUIRE(11<=md.get_id());
    REQUIRE(20>=md.get_id());
    REQUIRE(1==static_cast<int>(md.get_material_type()));
    md.set_material_type(gen_material_type(0));
    REQUIRE(0==static_cast<int>(md.get_material_type()));
}

TEST_CASE("init 02", "[Material]")
{
    Material<float> mfa(random_value(1, 10), gen_material_type(0),
        {7.8e3f, 2.06e11f, 0.94e10f, .35f, 0.01f, .1e-2f, .2e-3f,});

    REQUIRE(mfa.get_material_prop(gen_material_prop(0))==Approx(7.8e3f).epsilon(.1e-3f));
    for(int x: {1, 2, 3, 4})REQUIRE(mfa.get_material_prop(gen_material_prop(x))==Approx(2.06e11f).epsilon(.1e-3f));
    for(int x: {5, 6, 7})REQUIRE(mfa.get_material_prop(gen_material_prop(x))==Approx(.94e10f).epsilon(.1e-3f));
    for(int x: {8, 9, 10, 11, 12, 13, 14})REQUIRE(mfa.get_material_prop(gen_material_prop(x))==Approx(.35f).epsilon(.1e-3f));
    REQUIRE(mfa.get_material_prop(gen_material_prop(15))==Approx(.01f).epsilon(.1e-6f));
    REQUIRE(mfa.get_material_prop(gen_material_prop(16))==Approx(.001f).epsilon(.1e-6f));
    REQUIRE(mfa.get_material_prop(gen_material_prop(17))==Approx(2.e-4f).epsilon(.1e-6f));
    for(int i=0; i<18; i++){
        auto val = random_value<float>(0.f, 1.e20f);
        mfa.set_material_prop(gen_material_prop(i), val);
        REQUIRE(mfa.get_material_prop(gen_material_prop(i))==Approx(val).epsilon(.1e-6f));
    }
    REQUIRE(mfa.get_material_prop_vec().empty());

    Material<float> mfb(random_value(1, 10), gen_material_type(1),
        {1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 1.e1f,});
    auto spring = mfb.get_material_prop_vec();
    REQUIRE(1<=spring.size());
    Material<float> mfc(random_value(1, 10), gen_material_type(2), {2.3f, .3f,});
    auto mass = mfc.get_material_prop_vec();
    REQUIRE(1<=mass.size());
}
