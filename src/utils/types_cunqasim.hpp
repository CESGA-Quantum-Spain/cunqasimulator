#pragma once

#include <nlohmann/json.hpp>
#include <complex>

#if defined(USE_DOUBLE_PRECISION_MACRO)
    using Precision = double;
    #define MPI_COMPLEX MPI_CXX_Precision_COMPLEX
#elif defined(USE_FLOAT_PRECISION_MACRO)
    using Precision = float;
    #define MPI_COMPLEX MPI_CXX_FLOAT_COMPLEX
#endif

using json = nlohmann::json;
using QuantumCircuit = std::vector<json>; 
using State = std::complex<Precision>;
using StateVector = std::vector<State>;
using Matrix = std::vector<std::vector<State>>; // [[ROW],[ROW]]
using Params = std::vector<Precision>;

struct meas_out {
    StateVector statevector;
    int measure;
};

// Uncomment to compile Cunqasimulator isolated
namespace std {
    
    template< class T > void to_json(json &j, const std::complex< T > &p) {
        j = json {p.real(), p.imag()};
    }
    
    template< class T > void from_json(const json &j, std::complex< T > &p) {
        p.real(j.at(0));
        p.imag(j.at(1));
    }
}

