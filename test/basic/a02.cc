#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <random>
#include <ostream>

#include "load.h"

using cafea::LoadSet;
using cafea::LoadCell;
using cafea::LoadType;
using cafea::LoadDomain;
using cafea::DofLabel;
using cafea::DofType;

template <class T=float>
LoadCell<T> gen_random_cell()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    LoadCell<T> tmp;
    return std::move(tmp);
}

TEST_CASE("init load cell", "[LoadCell]")
{
    LoadCell<float> pa{42, LoadType::VEL, DofLabel::UZ, LoadDomain::TIME};
    pa.val_ = 1.655f;
    REQUIRE(pa.id_==42);
    REQUIRE(pa.lt_==LoadType::VEL);
    REQUIRE(pa.df_==DofLabel::UZ);
    REQUIRE(pa.ld_==LoadDomain::TIME);
    REQUIRE(pa.val_==Approx(1.655f).epsilon(0.0001f));
    LoadCell<double> pb{};
    REQUIRE(pb.id_==-1);
}

TEST_CASE("init load set", "[LoadSet]")
{
    LoadSet<float> pa(31, LoadDomain::FREQ, 1.25);
    REQUIRE(pa.get_id()==31);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 6);


}
