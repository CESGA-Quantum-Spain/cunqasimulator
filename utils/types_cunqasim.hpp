#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;
using QuantumCircuit = std::vector<json>; 
using DistributedQuantumCircuit = std::vector<json>;
using complex = std::complex<double>;
using StateVector = std::vector<complex>;

struct meas_out {
    StateVector statevector;
    int measure;
};

struct Counts {
    std::vector<int> counts_list;
    json counts;
};