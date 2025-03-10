#pragma once

#include <vector>
#include <algorithm>
#include <bitset>


#include "utils/types_cunqasim.hpp"
#include "utils/utils_cunqasim.hpp"


class ResultCunqa
{
public:
    int n_qubits;
    json counts;
    double total_time;

    json to_json(); 
    

};

json ResultCunqa::to_json()
{
    //this->counts = this->get_counts().counts;
    json attributes;
    attributes = {
        {"n_qubits", this->n_qubits},
        {"counts", this->counts},
        {"total_time[ms]", this->total_time}

    };

    return attributes;
}
