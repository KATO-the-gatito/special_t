#include <iostream>
#include "special_t.hpp"

int main() {
    special_t num1(30, true), num2(30, true), res(5'000, false);
    num1 = 125;

    std::cout << "num1: " << num1.bin() << "\n"; 
    std::cout << "num2: " << num2.bin() << "\n"; 
    res = power(num1, 1230, res.size);
    std::cout << "res: " << res << "\n";
    //std::cout << "bin res: " << res.bin() << "\n";
    
    return 0;
}