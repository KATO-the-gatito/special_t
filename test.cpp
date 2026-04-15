#include <iostream>
#include "special_t.hpp"

int main() {
    special_t num1 = 2, num2 = 8;

    std::cout << "num1: " << num1 << "\n";
    std::cout << "num2: " << num2 << "\n";    

    num1 *= num2;

    std::cout << "2 * 18 = " << num1 << "\n";
    //exit(0);
    return 0;
}