#pragma once

#include <iostream>
#include <vector>
#include <complex>
#include <array>
#include <chrono>
#include <string>
#include <optional>

#include "instructions.hpp"
#include "result_cunqasim.hpp"
#include "utils/types_cunqasim.hpp"
#include "utils/utils_cunqasim.hpp"

//#include "logger/logger.hpp"


class Executor 
{
public:
    int n_qubits;
    StateVector statevector;

    Executor(int n_qubits);
    Executor(StateVector initial_state);

    ResultCunqa run(QuantumCircuit& quantumcircuit, int shots = 10);
    inline int apply_measure(std::array<int, 3>& qubits);
    inline void apply_gate(std::string& gate_name, std::array<int, 3>& qubits);
    inline void apply_parametric_gate(std::string& gate_name, std::array<int, 3>& qubits, std::vector<double>& param);
    inline int get_nonzero_position();
    inline void restart_statevector();
    
};

Executor::Executor(int n_qubits) : n_qubits{n_qubits}, statevector(1 << this->n_qubits)
{
    statevector[0] = 1.0;
}

Executor::Executor(StateVector initial_state) : n_qubits(initial_state.size()), statevector(initial_state)
{}


//TODO: Classical Registers
ResultCunqa Executor::run(QuantumCircuit& quantumcircuit, int shots)
{
    ResultCunqa result;
    std::string instruction_name;
    std::array<int, 3> qubits;
    Params param;

    result.n_qubits = this->n_qubits;

    //SPDLOG_LOGGER_DEBUG(logger, "In Cunqa run: variables defined and n_qubits set.");

    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < shots; i++) {

        for (auto& instruction : quantumcircuit) {
            instruction_name = instruction.at("name");
            qubits = instruction.at("qubits");
            switch (instructions_map[instruction_name])
            {
                case measure:
                    Instruction::apply_measure(this->statevector, qubits);
                    break;
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
                    Instruction::apply_instruction(this->statevector, instruction_name, qubits);
                    break;
                case rx:
                case ry:
                case rz:
                case c_if_rx:
                case c_if_ry:
                case c_if_rz:
                    param = instruction.at("params").get<Params>();
                    Instruction::apply_param_instruction(this->statevector, instruction_name, qubits, param);
                    break;
                
                default:
                    std::cout << "Error. Invalid gate name" << "\n";
                    break;
            }
        }
        
        int position_result = this->get_nonzero_position();
        //SPDLOG_LOGGER_DEBUG(logger, "position_result: {}", position_result);
        result.counts[position_result]++;
        
        restart_statevector();

    }

    auto stop_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);

    double total_time = duration.count();

    result.total_time = total_time;

    return result;
}


inline int Executor::apply_measure(std::array<int, 3>& qubits)
{
    meas_out meas =  Instruction::apply_measure(this->statevector, qubits);

    return meas.measure;
}

inline void Executor::apply_gate(std::string& gate_name, std::array<int, 3>& qubits)
{
    Instruction::apply_instruction(this->statevector, gate_name, qubits);
}

inline void Executor::apply_parametric_gate(std::string& gate_name, std::array<int, 3>& qubits, std::vector<double>& param)
{
    Instruction::apply_param_instruction(this->statevector, gate_name, qubits, param);
}

inline void Executor::restart_statevector()
{
    this->statevector.assign(this->statevector.size(), {0.0, 0.0});
    this->statevector[0] = 1.0;
}

inline int Executor::get_nonzero_position()
{
    int position;
    try {
        auto it = std::find_if(this->statevector.begin(), this->statevector.end(), [](const complex& c) {
            return c != std::complex<double>(0, 0); // Check for nonzero
        });
        position = std::distance(this->statevector.begin(), it);
    } catch (const std::exception& e) {
        //SPDLOG_LOGGER_ERROR(logger, "Error findind the non-zero position. Check if all qubits were measured.");
        return -1;
    }

    return position;
}

