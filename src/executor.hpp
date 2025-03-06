#pragma once

#include <iostream>
#include <vector>
#include <complex>
#include <array>
#include <chrono>

#include "instructions.hpp"
#include "result_cunqasim.hpp"
#include "utils/types_cunqasim.hpp"


class Executor 
{
public:
    int n_qubits;
    StateVector statevector;

    Executor(int n_qubits);
    Executor(StateVector initial_state);

    void restart_statevector();
    StateVector apply(std::string instruction_name, std::array<int, 3> qubits, Params param = {0.0});
    ResultCunqa run(QuantumCircuit& quantumcircuit, int shots = 10);

};

Executor::Executor(int n_qubits) : n_qubits{n_qubits}, statevector(1 << this->n_qubits)
{
    statevector[0] = 1.0;
}

Executor::Executor(StateVector initial_state) : n_qubits(initial_state.size()), statevector(initial_state)
{}

void Executor::restart_statevector()
{
    this->statevector.assign(this->statevector.size(), {0.0, 0.0});
    this->statevector[0] = 1.0;
}

StateVector Executor::apply(std::string instruction_name, std::array<int, 3> qubits, Params param)
{
    Instruction instruction(instruction_name);
    
    this->statevector = instruction.apply_instruction(this->statevector, param, qubits, this->n_qubits);

    return this->statevector;
}



//TODO: Classical Registers
ResultCunqa Executor::run(QuantumCircuit& quantumcircuit, int shots)
{
    ResultCunqa result;
    std::string instruction_name;
    std::array<int, 3> qubits;
    Params param;

    result.n_qubits = this->n_qubits;

    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < shots; i++) {

        for (auto& instruction : quantumcircuit) {
            instruction_name = instruction.at("name");
            qubits = instruction.at("qubits");
            switch (instructions_map[instruction_name])
            {
                case measure:
                case id:
                case x:
                case y:
                case z:
                case h:
                case cx:
                case cy:
                case cz:
                case ecr:
                case c_if_x:
                case c_if_y:
                case c_if_z:
                case c_if_cx:
                case c_if_cy:
                case c_if_cz:
                    this->statevector = this->apply(instruction_name, qubits);
                    break;
                case rx:
                case ry:
                case rz:
                case c_if_rx:
                case c_if_ry:
                case c_if_rz:
                    param = instruction.at("params").get<Params>();
                    this->statevector = this->apply(instruction_name, qubits, param);
                    break;
                default:
                    std::cout << "Error. Invalid gate name" << "\n";
                    break;
            }
        }

        result.sample.push_back(this->statevector);
        restart_statevector();

    }

    auto stop_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);

    double total_time = duration.count();

    result.total_time = total_time;

    return result;
}


