#include <array>
#include <string>
#include <iostream>
#include "cafea/utils_ext.h"

int main(int agrc, char **argv) {
        
    constexpr size_t x[3]{2,3};
    auto [pt, wt] = cafea::gauss_quad<x[0]>();
    
    std::cout << "Gauss quadrature: " << x[0] << "\n";
    std::cout << pt << "\n" << wt << "\n";
    
    {auto [pt, wt] = cafea::gauss_quad<x[1]>();
    
    std::cout << "Gauss quadrature: " << x[1] << "\n";
    std::cout << pt << "\n" << wt << "\n";
    }
    return 0;
}