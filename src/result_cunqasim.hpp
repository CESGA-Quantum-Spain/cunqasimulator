#pragma once

#include <vector>
#include <algorithm>
#include <bitset>


#include "utils/types_cunqasim.hpp"
#include "utils/utils_cunqasim.hpp"
#include "utils/constants_cunqasim.hpp"


class ResultCunqa
{
public:
    int n_qubits;
    std::unordered_map<int, int> counts;
    double total_time;

    json get_ordered_counts();
    json to_json(bool flip_counts_order = false); 
    
};

json ResultCunqa::get_ordered_counts()
{
    json count_js = {};

    for (const auto& [key, value] : this->counts) {
        int new_key = formatbits(key, this->n_qubits);
        std::string new_key_str = std::to_string(new_key);
        count_js.at(new_key_str) = value;
    }

    return counts_js;
}

json ResultCunqa::to_json(bool flip_counts_order)
{
    json attributes;
    json counts_js;

    if (flip_counts_order) {
        counts_js = this->get_ordered_counts();
    } else {
        counts_js = this->counts;
    }

    attributes.at("n_qubits") = this->n_qubits;
    attributes.at("counts") = counts_js;
    attributes.at("total_time[ms]") = this->total_time; 

    return attributes;
}

