#include <string>
#include <iostream>
#include "cafea/utils/gauss_legendre.hpp"

int main(int agrc, char **argv) {
        
    constexpr size_t x[]{5,3, 20,10,7,8,9,11};
    
    auto [pt, wt] = cafea::gauss_quad<x[3]>();
    
    std::cout << "Gauss quadrature: " << x[3] << "\nPoints\n";
    std::cout << pt << "\nWeights:\n" << wt << "\n";
    
    
    return 0;
}