#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "cafea/utils/gauss_legendre.hpp"
#include "integration.hpp"

TEST_CASE("Gauss Legendre quadrature", "[Utility]") {
    SECTION("Compare results") {
        constexpr size_t nPoints = 1;
        auto [pt, wt] = cafea::gauss_quad<nPoints>();
        auto [pt_base, wt_base] = gauss_quad_table(nPoints);
        for(int i=0; i<nPoints; i++) {
            // REQUIRE(pt[i]==Approx(pt_base[i]).epsilon(0.0001f));
            REQUIRE(wt[i]==Approx(wt_base[i]).epsilon(0.0001f));
        }
    }
    SECTION("Compare results") {
        constexpr size_t nPoints = 2;
        auto [pt, wt] = cafea::gauss_quad<nPoints>();
        auto [pt_base, wt_base] = gauss_quad_table(nPoints);
        for(int i=0; i<nPoints; i++) {
            REQUIRE(pt[i]==Approx(pt_base[i]).epsilon(0.0001f));
            REQUIRE(wt[i]==Approx(wt_base[i]).epsilon(0.0001f));
        }
    }
    SECTION("Compare results") {
        constexpr size_t nPoints = 3;
        auto [pt, wt] = cafea::gauss_quad<nPoints>();
        auto [pt_base, wt_base] = gauss_quad_table(nPoints);
        for(int i=0; i<nPoints; i++) {
            // REQUIRE(pt[i]==Approx(pt_base[i]).epsilon(0.0001f));
            REQUIRE(wt[i]==Approx(wt_base[i]).epsilon(0.0001f));
        }
    }
    SECTION("Compare results") {
        constexpr size_t nPoints = 4;
        auto [pt, wt] = cafea::gauss_quad<nPoints>();
        auto [pt_base, wt_base] = gauss_quad_table(nPoints);
        for(int i=0; i<nPoints; i++) {
            REQUIRE(pt[i]==Approx(pt_base[i]).epsilon(0.0001f));
            REQUIRE(wt[i]==Approx(wt_base[i]).epsilon(0.0001f));
        }
    }
    SECTION("Compare results") {
        constexpr size_t nPoints = 5;
        auto [pt, wt] = cafea::gauss_quad<nPoints>();
        auto [pt_base, wt_base] = gauss_quad_table(nPoints);
        for(int i=0; i<nPoints; i++) {
            // REQUIRE(pt[i]==Approx(pt_base[i]).epsilon(0.0001f));
            REQUIRE(wt[i]==Approx(wt_base[i]).epsilon(0.0001f));
        }
    }
    SECTION("Compare results") {
        constexpr size_t nPoints = 6;
        auto [pt, wt] = cafea::gauss_quad<nPoints>();
        auto [pt_base, wt_base] = gauss_quad_table(nPoints);
        for(int i=0; i<nPoints; i++) {
            REQUIRE(pt[i]==Approx(pt_base[i]).epsilon(0.0001f));
            REQUIRE(wt[i]==Approx(wt_base[i]).epsilon(0.0001f));
        }
    }
    SECTION("Compare results") {
        constexpr size_t nPoints = 7;
        auto [pt, wt] = cafea::gauss_quad<nPoints>();
        auto [pt_base, wt_base] = gauss_quad_table(nPoints);
        for(int i=0; i<nPoints; i++) {
            // REQUIRE(pt[i]==Approx(pt_base[i]).epsilon(0.0001f));
            REQUIRE(wt[i]==Approx(wt_base[i]).epsilon(0.0001f));
        }
    }
    SECTION("Compare results") {
        constexpr size_t nPoints = 8;
        auto [pt, wt] = cafea::gauss_quad<nPoints>();
        auto [pt_base, wt_base] = gauss_quad_table(nPoints);
        for(int i=0; i<nPoints; i++) {
            REQUIRE(pt[i]==Approx(pt_base[i]).epsilon(0.0001f));
            REQUIRE(wt[i]==Approx(wt_base[i]).epsilon(0.0001f));
        }
    }
    SECTION("Compare results") {
        constexpr size_t nPoints = 9;
        auto [pt, wt] = cafea::gauss_quad<nPoints>();
        auto [pt_base, wt_base] = gauss_quad_table(nPoints);
        for(int i=0; i<nPoints; i++) {
            // REQUIRE(pt[i]==Approx(pt_base[i]).epsilon(0.0001f));
            REQUIRE(wt[i]==Approx(wt_base[i]).epsilon(0.0001f));
        }
    }
    SECTION("Compare results") {
        constexpr size_t nPoints = 10;
        auto [pt, wt] = cafea::gauss_quad<nPoints>();
        auto [pt_base, wt_base] = gauss_quad_table(nPoints);
        for(int i=0; i<nPoints; i++) {
            REQUIRE(pt[i]==Approx(pt_base[i]).epsilon(0.0001f));
            REQUIRE(wt[i]==Approx(wt_base[i]).epsilon(0.0001f));
        }
    }
}