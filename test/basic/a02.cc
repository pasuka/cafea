#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <chrono>
#include <random>
#include <ostream>
#include <algorithm>

#include "load.h"

using cafea::LoadSet;
using cafea::LoadCell;
using cafea::LoadType;
using cafea::LoadDomain;
using cafea::DofLabel;

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 gen(seed);

int random_value(int a, int b)
{
    std::uniform_int_distribution<> dis(a, b);
    return dis(gen);
}

template <class T=float>
T random_value(T a, T b)
{
    std::uniform_real_distribution<T> dis(a, b);
    return dis(gen);
}

LoadType gen_load_type()
{
    int load_type = random_value(1, 7);
    LoadType lt;
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
    return lt;
}

DofLabel gen_dof_label()
{
    int dof_label = random_value(1, 11);
    DofLabel dl;
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
    return dl;
}

template <class T=float>
LoadCell<T> gen_random_cell()
{
    int id = random_value(1, 10000000);

    int load_domain = 1;
    LoadDomain ld;
    switch (load_domain) {
    case 1: ld = LoadDomain::FREQ; break;
    case 2:
    default: ld = LoadDomain::TIME;
    }

    LoadCell<T> tmp{id, gen_load_type(), gen_dof_label(), ld};
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
    std::cout << pa;
}

TEST_CASE("init load set", "[LoadSet]")
{
    LoadSet<float> pa(31, LoadDomain::FREQ, 1.25);
    REQUIRE(pa.get_id()==31);
    pa.set_id(43);
    REQUIRE(pa.get_id()==43);
    REQUIRE(pa.get_load_domain()==LoadDomain::FREQ);
    REQUIRE(pa.get_value()==Approx(1.25f).epsilon(0.0001f));
    pa.set_load_domain(LoadDomain::TIME);
    REQUIRE(pa.get_load_domain()==LoadDomain::TIME);
    pa.set_value(2.9f);
    REQUIRE(pa.get_value()==Approx(2.9f).epsilon(0.0001f));
    std::cout << pa;
}

TEST_CASE("add load cell 01", "[LoadSet]")
{
    LoadSet<float> pa(random_value(1, 100), LoadDomain::FREQ, 2.25f);
    for(int num: {10, 100, 1000, 10000, 100000, 1000000,}){
        for(int i=0; i<num; i++)pa.add_load(gen_random_cell<float>());
        REQUIRE(pa.get_count()==num);
        int total = num;
        for(auto x: {LoadType::FORCE, LoadType::DISP, LoadType::VEL, LoadType::PRES,
            LoadType::ACCEL, LoadType::STRESS, LoadType::UNKNOWN,}){
            auto rst = pa.get_load_by_type(x);
            REQUIRE(rst.size()>=0);
            total -= rst.size();
        }
        REQUIRE(total==0);
        total = num;
        for(auto x: {DofLabel::UX, DofLabel::UY, DofLabel::UZ, DofLabel::URX, DofLabel::URY, DofLabel::URZ,
            DofLabel::WARP, DofLabel::U_ALL, DofLabel::UR_ALL, DofLabel::ALL, DofLabel::UNKNOWN,}){
            auto rst = pa.get_load_by_dof(x);
            REQUIRE(rst.size()>=0);
            total -= rst.size();
        }
        REQUIRE(total==0);
        pa.clear();
        REQUIRE(pa.get_count()==0);
    }
}

TEST_CASE("add load cell 02", "[LoadSet]")
{
    LoadSet<float> pa(random_value(1, 1000), LoadDomain::TIME, random_value<float>(0.f, 16.f));
    for(int num: {10, 100, 1000, 10000, 100000, 1000000,}){
        for(int i=0; i<num; i++){
            auto y = pa.add_load(random_value(1, num*2), gen_load_type(), gen_dof_label(), random_value<float>(-2.f*num, 2.f*num));
            REQUIRE(y==0);
        }
        REQUIRE(pa.get_count()==num);
        int total = num;
        for(auto x: {LoadType::FORCE, LoadType::DISP, LoadType::VEL, LoadType::PRES,
            LoadType::ACCEL, LoadType::STRESS, LoadType::UNKNOWN,}){
            auto rst = pa.get_load_by_type(x);
            REQUIRE(rst.size()>=0);
            total -= rst.size();
        }
        REQUIRE(total==0);
        total = num;
        for(auto x: {DofLabel::UX, DofLabel::UY, DofLabel::UZ, DofLabel::URX, DofLabel::URY, DofLabel::URZ,
            DofLabel::WARP, DofLabel::U_ALL, DofLabel::UR_ALL, DofLabel::ALL, DofLabel::UNKNOWN,}){
            auto rst = pa.get_load_by_dof(x);
            REQUIRE(rst.size()>=0);
            total -= rst.size();
        }
        REQUIRE(total==0);
        pa.clear();
        REQUIRE(pa.get_count()==0);
    }
}

TEST_CASE("add load cell 03", "[LoadSet]")
{
    LoadSet<float> pa(random_value(1, 1000), LoadDomain::FREQ, random_value<float>(0.f, 1.e3f));
    for(int num: {10, 100, 1000, 10000, 100000, 1000000,}){
        for(int i=0; i<num; i++){
            if(0==i%2){
                auto y = pa.add_load(random_value(1, num*2), gen_load_type(), gen_dof_label(),
                random_value<float>(-2.f, 2.f), random_value<float>(-3.f, 3.f));
                REQUIRE(y==0);
            }
            else{
                std::complex<float> val(random_value<float>(-5.f, 5.f), random_value<float>(-7.f, 7.f));
                auto y = pa.add_load(random_value(num*3, num*4), gen_load_type(), gen_dof_label(), val);
                REQUIRE(y==0);
            }
        }
        REQUIRE(pa.get_count()==num);
        int total = num;
        for(auto x: {LoadType::FORCE, LoadType::DISP, LoadType::VEL, LoadType::PRES,
            LoadType::ACCEL, LoadType::STRESS, LoadType::UNKNOWN,}){
            auto rst = pa.get_load_by_type(x);
            REQUIRE(rst.size()>=0);
            total -= rst.size();
        }
        REQUIRE(total==0);
        total = num;
        for(auto x: {DofLabel::UX, DofLabel::UY, DofLabel::UZ, DofLabel::URX, DofLabel::URY, DofLabel::URZ,
            DofLabel::WARP, DofLabel::U_ALL, DofLabel::UR_ALL, DofLabel::ALL, DofLabel::UNKNOWN,}){
            auto rst = pa.get_load_by_dof(x);
            REQUIRE(rst.size()>=0);
            total -= rst.size();
        }
        REQUIRE(total==0);
        pa.clear();
        REQUIRE(pa.get_count()==0);
    }
}
