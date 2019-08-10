#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "cafea/base/base.hpp"

using cafea::ObjectBase;

TEST_CASE("init", "[ObjectBase]") {
    ObjectBase obj(42, "Basic Object");
    REQUIRE(obj.get_id() == 42);
    REQUIRE(obj.get_name() == "Basic Object#42");
    for (const auto &x: obj.get_group()) REQUIRE(x == 0);
    {
        ObjectBase obj;
        REQUIRE(obj.get_id() == -1);
        REQUIRE(obj.get_name() == "Empty");
        auto gp = obj.get_group();
        for (const auto &x: gp) REQUIRE(x == 0);
    }
}

TEST_CASE("id", "[ObjectBase]") {
    ObjectBase obj;
    obj.set_id(37);
    REQUIRE(obj.get_id() == 37);
    obj.set_id(41);
    REQUIRE(obj.get_id() == 41);
}

TEST_CASE("name", "[ObjectBase]") {
    ObjectBase obj;
    obj.set_name("String name");
    REQUIRE(obj.get_name() == "String name");
    obj.set_name(789);
    REQUIRE(obj.get_name() == "789");
    obj.set_name(123.45);
    REQUIRE(obj.get_name() == "123.45");
    obj.set_name("Node#123");
    REQUIRE(obj.get_name() == "Node#123");
}

TEST_CASE("group", "[ObjectBase]") {
    ObjectBase obj;
    obj.set_group({3, 5, 7, 11, 13, 17, 19, 23,});
    auto gp = obj.get_group();
    int i{0};
    for (auto x: {3, 5, 7, 11, 13, 17, 19, 23,}) REQUIRE(x == gp[i++]);

    int a[9]{29, 31, 37, 41, 43, 47, 53, 59, 61,};
    obj.set_group(a, 9);
    {
        auto gp = obj.get_group();
        for (int i = 0; i < 9; i++) REQUIRE(gp[i] == a[i]);
    }
}
