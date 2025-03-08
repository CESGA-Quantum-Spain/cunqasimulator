#pragma once

#include <vector>
#include <algorithm>
#include <bitset>


#include "utils/types_cunqasim.hpp"

int get_nonzero_position(StateVector& statevector_list)
{
    int position;
    auto it = std::find_if(statevector_list.begin(), statevector_list.end(), [](const complex& c) {
        return c != std::complex<double>(0, 0); // Check for nonzero
    });

    position = std::distance(statevector_list.begin(), it);

    return position;
}

class ResultCunqa
{
public:
    int n_qubits;
    std::vector<StateVector> sample;
    json counts;
    double total_time;

    json to_json(); 
    Counts get_counts();
    

};

json ResultCunqa::to_json()
{
    this->counts = this->get_counts().counts;
    json attributes;
    attributes = {
        {"n_qubits", this->n_qubits},
        {"counts", this->counts},
        {"total_time[ms]", this->total_time}

    };

    return attributes;
}

Counts ResultCunqa::get_counts() 
{
    Counts result;
    int position = -1;
    std::unordered_map<int, int> counts_map;

    for (auto& one_result : this->sample) {
        position = get_nonzero_position(one_result);
        counts_map[position]++;
        result.counts_list.push_back(position);
    }

    for (const auto& [key, value] : counts_map) {
        std::string bit_key = std::bitset<32>(key).to_string();
        bit_key = bit_key.substr(bit_key.size() - this->n_qubits);
        result.counts[bit_key] = value;
    }

    return result;
}