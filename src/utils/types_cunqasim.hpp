#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;
using QuantumCircuit = std::vector<json>; 
// using DistributedQuantumCircuit = std::vector<json>;
using complex = std::complex<double>;
using StateVector = std::vector<complex>;
using Matrix = std::vector<std::vector<complex>>; // [[ROW],[ROW]]
using Params = std::vector<double>;

struct meas_out {
    StateVector statevector;
    int measure;
};

namespace nlohmann {
    template <>
    struct adl_serializer<complex> {
        static void to_json(json& j, const complex& c) 
        {
            j = json::array({ c.real(), c.imag() });
        }
  
        static void from_json(const json& j, complex& c)
        {
            if (!j.is_array() || j.size() != 2) {
                throw std::invalid_argument("Expected JSON array of size 2 for complex number");
            }
            c = complex(j[0].get<double>(), j[1].get<double>());
        }
    };
}

