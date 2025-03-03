#include <iostream>
#include <vector>
#include <complex>
#include <array>
#include <nlohmann/json.hpp>

#include "qpu.hpp"
#include "utils.hpp"

using json = nlohmann::json;
using QuantumCircuit = std::vector<json>; 
using complex = std::complex<double>;
using StateVector = std::vector<complex>;


int main() {


    QuantumCircuit qc = {
        {
            {"name","h"},
            {"qubits", {0,-1}}
        },
        {
            {"name", "cx"},
            {"qubits", {0,1}}
        },
        {
            {"name","measure"},
            {"qubits", {0,-1}}
        },
        {
            {"name", "measure"},
            {"qubits", {1,-1}}
        }
    };


    QPU qpu(2);

    //StateVector sv = qpu.run(qc);
    StateVector sv = qpu.apply("h", {0,-1});
    sv = qpu.apply("measure", {0,-1});

    
    for (int i = 0; i < sv.size(); i++) {
        std::cout << sv[i] << "\n";
    } 


    




    


    return 0;
}