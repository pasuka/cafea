#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <ostream>
#include <algorithm>

#include "load.h"
#include "func_random.hpp"

using cafea::LoadSet;
using cafea::LoadCell;

template <class T=float>
LoadCell<T> gen_random_cell()
{
    int id = random_value(1, 10000000);
    LoadCell<T> tmp{id, gen_load_type(), gen_dof_label(), gen_load_domain()};
    // std::cout << tmp;
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
