#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "load.h"

using cafea::LoadSet;
using cafea::LoadCell;
using cafea::LoadType;
using cafea::LoadDomain;
using cafea::DofLabel;
using cafea::DofType;

TEST_CASE("init", "[LoadCell]")
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
};
