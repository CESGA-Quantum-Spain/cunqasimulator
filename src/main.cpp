#include <iostream>
#include <vector>
#include <complex>
#include <array>
#include <nlohmann/json.hpp>

#include "qpu.hpp"
#include "../utils/utils.hpp"
#include "../utils/types.hpp"

void printvector (StateVector& sv)
{
    std::cout << "( ";
    for (int i = 0; i < sv.size(); i++) {
        std::cout << sv[i];
        if (i != sv.size() - 1) {
            std::cout << ", ";
        }
    } 
    std::cout << " ) \n";
}


int main() {


    QuantumCircuit qc = {
        {
            {"name","h"},
            {"qubits", {0,-1,-1}}
        },
        {
            {"name", "cx"},
            {"qubits", {0,1,-1}}
        },
        {
            {"name","measure"},
            {"qubits", {0,-1,-1}}
        },
        {
            {"name", "measure"},
            {"qubits", {1,-1,-1}}
        }
    };


    QPU qpu(2);

/*     StateVector sv = qpu.apply("h", {0});
    sv = qpu.apply("measure", {0});
    printvector(sv);

    sv = qpu.apply("c_if_x", {1,0});
    printvector(sv); */


    Result result = qpu.run(qc);
    std::cout << "Size: " << result.sample.size() << "\n";
    printvector(result.sample[65]);









    




    


    return 0;
}