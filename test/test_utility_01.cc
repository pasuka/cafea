#include <string>
#include <iostream>
#include "cafea/utils_ext.h"

int main(int agrc, char **argv) {
        
    constexpr size_t x[]{5,3, 2,6,7,8,9,11};
    
    auto [pt, wt] = cafea::gauss_quad<x[3]>();
    
    std::cout << "Gauss quadrature: " << x[3] << "\n";
    std::cout << pt << "\n" << wt << "\n";
    
    
    return 0;
}