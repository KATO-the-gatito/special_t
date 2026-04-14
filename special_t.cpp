#include "special_t.hpp"

std::string special_t::bin(bool sep){
    std::string str;
    for (int i = 0; i < size; i++) {
        if (sep) str += '\'';
        for (int j = 0; j < BBITS; j++) {
            str += (((byte)(bytes_array[i] & (1 << j)) != 0) + '0');
            //std::cout << ((byte)(bytes_array[i] & (1 << j)) != 0) << " " << str << '\n';
        }
    }
    std::reverse(str.begin(), str.end());
    str.pop_back();
    return str;
}

special_t& special_t::setspec(special_t spec) {
    Min_Max min_max = size < spec.size ? Min_Max<special_t>{*this, spec} : Min_Max<special_t>{spec, *this};
    clear();

    for (int i = 0; i < min_max.min.size; i++) {
        bytes_array[i] = spec.bytes_array[i];
    }    
    return *this;
}

special_t& special_t::resize(int new_size){
    byte* new_bytes_array = new byte[new_size];
    for (int i = 0; i < new_size; i++) {
        new_bytes_array[i] = bytes_array[i];
    }
    delete[] bytes_array;
    bytes_array = new_bytes_array;
    return *this;
}

special_t& special_t::invert(){
    for (int i = 0; i < size; i++) {
        bytes_array[i] = ~bytes_array[i];
    }
    return *this;
}

special_t& special_t::negate() {
    this->invert();
    special_t tmp_spec(size, false);
    tmp_spec.setval(1);
    setspec(do_action(*this, tmp_spec, '+'));
    return *this;
}

special_t& special_t::add_bytes(byte dir, int count) {
    int new_size = size + count;
    byte* tmp_bytes_array = new byte[new_size];
    clear_buffer(tmp_bytes_array, new_size);
    int glob_i = 0;

    switch (dir)
    {
    case LEFT:
        glob_i = 0;
        break;
    case RIGHT:
        glob_i = count;
        break;
    default:
        perror("wrong direction for adding bytes!\n");
        break;
    }
    
    for (int i = 0; i < size; i++, glob_i++) {
        tmp_bytes_array[glob_i] = bytes_array[i];
    }
    delete[] bytes_array;
    bytes_array = tmp_bytes_array;
    size = new_size;
    return *this;
}

special_t& special_t::shift(byte dir, int count) {
    bool is_needtobreak = false;
    while (true){
        int shift_cnt = 0;
        if (count >= 0 && count <= 8){
            shift_cnt = count;
            is_needtobreak = true;
        }
        else if (count < 0)
            break;
        else {
            shift_cnt = 8;
            count -= 8;
        }
        
        switch (dir)
        {
        case LEFT:
        {
            short tmp = 0;
            byte old_tmp = 0;
            for (int i = 0; i < size; i++) {
                tmp = bytes_array[i] << shift_cnt;
                bytes_array[i] = (byte)tmp;
                bytes_array[i] += old_tmp;
                old_tmp = (byte)(tmp >> 8);
            }
        }
            break;
        case RIGHT:
        {
            short tmp = 0;
            byte old_tmp = 0;
            for (int i = size - 1; i >= 0; i--) {
                tmp = bytes_array[i] << 8 >> shift_cnt;
                bytes_array[i] = (byte)(tmp >> 8);
                bytes_array[i] += old_tmp;
                old_tmp = (byte)tmp;
            }
        }
            break;
        default:
            perror("wrong direction for shifting!\n");
            break;
        }

        if (is_needtobreak) break;
    }
    return *this;
}

//--------------------------------------------------------------------------------------------------------

void clear_zeros(std::string& str){
    std::string tmp_str = str;
    str.clear();
    bool flag = false;
    for (char& ch : tmp_str){
        if (ch != '0' && ch != '\'' && !flag) flag = true;
        if (flag) str += ch;
    }
}

special_t do_action(special_t first, special_t second, char action) {
    Min_Max min_max = first.size < second.size ? Min_Max<special_t>{first, second} : Min_Max<special_t>{second, first};
    special_t result(min_max.max.size + 1, false), spec1(min_max.max.size + 1, false), spec2(min_max.max.size + 1, false);
    spec1.setspec(first); spec2.setspec(second);

    switch (action)
    {
    case '-':
    {
        spec2.negate();
        spec2.resize(min_max.max.size + 1);
    }
        // fall through <----------!
    case '+':
    {
        unsigned short tmp = 0;
        byte rem = 0;

        for (int i = 0; i < min_max.min.size + 1; i++) {
            tmp = (short)(spec1.bytes_array[i]) + (short)(spec2.bytes_array[i]) + (short)(rem);
            result.bytes_array[i] = (byte)tmp;
            rem = (tmp >> 8);
        }  
    }
        break;
    case '*':
    {
        special_t A(min_max.max.size * 2 + 1, false);
        special_t S(min_max.max.size * 2 + 1, false);
        special_t P(min_max.max.size * 2 + 1, false);
        A.setspec(spec1).add_bytes(RIGHT, min_max.max.size + 1);
        S.setspec(spec1.negate()).add_bytes(RIGHT, min_max.max.size + 1);
        P.setspec(spec2).add_bytes(LEFT, min_max.max.size).add_bytes(RIGHT, 1);

        // in progress...

    }
        break;
    default:
        perror("wrong action!\n");
        break;
    }

    return result; 
}

special_t operator+ (special_t first, special_t second) { 
    return do_action(first, second, '+');
}
special_t operator- (special_t first, special_t second) { 
    return do_action(first, second, '-');
}


