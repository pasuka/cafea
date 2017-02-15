#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "node.h"
#include "func_random.hpp"

using cafea::CoordinateSystem;
using cafea::NodeBase;
using cafea::Node;
using cafea::NodeFunc;

TEST_CASE("init", "[NodeBase]")
{
    SECTION("init with id")
    {
        NodeBase<float> pt(53);
        REQUIRE(53==pt.get_id());
        REQUIRE(CoordinateSystem::CARTESIAN==pt.get_csys());
    }
    SECTION("init with id and coordinate system")
    {
        NodeBase<float> pt(53, CoordinateSystem::CYLINDRICAL);
        REQUIRE(CoordinateSystem::CYLINDRICAL==pt.get_csys());
        pt.set_csys(CoordinateSystem::SPHERICAL);
        REQUIRE(CoordinateSystem::SPHERICAL==pt.get_csys());
        pt.set_csys(CoordinateSystem::CARTESIAN);
        REQUIRE(CoordinateSystem::CARTESIAN==pt.get_csys());
    }
    SECTION("init with id and x y z values")
    {
        NodeBase<float> p2(47, 1.f, 2.f, 3.f);
        REQUIRE(47==p2.get_id());
        REQUIRE(CoordinateSystem::CARTESIAN==p2.get_csys());
        auto xyz = p2.get_xyz();
        for(int x: {1, 2, 3})REQUIRE(xyz[x-1]==Approx(float(x)).epsilon(.0001f));
    }
    SECTION("init with id and xyz values")
    {
        NodeBase<float> p2(47, {1.f, 2.f, 3.f});
        REQUIRE(47==p2.get_id());
        REQUIRE(CoordinateSystem::CARTESIAN==p2.get_csys());
        auto xyz = p2.get_xyz();
        for(int x: {1, 2, 3})REQUIRE(xyz[x-1]==Approx(float(x)).epsilon(.0001f));
    }
    SECTION("init with id and xyz and coordinate system")
    {
        NodeBase<float> pt(61, CoordinateSystem::SPHERICAL, 1.1f, 2.3f, 3.1f);
        REQUIRE(61==pt.get_id());
        REQUIRE(CoordinateSystem::SPHERICAL==pt.get_csys());
        pt.set_csys(CoordinateSystem::CYLINDRICAL);
        REQUIRE(CoordinateSystem::CYLINDRICAL==pt.get_csys());
        REQUIRE(pt.get_x()==Approx(1.1f).epsilon(0.0001f));
        REQUIRE(pt.get_y()==Approx(2.3f).epsilon(0.0001f));
        REQUIRE(pt.get_z()==Approx(3.1f).epsilon(0.0001f));
        auto rot = pt.get_angle();
        REQUIRE(rot[0]==Approx(.0f).epsilon(0.0001f));
        REQUIRE(rot[1]==Approx(.0f).epsilon(0.0001f));
        REQUIRE(rot[2]==Approx(.0f).epsilon(0.0001f));
        for(int x: {0, 1, 2,})REQUIRE(pt.get_rot(x)==Approx(0.f).epsilon(.0001f));
    }
    SECTION("init with id and x y z and coordinate system")
    {
        NodeBase<float> pt(61, CoordinateSystem::SPHERICAL, {1.1f, 2.3f, 3.1f});
        REQUIRE(61==pt.get_id());
        REQUIRE(CoordinateSystem::SPHERICAL==pt.get_csys());
        pt.set_csys(CoordinateSystem::CYLINDRICAL);
        REQUIRE(CoordinateSystem::CYLINDRICAL==pt.get_csys());
        REQUIRE(pt.get_x()==Approx(1.1f).epsilon(0.0001f));
        REQUIRE(pt.get_y()==Approx(2.3f).epsilon(0.0001f));
        REQUIRE(pt.get_z()==Approx(3.1f).epsilon(0.0001f));
        auto rot = pt.get_angle();
        REQUIRE(rot[0]==Approx(.0f).epsilon(0.0001f));
        REQUIRE(rot[1]==Approx(.0f).epsilon(0.0001f));
        REQUIRE(rot[2]==Approx(.0f).epsilon(0.0001f));
        for(int x: {0, 1, 2,})REQUIRE(pt.get_rot(x)==Approx(0.f).epsilon(.0001f));
    }
    SECTION("init with id and xyz rot values")
    {
        NodeBase<float> pt(59, {1.f, 2.f, 3.f}, {1.2f, 2.3f, 3.4f});
        REQUIRE(59==pt.get_id());
        REQUIRE(CoordinateSystem::CARTESIAN==pt.get_csys());
        REQUIRE(pt.get_x()==Approx(1.f).epsilon(0.0001f));
        REQUIRE(pt.get_y()==Approx(2.f).epsilon(0.0001f));
        REQUIRE(pt.get_z()==Approx(3.f).epsilon(0.0001f));
        auto rot = pt.get_angle();
        REQUIRE(rot[0]==Approx(1.2f).epsilon(0.0001f));
        REQUIRE(rot[1]==Approx(2.3f).epsilon(0.0001f));
        REQUIRE(rot[2]==Approx(3.4f).epsilon(0.0001f));
        for(int x: {0, 1, 2,})REQUIRE(pt.get_rot(x)==Approx(float(x)*1.1f+1.2f).epsilon(.0001f));
    }
    SECTION("init with id and x y z rot-x rot-y rot-z values")
    {
        NodeBase<float> pt(59, 1.f, 2.f, 3.f, 1.2f, 2.3f, 3.4f);
        REQUIRE(59==pt.get_id());
        REQUIRE(CoordinateSystem::CARTESIAN==pt.get_csys());
        REQUIRE(pt.get_x()==Approx(1.f).epsilon(0.0001f));
        REQUIRE(pt.get_y()==Approx(2.f).epsilon(0.0001f));
        REQUIRE(pt.get_z()==Approx(3.f).epsilon(0.0001f));
        auto rot = pt.get_angle();
        REQUIRE(rot[0]==Approx(1.2f).epsilon(0.0001f));
        REQUIRE(rot[1]==Approx(2.3f).epsilon(0.0001f));
        REQUIRE(rot[2]==Approx(3.4f).epsilon(0.0001f));
        for(int x: {0, 1, 2,})REQUIRE(pt.get_rot(x)==Approx(float(x)*1.1f+1.2f).epsilon(.0001f));
    }
}
TEST_CASE("init cases", "[Node]")
{
    SECTION("init 01")
    {
        NodeBase<float> pt[2]{{3, {1.f, 0.f, 10.f}}, {5, {2.f, 0.f, 10.f}}};
        auto rst = NodeFunc<float, double>::coord_tran(&pt[0], &pt[1]);
        REQUIRE(std::get<0>(rst)==Approx(1.f).epsilon(0.0001f));
        fmt::print("Length:{}\n", std::get<0>(rst));
        std::cout << std::get<1>(rst) << "\n";
    }
}
