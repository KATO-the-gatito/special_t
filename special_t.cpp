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
    if (sep) str.pop_back();
    return str;
}
std::string special_t::dec() {
    std::string result = "0";
    
    for (char bit : bin(false)) {
        int carry = 0;
        for (int i = result.size() - 1; i >= 0; --i) {
            int digit = (result[i] - '0') * 2 + carry;
            result[i] = (digit % 10) + '0';
            carry = digit / 10;
        }
        if (carry) result.insert(result.begin(), carry + '0');
        
        if (bit == '1') {
            int i = result.size() - 1;
            while (i >= 0 && result[i] == '9') {
                result[i] = '0';
                i--;
            }
            if (i >= 0) result[i]++;
            else result.insert(result.begin(), '1');
        }
    }
    
    return result;
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
    int min_size = std::min(new_size, size);
    clear_buffer(new_bytes_array, new_size);
    for (int i = 0; i < min_size; i++) {
        new_bytes_array[i] = bytes_array[i];
    }
    delete[] bytes_array;
    bytes_array = new_bytes_array;
    size = new_size;
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

special_t& special_t::operator= (special_t spec) {
    return this->setspec(spec);
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
    special_t result(min_max.max.size + 1, false), spec1(min_max.max.size, false), spec2(min_max.max.size, false);
    spec1.setspec(first); spec2.setspec(second);

    switch (action)
    {
    case '-':
    {
        spec1.resize(min_max.max.size + 1);
        spec2.resize(min_max.max.size + 1);
        spec2.negate();
        spec2.resize(min_max.max.size + 1);
    }
        // fall through <----------!
    case '+':
    {
        spec1.resize(min_max.max.size + 1);
        spec2.resize(min_max.max.size + 1);
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
        special_t A(min_max.max.size, false);
        special_t S(min_max.max.size, false);
        special_t P(min_max.max.size, false);
        A.setspec(spec1).add_bytes(RIGHT, min_max.max.size + 1);
        S.setspec(spec1.negate()).add_bytes(RIGHT, min_max.max.size + 1);
        P.setspec(spec2).add_bytes(LEFT, min_max.max.size).add_bytes(RIGHT, 1);

        for (int i = 0; i < min_max.max.size * BBITS; i++) {
            if (!(P.bytes_array[1] & 1) && (P.bytes_array[0] & 128)) {
                P += A;
                P >>= 1;
            }
            else if ((P.bytes_array[1] & 1) && !(P.bytes_array[0] & 128)) {
                P += S;
                P >>= 1;
            }
            else {
                P >>= 1;
            }
        }
        result.setspec(P);
        result >>= 8;
        result.resize(min_max.max.size + 1);

    }
        break;
    default:
        perror("wrong action!\n");
        break;
    }

    return result; 
}

//--------------------------------------------------------------------------------------------------------

special_t operator+ (special_t first, special_t second) { 
    return do_action(first, second, '+');
}
special_t operator- (special_t first, special_t second) { 
    return do_action(first, second, '-');
}
special_t operator* (special_t first, special_t second) {
    return do_action(first, second, '*');
}
special_t operator<< (special_t spec, int count) {
    special_t tmp;
    tmp.setspec(spec);
    return tmp.shift(LEFT, count);
}
special_t operator>> (special_t spec, int count) {
    special_t tmp;
    tmp.setspec(spec);
    return tmp.shift(RIGHT, count);
}

special_t& operator+= (special_t& first, special_t second) {
    return first.setspec(do_action(first, second, '+'));
}
special_t& operator-= (special_t& first, special_t second) {
    return first.setspec(do_action(first, second, '-'));
}
special_t& operator*= (special_t& first, special_t second) {
    return first.setspec(do_action(first, second, '*'));
}
special_t& operator<<= (special_t& spec, int count) {
    return spec.shift(LEFT, count);
}
special_t& operator>>= (special_t& spec, int count) {
    return spec.shift(RIGHT, count);
}

bool operator== (special_t first, special_t second) {
    Min_Max min_max = first.size < second.size ? Min_Max<special_t>{first, second} : Min_Max<special_t>{second, first};
    min_max.min.resize(min_max.max.size);

    for (int i = 0; i < min_max.max.size; i++) {
        if (min_max.min.bytes_array[i] != min_max.max.bytes_array[i])
            return false;
    }
    return true;
    
}
bool operator<= (special_t first, special_t second) {
    return first == second || first < second;
}
bool operator>= (special_t first, special_t second) {
    return first == second || first > second;
}
bool operator< (special_t first, special_t second) {
    special_t res = first - second;
    if (res.bytes_array[res.size - 1] & 128)
        return true;
    return false;
}
bool operator> (special_t first, special_t second) {
    special_t res = first - second;
    if (res.bytes_array[res.size - 1] & 128)
        return false;
    return true;
}


