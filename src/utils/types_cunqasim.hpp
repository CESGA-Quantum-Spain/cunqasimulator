#pragma once

#include <nlohmann/json.hpp>
#include <complex>

using json = nlohmann::json;
using QuantumCircuit = std::vector<json>; 
// using DistributedQuantumCircuit = std::vector<json>;
using State = std::complex<double>;
using StateVector = std::vector<State>;
using Matrix = std::vector<std::vector<State>>; // [[ROW],[ROW]]
using Params = std::vector<double>;

struct meas_out {
    StateVector statevector;
    int measure;
};

//Uncomment to test cunqasimulator isolated
/* namespace nlohmann  {
    template <>
    struct adl_serializer<State> {
        static void to_json(json& j, const State& c) 
        {
            j = json::array({ c.real(), c.imag() });
        }
  
        static void from_json(const json& j, State& c)
        {
            if (!j.is_array() || j.size() != 2) {
                throw std::invalid_argument("Expected JSON array of size 2 for complex number");
            }
            c = State(j[0].get<double>(), j[1].get<double>());
        }
    };
} */

