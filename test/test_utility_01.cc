#include <string>
#include <iostream>
#include "cafea/utils_ext.h"

int main(int agrc, char **argv) {
        
    auto rst = cafea::gauss_quad<4>();
    if(rst!=std::nullopt) {
        std::cout << rst->first << "\n" << rst->second << "\n";
    }
    
    return 0;
}