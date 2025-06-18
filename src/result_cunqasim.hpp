#include <unordered_map>

#include "utils/types_cunqasim.hpp"

class ResultCunqa
{
public:
    int n_qubits;
    std::unordered_map<int, int> counts;
    double total_time;

    json get_ordered_counts();
    json to_json(); 
    
};



