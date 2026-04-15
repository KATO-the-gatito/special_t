#include <iostream>
#include "special_t.hpp"

int main() {
    char n = 22, m = 55;
    special_t num1(1, false), num2(1, false), res(1, false);
    num1.setval(n); num2.setval(m);
    
    std::cout << "num1: " << num1 << "\n";
    std::cout << "num2: " << num2 << "\n";
    std::cout << "num1: " << num1.bin() << "\n";
    std::cout << "num2: " << num2.bin() << "\n";

    //res.resize(4);
    std::cout << "res:  " << (int)(num1 <= num2) << "\n";
    //exit(0);
    return 0;
}

// std::cout << "num1: " << num1.bin() << "\n";
// std::cout << "num2: " << num2.bin() << "\n";