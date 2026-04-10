#include <iostream>
#include "special_t.hpp"

int main() {
    unsigned long long n = 10, m = 5;
    special_t num1(4, false), num2(4, false), res(5, false);
    num1.setval(n); num2.setval(m);

    std::cout << "num1: " << num1.bin() << "\n";
    std::cout << "num2: " << num2.bin() << "\n";

    num1.add_bytes(LEFT, 4);
    std::cout << "new num1: " << num1.bin() << "\n";
    // res.setspec(num1 - num2); 

    // std::cout << "res: " << res.bin() << "\n";

    return 0;
}

// std::cout << "num1: " << num1.bin() << "\n";
// std::cout << "num2: " << num2.bin() << "\n";