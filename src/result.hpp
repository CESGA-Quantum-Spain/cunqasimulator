#pragma once

#include <vector>
#include <algorithm>
#include <bitset>


#include "../utils/types.hpp"

int get_nonzero_position(StateVector& statevector_list)
{
    int position;
    auto it = std::find_if(statevector_list.begin(), statevector_list.end(), [](const complex& c) {
        return c != std::complex<double>(0, 0); // Check for nonzero
    });

    position = std::distance(statevector_list.begin(), it);

    return position;
}

class Result
{
public:
    std::vector<StateVector> sample;

    Counts get_counts()
    {
        Counts result_counts;
        int position = -1;
        std::unordered_map<int, int> counts_map;

        for (auto& result : this->sample) {
            position = get_nonzero_position(result);
            counts_map[position]++;
            result_counts.counts_list.push_back(position);
        }

        for (const auto& [key, value] : counts_map) {
            std::string bit_key = std::bitset<16>(key).to_string();
            result_counts.counts[bit_key] = value;
        }

        return result_counts;
    }

};