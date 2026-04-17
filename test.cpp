#include <iostream>
#include "special_t.hpp"

int main() {
    special_t num1(100000, false), num2(100000, false), res(100000, false);
    num1 = 2;
    num2 = 20;

    std::cout << "num1: " << num1 << "\n"; 
    std::cout << "num2: " << num2 << "\n"; 
    res = num1 / num2;
    std::cout << "res: " << res << "\n";
    
    return 0;
}
/*

todo:
1) devision (/ and %); +
2) init size and bytes_array using struct in constructor;
3) exponentiation; +
4) operator !=; +
5) len of valid data (non zero) +

*/
