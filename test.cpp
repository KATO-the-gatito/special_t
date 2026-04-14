#include <iostream>
#include "special_t.hpp"

int main() {
    unsigned long long n = 92, m = 92;
    special_t num1(20, false), num2(20, false), res(200, false);
    num1.setval(n); num2.setval(m);
    
    std::cout << "num1: " << num1.bin() << "\n";
    std::cout << "num2: " << num2.bin() << "\n";
    res = num1;
    for (int i = 0; i < 100; i++) {
        res *= num1;
    }
    //res.resize(4);
    std::cout << "res:  " << res.bin() << "\n";
    //exit(0);
    return 0;
}

// std::cout << "num1: " << num1.bin() << "\n";
// std::cout << "num2: " << num2.bin() << "\n";