#pragma once

#include <iostream>
#include <vector>
#include <complex>
#include <array>
#include <chrono>
#include <string>

#include "instructions.hpp"
#include "result_cunqasim.hpp"
#include "utils/types_cunqasim.hpp"
#include "utils/utils_cunqasim.hpp"


class Executor 
{
public:
    int n_qubits;
    StateVector statevector;

    Executor(int n_qubits);
    Executor(StateVector initial_state);

    void restart_statevector();

    #if defined(QPU_MPI) || defined(NO_COMM)
    ResultCunqa run(QuantumCircuit& quantumcircuit, int shots = 10);
    inline StateVector apply(std::string instruction_name, std::array<int, 3> qubits, Params param = {0.0}, std::array<int, 2> qpus = {-1,-1});
    #elif defined(QPU_ZMQ)
    ResultCunqa run(QuantumCircuit& quantumcircuit, ZMQSockets& zmq_sockets, int shots = 10);
    inline StateVector apply(std::string instruction_name, std::array<int, 3> qubits, std::array<std::string, 2> qpus, ZMQSockets& zmq_sockets, Params param = {0.0});
    #endif
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



inline StateVector Executor::apply(std::string instruction_name, std::array<int, 3> qubits, Params param, std::array<int, 2> qpus)
{
    //Instruction instruction(instruction_name);
    
    this->statevector = Instruction::apply_instruction(instruction_name, this->statevector, param, qubits, qpus);

    return this->statevector;
}

#if defined(NO_COMM) || defined(QPU_MPI)
//TODO: Classical Registers
ResultCunqa Executor::run(QuantumCircuit& quantumcircuit, int shots)
{
    ResultCunqa result;
    std::string instruction_name;
    std::array<int, 3> qubits;
    Params param;
    std::array<int, 2> qpus;

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
                case c_if_h:
                case c_if_x:
                case c_if_y:
                case c_if_z:
                case c_if_cx:
                case c_if_cy:
                case c_if_cz:
                case c_if_ecr:
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
                case d_c_if_h:
                case d_c_if_x:
                case d_c_if_y:
                case d_c_if_z:
                case d_c_if_cx:
                case d_c_if_cy:
                case d_c_if_cz:
                case d_c_if_ecr:
                    qpus = instruction.at("qpus").get<std::array<int, 2>>();
                    param = {0};
                    this->statevector = this->apply(instruction_name, qubits, param, qpus);
                    break;
                case d_c_if_rx:
                case d_c_if_ry:
                case d_c_if_rz:
                    param = instruction.at("params").get<Params>();
                    qpus = instruction.at("qpus").get<std::array<int, 2>>();
                    this->statevector = this->apply(instruction_name, qubits, param, qpus);
                    break;
                default:
                    std::cout << "Error. Invalid gate name" << "\n";
                    break;
            }
        }
        
        int position_result = get_nonzero_position(statevector);
        result.counts[position_result]++;
        
        restart_statevector();

    }

    auto stop_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);

    double total_time = duration.count();

    result.total_time = total_time;

    return result;
}

#elif defined(QPU_ZMQ)
//TODO
inline StateVector Executor::d_apply(std::string instruction_name, std::array<int, 3> qubits, std::array<std::string, 2> qpus, ZMQSockets& zmq_sockets, Params param)
{
    //Instruction instruction(instruction_name);
    
    this->statevector = Instruction::apply_instruction(instruction_name, this->statevector, param, qubits, qpus, zmq_sockets);

    return this->statevector;
}

ResultCunqa Executor::run(QuantumCircuit& quantumcircuit, ZMQSockets& zmq_sockets, int shots)
{
    ResultCunqa result;
    std::string instruction_name;
    std::array<int, 3> qubits;
    Params param;
    std::array<int, 2> qpus;

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
                case c_if_h:
                case c_if_x:
                case c_if_y:
                case c_if_z:
                case c_if_cx:
                case c_if_cy:
                case c_if_cz:
                case c_if_ecr:
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
                case d_c_if_h:
                case d_c_if_x:
                case d_c_if_y:
                case d_c_if_z:
                case d_c_if_cx:
                case d_c_if_cy:
                case d_c_if_cz:
                case d_c_if_ecr:
                    qpus = instruction.at("qpus").get<std::array<std::string, 2>>();
                    param = {0};
                    this->statevector = this->d_apply(instruction_name, qubits, param, qpus, zmq_sockets);
                    break;
                case d_c_if_rx:
                case d_c_if_ry:
                case d_c_if_rz:
                    param = instruction.at("params").get<Params>();
                    qpus = instruction.at("qpus").get<std::array<std::string, 2>>();
                    this->statevector = this->d_apply(instruction_name, qubits, param, qpus, zmq_sockets);
                    break;
                default:
                    std::cout << "Error. Invalid gate name" << "\n";
                    break;
            }
        }
        
        int position_result = get_nonzero_position(statevector);
        result.counts[position_result]++;
        
        restart_statevector();

    }

    auto stop_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);

    double total_time = duration.count();

    result.total_time = total_time;

    return result;
}

#else

    std::cout << "In CunqaSimulator: No QPU-communication MACRO defined." << "\n";
    
#endif
