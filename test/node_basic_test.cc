#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <complex>
#include <iostream>
#include <vector>

#include "base.h"
#include "boundary.h"
#include "node.h"

template <class Scalar, class ResType>
int node_add_test()
{
	using cafea::Node;
	using cafea::CoordinateSystem;
	
	std::vector<Node<Scalar, ResType> > list2;
	list2.push_back({100});
	list2.push_back({101, 1.0f, 2.0f, 3.0f});
	list2.push_back({103, CoordinateSystem::SPHERICAL,2.0f, 3.5f, 3.0f});
	list2.push_back({104, CoordinateSystem::CYLINDRICAL});
	list2.push_back({202, 0.1, 0.1, 0.1, 20.0, 30.0, 4.e1});
	list2.push_back({203, CoordinateSystem::UNKNOWN,0.1, 0.1, 0.1, 20.0, 30.0, 4.e1});
	for(const auto& x: list2)std::cout << x;
	return 0;
}

TEST_CASE("Node basic operator", "[Node Basic]")
{
	REQUIRE((node_add_test<float, double>()) == 0);
	REQUIRE((node_add_test<double, double>()) == 0);
	REQUIRE((node_add_test<float, std::complex<float> >()) == 0);
	REQUIRE((node_add_test<float, std::complex<double> >()) == 0);
	REQUIRE((node_add_test<double, std::complex<float> >()) == 0);
	REQUIRE((node_add_test<double, std::complex<double> >()) == 0);
}