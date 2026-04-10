#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>

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
void clear_buffer(T* buffer, int len){
    for (int i = 0; i < len; i++){
        buffer[i] = 0;
    }
}

class special_t
{  
public:
    special_t() : size(4), is_signed(false), bytes_array(new byte[4])
    { clear(); }
    special_t(int size, bool is_signed) : size(size), is_signed(is_signed), bytes_array(new byte[size])
    { clear(); }
    special_t(const special_t& spec) : size(spec.size), bytes_array(new byte[spec.size]), is_signed(spec.is_signed)
    {
        for (int i = 0; i < size; i++) {
            bytes_array[i] = spec.bytes_array[i];
        }
        
    }
    
    ~special_t() 
    {
        if (bytes_array) delete[] bytes_array;
    }

    void clear(){
        for (int i = 0; i < size; i++){
            bytes_array[i] = 0;
        }
    }
    std::string bin(bool sep = true);
    //std::string dec(bool sep = true);
    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
    special_t& setval(T val) {
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
    

//private:
    byte* bytes_array;
    int size;
    bool is_signed;
};

void clear_zeros(std::string& str);
special_t do_action(special_t first, special_t second, char action);

special_t operator+ (special_t first, special_t second);
special_t operator- (special_t first, special_t second);