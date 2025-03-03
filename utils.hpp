#pragma once

int flipbit(int N, int p) {
    return N ^ (1 << p);  
}

bool is_zero(int N, int p) {
    bool bit = (N & (1 << p)) == 0;
    return bit;
}