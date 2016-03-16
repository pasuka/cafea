#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <complex>
#include <iostream>
#include <vector>
#include <tuple>

#include "cafea.h"
	
template <class Scalar, class ResType>
int node_add_test()
{
	using cafea::Node;
	using cafea::CoordinateSystem;
	
	std::vector<Node<Scalar, ResType> > list2;
	list2.push_back({100});
	list2.push_back({101, 1.0f, -2.0f, 3.0f});
	list2.push_back({103, CoordinateSystem::SPHERICAL,2.0f, 3.5f, -3.0f});
	list2.push_back({104, CoordinateSystem::CYLINDRICAL});
	list2.push_back({202, -0.1, 0.1, 0.1, 20.0, 30.0, 4.e1});
	list2.push_back({203, CoordinateSystem::UNKNOWN,0.1, -0.1, 0.1, 20.500002, 30.60, 4.2e1});
	// for(const auto& x: list2)std::cout << x;
	return 0;
}

template <class Scalar, class ResType>
Scalar triangle_area_test_01()
{
	using cafea::NodeBase;
	using cafea::Node;
	using cafea::coord_tran;
	using cafea::matrix_;
	using std::tie;
	
	Node<Scalar, ResType> p1{101, 0, 0, 0};
	Node<Scalar, ResType> p2{102, 2, 0, 0};
	Node<Scalar, ResType> p3{103, 0, 2, 0};
	Node<Scalar, ResType> p4{104, -2, 2, 0};
	
	NodeBase<Scalar> pp1{101, 0, 0, 0};
	NodeBase<Scalar> pp2{102, 2, 0, 0};
	NodeBase<Scalar> pp3{103, 0, 2, 0};
	NodeBase<Scalar> pp4{104, -2, 2, 0};
	Scalar area{2};
	matrix_<Scalar> xy, tran;
	tie(area, xy, tran) = coord_tran<Scalar>(&p1, &p2, &p3);
	return area;
}

TEST_CASE("Node basic operator", "[Node Basic]")
{
	REQUIRE((node_add_test<float, double>()) == 0);
	REQUIRE((node_add_test<double, double>()) == 0);
	REQUIRE((node_add_test<float, std::complex<float> >()) == 0);
	REQUIRE((node_add_test<float, std::complex<double> >()) == 0);
	REQUIRE((node_add_test<double, std::complex<float> >()) == 0);
	REQUIRE((node_add_test<double, std::complex<double> >()) == 0);
	REQUIRE((triangle_area_test_01<float, double>()) == Approx(2.0f).epsilon(0.001f));
}