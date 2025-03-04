#include <iostream>
#include <vector>
#include <complex>
#include <array>

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

    //The qubits hardcoded to -1 are not used but they have to have a value.
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


    QPU qpu(5);

/*     StateVector sv = qpu.apply("h", {0});
    sv = qpu.apply("measure", {0});
    printvector(sv);

    sv = qpu.apply("c_if_x", {1,0});
    printvector(sv); */


    Result result = qpu.run(qc);
    std::cout << "Total time: " << result.total_time << " [ms] \n";
    std::cout << "Mean time per shot: " << result.mean_time_per_shot << " [ms] \n";

    Counts counts = result.get_counts();
    std::cout << counts.counts.dump(2) << "\n"; 










    




    


    return 0;
}