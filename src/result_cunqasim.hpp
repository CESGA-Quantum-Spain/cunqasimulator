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
    json counts_js = {};

    for (const auto& [key, value] : this->counts) {
        int new_key = reverse_bits(key, this->n_qubits);
        std::cout << "New int: " << new_key << "\n";
        std::string new_key_str = std::to_string(new_key);
        counts_js[new_key_str] = value;
    }

    return counts_js;
}

json ResultCunqa::to_json()
{
    json attributes;
    json counts_js = this->counts;

    attributes["n_qubits"] = this->n_qubits;
    attributes["counts"] = counts_js;
    attributes["total_time[ms]"] = this->total_time; 

    return attributes;
}

