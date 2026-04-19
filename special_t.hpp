#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <limits>

#define BBITS 8
#define LEFT 0
#define RIGHT 1

typedef unsigned char byte;

template<typename T>
struct Min_Max
{
    T min, max;
};
template<typename T>
union splitted_num {
    T num;
    byte part[sizeof(T)];
};

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
int find_valid_size(T num) {
    splitted_num<T> n;
    n.num = num;
    for (int i = sizeof(T) - 1; i >= 0; i--) {
        if (n.part[i])
            return i + 1;
    }
    return 0;
}
template<typename T>
void clear_buffer(T* buffer, int len){
    for (int i = 0; i < len; i++){
        buffer[i] = 0;
    }
}

class special_t
{  
public:
    special_t() : size(4), is_signed(true), sign(false), bytes_array(new byte[4])
    { clear(); }
    special_t(int size, bool is_signed) : size(size), is_signed(is_signed), sign(false), bytes_array(new byte[size])
    { clear(); }
    special_t(const special_t& spec) : size(spec.size), is_signed(spec.is_signed), sign(spec.is_signed ? spec.sign : false), bytes_array(new byte[spec.size])
    {
        for (int i = 0; i < size; i++) {
            bytes_array[i] = spec.bytes_array[i];
        }
        
    }
    template<typename T>
    special_t(T val) : size(sizeof(T)), is_signed(std::numeric_limits<T>::is_signed), bytes_array(new byte[sizeof(T)])
    {
        this->setval(val); // sign init there
    }
    
    ~special_t() 
    {
        delete[] bytes_array;
    }

    void clear(){
        for (int i = 0; i < size; i++){
            bytes_array[i] = 0;
        }
    }
    std::string bin(bool sep = true);
    std::string dec();
    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    special_t& setval(T val) {
        sign = val < 0;
        clear();
        int val_j = 0;
        for (int i = 0; i < sizeof(T); i++) {
            for (int j = 0; j < BBITS; j++, val_j++) {
                if (val & ((T)1 << val_j)) // костыль прям
                    bytes_array[i] = bytes_array[i] | (1 << j); 
            }
        }
        return *this;
    }
    special_t& setspec(special_t);
    special_t& resize(int new_size);
    special_t& invert();
    special_t& negate();
    special_t& add_bytes(byte dir, int count);
    special_t& shift(byte dir, int count);
    bool get_bit(int index);
    special_t& set_bit(int index, bool val);
    int get_valid_size();

    special_t& operator= (special_t spec);
    template<typename T>
    special_t& operator= (T val) {
        this->setval(val);
        return *this;
    }
    
    friend std::ostream& operator<< (std::ostream& console_out, special_t& spec) {
        console_out << spec.dec();
        return console_out;
    }

//private:
    byte* bytes_array;
    int size;
    bool is_signed;
    bool sign; // '-' - true, '+' - false 
};

void clear_zeros(std::string& str);
special_t do_action(special_t first, special_t second, char action);
special_t power(special_t spec, long long exp, int res_size = 0);

special_t operator+ (special_t first, special_t second);
special_t operator- (special_t first, special_t second);
special_t operator* (special_t first, special_t second);
special_t operator/ (special_t first, special_t second);
special_t operator% (special_t first, special_t second);
special_t operator<< (special_t spec, int count);
special_t operator>> (special_t spec, int count);
special_t operator& (special_t first, special_t second);
special_t operator| (special_t first, special_t second);
special_t operator^ (special_t first, special_t second);

special_t& operator+= (special_t& first, special_t second);
special_t& operator-= (special_t& first, special_t second);
special_t& operator*= (special_t& first, special_t second);
special_t& operator<<= (special_t& spec, int count);
special_t& operator>>= (special_t& spec, int count);
special_t& operator~ (special_t& spec);
special_t& operator&= (special_t& first, special_t second);
special_t& operator|= (special_t& first, special_t second);
special_t& operator^= (special_t& first, special_t second);

bool operator== (special_t first, special_t second);
bool operator!= (special_t first, special_t second);
bool operator<= (special_t first, special_t second);
bool operator>= (special_t first, special_t second);
bool operator< (special_t first, special_t second);
bool operator> (special_t first, special_t second);