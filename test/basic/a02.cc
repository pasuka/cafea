#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <random>
#include <ostream>
#include <algorithm>

#include "load.h"

using cafea::LoadSet;
using cafea::LoadCell;
using cafea::LoadType;
using cafea::LoadDomain;
using cafea::DofLabel;

template <class T=float>
LoadCell<T> gen_random_cell()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    int id, load_type, dof_label, load_domain;
    LoadType lt;
    LoadDomain ld;
    DofLabel dl;
    {
        std::uniform_int_distribution<> dis(1, 1e6);
        id = dis(gen);
    }
    {
        std::uniform_int_distribution<> dis(1, 7);
        load_type = dis(gen);
        switch (load_type) {
        case 1: lt = LoadType::FORCE;  break;
        case 2: lt = LoadType::DISP;   break;
        case 3: lt = LoadType::VEL;    break;
        case 4: lt = LoadType::ACCEL;  break;
        case 5: lt = LoadType::PRES;   break;
        case 6: lt = LoadType::STRESS; break;
        case 7:
        default: lt = LoadType::UNKNOWN;
        }
    }
    {
        std::uniform_int_distribution<> dis(1, 11);
        dof_label = dis(gen);
        switch (dof_label) {
        case 1: dl = DofLabel::UX;     break;
        case 2: dl = DofLabel::UY;     break;
        case 3: dl = DofLabel::UZ;     break;
        case 4: dl = DofLabel::URX;    break;
        case 5: dl = DofLabel::URY;    break;
        case 6: dl = DofLabel::URZ;    break;
        case 7: dl = DofLabel::WARP;   break;
        case 8: dl = DofLabel::U_ALL;  break;
        case 9: dl = DofLabel::UR_ALL; break;
        case 10: dl = DofLabel::ALL;   break;
        case 11:
        default : dl = DofLabel::UNKNOWN;
        }
    }
    {
        std::uniform_int_distribution<> dis(1,2);
        load_domain = dis(gen);
        switch (load_domain) {
        case 1: ld = LoadDomain::FREQ; break;
        case 2:
        default: ld = LoadDomain::TIME;
        }
    }
    LoadCell<T> tmp{id, lt, dl, ld};
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
    for(int i=0; i<1e3; i++)pa.add_load(gen_random_cell<float>());
    
}
