#include <iostream>
#include "special_t.hpp"

int main() {
    unsigned long long n = 18'446'744'073'709'551'615, m = 1;
    special_t num1(8, false), num2(8, false); 
    num1.setval(n); num2.setval(m);
    std::cout << "num1: " << num1.bin() << "\n";
    std::cout << "num2: " << num2.bin() << "\n";
    special_t res(10, false);
    res.setspec(num1 - num2); 

    std::cout << res.bin() << "\n";

    return 0;
}

// std::cout << "num1: " << num1.bin() << "\n";
// std::cout << "num2: " << num2.bin() << "\n";