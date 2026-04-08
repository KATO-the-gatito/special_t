#include "special_t.hpp"

std::string special_t::bin(bool sep){
    std::string str;
    for (int i = 0; i < size; i++) {
        if (sep) str += '\'';
        for (int j = 0; j < BBITS; j++) {
            str += (((byte)(bytes_array[i] & (1 << j)) != 0) + (byte)'0');
            //std::cout << ((byte)(bytes_array[i] & (1 << j)) != 0) << " " << str << '\n';
        }
    }
    std::reverse(str.begin(), str.end());
    str.pop_back();
    return str;
}

void special_t::setspec(special_t spec) {
    Min_Max min_max = size < spec.size ? Min_Max<special_t>{*this, spec} : Min_Max<special_t>{spec, *this};
    clear();

    for (int i = 0; i < min_max.min.size; i++) {
        bytes_array[i] = spec.bytes_array[i];
    }    
}

void special_t::resize(int new_size){
    byte* new_bytes_array = new byte[new_size];
    for (int i = 0; i < new_size; i++) {
        new_bytes_array[i] = bytes_array[i];
    }
    bytes_array = new_bytes_array;
}

special_t do_action(special_t first, special_t second, char action) {
    Min_Max min_max = first.size < second.size ? Min_Max<special_t>{first, second} : Min_Max<special_t>{second, first};
    special_t result(min_max.max.size + 1, false), spec1(min_max.max.size + 1, false), spec2(min_max.max.size + 1, false);
    spec1.setspec(first); spec2.setspec(second);
    if (action == '-'){
        for (int i = 0; i < spec2.size; i++) {
            spec2.bytes_array[i] = ~spec2.bytes_array[i];
        }
        // std::cout << "spec2: " << spec2.bin() << "\n";
        special_t tmp_spec(min_max.max.size, false);
        tmp_spec.setval(1);
        spec2.setspec(do_action(spec2, tmp_spec, '+'));
        spec2.resize(min_max.max.size + 1);
        // std::cout << "spec2 aft: " << spec2.bin() << "\n";
    }
    unsigned short tmp = 0;
    byte rem = 0;

    for (int i = 0; i < min_max.min.size + 1; i++) {
        tmp = (short)(spec1.bytes_array[i]) + (short)(spec2.bytes_array[i]) + (short)(rem);
        result.bytes_array[i] = (byte)tmp;
        rem = (tmp >> 8);
    }
    return result;
}

special_t operator+ (special_t first, special_t second) { 
    return do_action(first, second, '+');
}
special_t operator- (special_t first, special_t second) { 
    return do_action(first, second, '-');
}


